// <bits/chrono_tzdb.h> — chrono time-zone runtime ([time.zone]).
// Ported from libc++ __chrono/{time_zone,time_zone_link,leap_second,tzdb,
// tzdb_list,zoned_time}.h (release/20.x). Reads the compact constexpr database
// in <bits/chrono_tzdb_data.h>.
//
// namespace std; chrono internals in std::detail; no underscore-prefixed names;
// exceptions OFF (throw paths -> __builtin_trap, matching chrono_format.h).
// Included from the tail of <bits/chrono_format.h>.
#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <compare>
#include <cstdlib>
#include <cstring>
#include <type_traits>
#include <unistd.h>

#include <bits/chrono_tzdb_data.h>
#include <__utility/private_constructor_tag.h>

namespace std {
namespace chrono {

// ---- choose ([time.zone.timezone.overview]) --------------------------------
enum class choose { earliest, latest };

// ===========================================================================
// time_zone ([time.zone.timezone])
// ===========================================================================
class time_zone {
public:
  explicit time_zone(const tzdb_data::zone_t* zone) noexcept : zone_(zone) {}

  time_zone(time_zone&&)            = default;
  time_zone& operator=(time_zone&&) = default;

  time_zone(const time_zone&)            = delete;
  time_zone& operator=(const time_zone&) = delete;

  [[nodiscard]] string_view name() const noexcept { return zone_->name; }

  // get_info(sys_time)
  template <class Duration>
  [[nodiscard]] sys_info get_info(const sys_time<Duration>& time) const {
    using namespace tzdb_data;
    sys_seconds sec = chrono::time_point_cast<seconds>(time);
    long long t     = sec.time_since_epoch().count();

    const trans_t* first = transitions + zone_->first;
    const trans_t* last  = first + zone_->count;

    // Find the first transition whose utc is > t.
    const trans_t* it = std::upper_bound(
        first, last, t, [](long long value, const trans_t& tr) { return value < tr.utc; });

    if (it == first) {
      // Before the zone's first kept transition: use init_* state.
      return sys_info{
          sys_seconds::min(),
          (zone_->count == 0) ? sys_seconds::max() : sys_seconds{seconds{first->utc}},
          seconds{zone_->init_off},
          chrono::duration_cast<minutes>(seconds{zone_->init_save}),
          string(abbrev_pool + zone_->init_ab)};
    }

    const trans_t* cur = it - 1;
    return sys_info{
        sys_seconds{seconds{cur->utc}},
        (it == last) ? sys_seconds::max() : sys_seconds{seconds{it->utc}},
        seconds{cur->off},
        chrono::duration_cast<minutes>(seconds{cur->save}),
        string(abbrev_pool + cur->ab)};
  }

  // get_info(local_time) ([time.zone.timezone] / libc++ __get_info)
  template <class Duration>
  [[nodiscard]] local_info get_info(const local_time<Duration>& time) const {
    seconds local = chrono::time_point_cast<seconds>(time).time_since_epoch();

    sys_info first  = get_info(sys_seconds{local} - get_first_offset());
    sys_info second = get_info(sys_seconds{local} - first.offset);

    if (first.offset == second.offset) {
      // Single candidate -> unique.
      return local_info{local_info::unique, first, sys_info{}};
    }

    // Two distinct candidate offsets surround the local time. Classify whether
    // the local time falls in the overlap (ambiguous), the gap (nonexistent),
    // or is unique against one of them.
    sys_seconds local_sec{local};

    // Candidate using the earlier (pre-transition) offset.
    sys_seconds sys_first = local_sec - first.offset;
    // Candidate using the later (post-transition) offset.
    sys_seconds sys_second = local_sec - second.offset;

    bool in_first  = sys_first >= first.begin && sys_first < first.end;
    bool in_second = sys_second >= second.begin && sys_second < second.end;

    if (in_first && in_second)
      return local_info{local_info::ambiguous, first, second};
    if (in_first)
      return local_info{local_info::unique, first, sys_info{}};
    if (in_second)
      return local_info{local_info::unique, second, sys_info{}};
    return local_info{local_info::nonexistent, first, second};
  }

  // to_sys(local_time) -> unique or throw
  template <class Duration>
  [[nodiscard]] sys_time<common_type_t<Duration, seconds>>
  to_sys(const local_time<Duration>& time) const {
    local_info info = get_info(time);
    if (info.result == local_info::nonexistent) {
#ifdef __cpp_exceptions
      throw nonexistent_local_time(time, info);
#else
      __builtin_trap();
#endif
    } else if (info.result == local_info::ambiguous) {
#ifdef __cpp_exceptions
      throw ambiguous_local_time(time, info);
#else
      __builtin_trap();
#endif
    }
    using D = common_type_t<Duration, seconds>;
    return sys_time<D>{chrono::duration_cast<D>(time.time_since_epoch()) - info.first.offset};
  }

  // to_sys(local_time, choose)
  template <class Duration>
  [[nodiscard]] sys_time<common_type_t<Duration, seconds>>
  to_sys(const local_time<Duration>& time, choose z) const {
    local_info info = get_info(time);
    using D         = common_type_t<Duration, seconds>;
    switch (info.result) {
    case local_info::unique:
      return sys_time<D>{chrono::duration_cast<D>(time.time_since_epoch()) - info.first.offset};
    case local_info::nonexistent:
      // Both candidates collapse to the transition instant.
      return info.first.end;
    case local_info::ambiguous:
      switch (z) {
      case choose::earliest:
        return sys_time<D>{chrono::duration_cast<D>(time.time_since_epoch()) - info.first.offset};
      case choose::latest:
        return sys_time<D>{chrono::duration_cast<D>(time.time_since_epoch()) - info.second.offset};
      }
    }
    return sys_time<D>{chrono::duration_cast<D>(time.time_since_epoch()) - info.first.offset};
  }

  template <class Duration>
  [[nodiscard]] local_time<common_type_t<Duration, seconds>>
  to_local(const sys_time<Duration>& time) const {
    using D = common_type_t<Duration, seconds>;
    sys_info info = get_info(time);
    return local_time<D>{(time + info.offset).time_since_epoch()};
  }

  [[nodiscard]] const tzdb_data::zone_t* implementation() const noexcept { return zone_; }

private:
  // The offset active at the zone's first kept transition's predecessor,
  // i.e. the init offset; used as the seed for the local_time lookup.
  [[nodiscard]] seconds get_first_offset() const {
    return seconds{zone_->init_off};
  }

  const tzdb_data::zone_t* zone_;
};

[[nodiscard]] inline bool operator==(const time_zone& x, const time_zone& y) noexcept {
  return x.name() == y.name();
}
[[nodiscard]] inline strong_ordering operator<=>(const time_zone& x, const time_zone& y) noexcept {
  return x.name() <=> y.name();
}

// ===========================================================================
// time_zone_link ([time.zone.link])
// ===========================================================================
class time_zone_link {
public:
  explicit time_zone_link(const tzdb_data::link_t* link) noexcept : link_(link) {}

  time_zone_link(time_zone_link&&)            = default;
  time_zone_link& operator=(time_zone_link&&) = default;

  [[nodiscard]] string_view name() const noexcept { return link_->name; }
  [[nodiscard]] string_view target() const noexcept { return link_->target; }

private:
  const tzdb_data::link_t* link_;
};

[[nodiscard]] inline bool operator==(const time_zone_link& x, const time_zone_link& y) noexcept {
  return x.name() == y.name();
}
[[nodiscard]] inline strong_ordering operator<=>(const time_zone_link& x, const time_zone_link& y) noexcept {
  return x.name() <=> y.name();
}

// ===========================================================================
// leap_second ([time.zone.leap])
// ===========================================================================
class leap_second {
public:
  constexpr leap_second(const sys_seconds& date, const seconds& value) noexcept
      : date_(date), value_(value) {}

  // libc++-compatible access-restricted constructor used by the conformance
  // test support header <__utility/private_constructor_tag.h>.
  constexpr leap_second(std::__private_constructor_tag, const sys_seconds& date, const seconds& value) noexcept
      : date_(date), value_(value) {}

  constexpr leap_second(const leap_second&)            = default;
  constexpr leap_second& operator=(const leap_second&) = default;

  [[nodiscard]] constexpr sys_seconds date() const noexcept { return date_; }
  [[nodiscard]] constexpr seconds value() const noexcept { return value_; }

private:
  sys_seconds date_;
  seconds value_;
};

[[nodiscard]] constexpr bool operator==(const leap_second& x, const leap_second& y) noexcept {
  return x.date() == y.date();
}
[[nodiscard]] constexpr strong_ordering operator<=>(const leap_second& x, const leap_second& y) noexcept {
  return x.date() <=> y.date();
}

template <class Duration>
[[nodiscard]] constexpr bool operator==(const leap_second& x, const sys_time<Duration>& y) noexcept {
  return x.date() == y;
}
template <class Duration>
[[nodiscard]] constexpr bool operator<(const leap_second& x, const sys_time<Duration>& y) noexcept {
  return x.date() < y;
}
template <class Duration>
[[nodiscard]] constexpr bool operator<(const sys_time<Duration>& x, const leap_second& y) noexcept {
  return x < y.date();
}
template <class Duration>
[[nodiscard]] constexpr bool operator>(const leap_second& x, const sys_time<Duration>& y) noexcept {
  return y < x;
}
template <class Duration>
[[nodiscard]] constexpr bool operator>(const sys_time<Duration>& x, const leap_second& y) noexcept {
  return y < x;
}
template <class Duration>
[[nodiscard]] constexpr bool operator<=(const leap_second& x, const sys_time<Duration>& y) noexcept {
  return !(y < x);
}
template <class Duration>
[[nodiscard]] constexpr bool operator<=(const sys_time<Duration>& x, const leap_second& y) noexcept {
  return !(y < x);
}
template <class Duration>
[[nodiscard]] constexpr bool operator>=(const leap_second& x, const sys_time<Duration>& y) noexcept {
  return !(x < y);
}
template <class Duration>
[[nodiscard]] constexpr bool operator>=(const sys_time<Duration>& x, const leap_second& y) noexcept {
  return !(x < y);
}
template <class Duration>
  requires three_way_comparable_with<sys_seconds, sys_time<Duration>>
[[nodiscard]] constexpr auto operator<=>(const leap_second& x, const sys_time<Duration>& y) noexcept {
  return x.date() <=> y;
}

// ===========================================================================
// tzdb ([time.zone.db.tzdb])
// ===========================================================================
struct tzdb {
  string version;
  vector<time_zone> zones;
  vector<time_zone_link> links;
  vector<leap_second> leap_seconds;

  [[nodiscard]] const time_zone* locate_zone(string_view tz_name) const {
    if (const time_zone* tz = find_zone(tz_name))
      return tz;

    // Try links.
    {
      auto it = std::lower_bound(
          links.begin(), links.end(), tz_name, [](const time_zone_link& l, string_view n) {
            return l.name() < n;
          });
      if (it != links.end() && it->name() == tz_name) {
        if (const time_zone* tz = find_zone(it->target()))
          return tz;
      }
    }

    // Not found.
    __builtin_trap();
  }

  [[nodiscard]] const time_zone* current_zone() const;

private:
  [[nodiscard]] const time_zone* find_zone(string_view tz_name) const {
    auto it = std::lower_bound(
        zones.begin(), zones.end(), tz_name, [](const time_zone& z, string_view n) {
          return z.name() < n;
        });
    if (it != zones.end() && it->name() == tz_name)
      return &*it;
    return nullptr;
  }
};

// ===========================================================================
// tzdb_list ([time.zone.db.list])
// ===========================================================================
class tzdb_list {
public:
  tzdb_list(const tzdb_list&)            = delete;
  tzdb_list& operator=(const tzdb_list&) = delete;

  explicit tzdb_list(tzdb&& db) { list_.push_back(std::move(db)); }

  using const_iterator = std::vector<tzdb>::const_iterator;

  [[nodiscard]] const tzdb& front() const noexcept { return list_.front(); }

  const_iterator erase_after(const_iterator p) { return list_.erase(std::next(p)); }

  [[nodiscard]] const_iterator begin() const noexcept { return list_.begin(); }
  [[nodiscard]] const_iterator end() const noexcept { return list_.end(); }
  [[nodiscard]] const_iterator cbegin() const noexcept { return list_.cbegin(); }
  [[nodiscard]] const_iterator cend() const noexcept { return list_.cend(); }

private:
  std::vector<tzdb> list_;
};

// ===========================================================================
// Singleton construction + free functions ([time.zone.db.access])
// ===========================================================================
namespace tzdb_detail {
[[nodiscard]] inline tzdb make_tzdb() {
  using namespace tzdb_data;
  tzdb db;
  db.version = version;

  db.zones.reserve(zone_count);
  for (unsigned i = 0; i < zone_count; ++i)
    db.zones.emplace_back(time_zone{&zones[i]});

  db.links.reserve(link_count);
  for (unsigned i = 0; i < link_count; ++i)
    db.links.emplace_back(time_zone_link{&links[i]});

  db.leap_seconds.reserve(leap_count);
  long long cumulative = 0;
  for (unsigned i = 0; i < leap_count; ++i) {
    db.leap_seconds.emplace_back(
        leap_second{sys_seconds{seconds{leaps[i].date}}, seconds{leaps[i].value}});
  }
  (void)cumulative;
  return db;
}

[[nodiscard]] inline tzdb_list& tzdb_list_singleton() {
  static tzdb_list list{make_tzdb()};
  return list;
}
} // namespace tzdb_detail

[[nodiscard]] inline tzdb_list& get_tzdb_list() { return tzdb_detail::tzdb_list_singleton(); }

[[nodiscard]] inline const tzdb& get_tzdb() { return get_tzdb_list().front(); }

[[nodiscard]] inline const time_zone* locate_zone(string_view tz_name) {
  return get_tzdb().locate_zone(tz_name);
}

[[nodiscard]] inline const time_zone* current_zone() { return get_tzdb().current_zone(); }

[[nodiscard]] inline string remote_version() { return string(tzdb_data::version); }

[[nodiscard]] inline const tzdb& reload_tzdb() { return get_tzdb(); }

// tzdb::current_zone defined out-of-line (needs locate_zone).
inline const time_zone* tzdb::current_zone() const {
  // Resolves a zone name (or link name) to a time_zone* without trapping.
  auto try_locate = [this](string_view name) -> const time_zone* {
    if (name.empty())
      return nullptr;
    if (const time_zone* z = find_zone(name))
      return z;
    auto it = std::lower_bound(
        links.begin(), links.end(), name, [](const time_zone_link& l, string_view nm) {
          return l.name() < nm;
        });
    if (it != links.end() && it->name() == name)
      if (const time_zone* z = find_zone(it->target()))
        return z;
    return nullptr;
  };

  // $TZ takes priority (matches libc++ and POSIX).
  if (const char* tz = std::getenv("TZ"))
    if (const time_zone* z = try_locate(string_view{tz}))
      return z;

  // /etc/localtime symlink: .../zoneinfo/<NAME>
  {
    char buf[4096];
    ssize_t n = ::readlink("/etc/localtime", buf, sizeof(buf) - 1);
    if (n > 0) {
      buf[n]          = '\0';
      string_view path{buf, static_cast<size_t>(n)};
      const char* key = "/zoneinfo/";
      size_t pos      = path.rfind(key);
      if (pos != string_view::npos)
        if (const time_zone* z = try_locate(path.substr(pos + std::strlen(key))))
          return z;
    }
  }

  // Fall back to UTC.
  return locate_zone("UTC");
}

// ===========================================================================
// zoned_traits ([time.zone.zonedtraits]) + zoned_time ([time.zone.zonedtime])
// ===========================================================================
template <class T>
struct zoned_traits {};

template <>
struct zoned_traits<const time_zone*> {
  [[nodiscard]] static const time_zone* default_zone() { return chrono::locate_zone("UTC"); }
  [[nodiscard]] static const time_zone* locate_zone(string_view name) {
    return chrono::locate_zone(name);
  }
};

namespace tzdb_detail {
// Detect zoned_traits<T> capabilities.
template <class T, class = void>
inline constexpr bool zoned_has_default_zone = false;
template <class T>
inline constexpr bool zoned_has_default_zone<T, void_t<decltype(zoned_traits<T>::default_zone())>> = true;

template <class T, class = void>
inline constexpr bool zoned_has_locate_zone = false;
template <class T>
inline constexpr bool zoned_has_locate_zone<
    T, void_t<decltype(zoned_traits<T>::locate_zone(string_view{}))>> = true;

template <class>
inline constexpr bool is_zoned_time_v = false;
} // namespace tzdb_detail

template <class Duration, class TimeZonePtr = const time_zone*>
class zoned_time {
public:
  using duration = common_type_t<Duration, seconds>;

private:
  using traits = zoned_traits<TimeZonePtr>;

public:
  // default
  zoned_time()
    requires tzdb_detail::zoned_has_default_zone<TimeZonePtr>
      : zone_(traits::default_zone()) {}

  zoned_time(const zoned_time&)            = default;
  zoned_time& operator=(const zoned_time&) = default;

  // from sys_time
  zoned_time(const sys_time<Duration>& st)
    requires tzdb_detail::zoned_has_default_zone<TimeZonePtr>
      : zone_(traits::default_zone()), tp_(st) {}

  // from TimeZonePtr
  explicit zoned_time(TimeZonePtr z) : zone_(std::move(z)) {}

  // from name
  explicit zoned_time(string_view name)
    requires tzdb_detail::zoned_has_locate_zone<TimeZonePtr>
      : zone_(traits::locate_zone(name)) {}

  // from another zoned_time (same TimeZonePtr, convertible duration)
  template <class Duration2>
  zoned_time(const zoned_time<Duration2, TimeZonePtr>& zt)
    requires is_convertible_v<sys_time<Duration2>, sys_time<Duration>>
      : zone_(zt.get_time_zone()), tp_(zt.get_sys_time()) {}

  // TimeZonePtr + sys_time
  zoned_time(TimeZonePtr z, const sys_time<Duration>& st) : zone_(std::move(z)), tp_(st) {}

  // name + sys_time
  zoned_time(string_view name, const sys_time<Duration>& st)
    requires tzdb_detail::zoned_has_locate_zone<TimeZonePtr>
      : zone_(traits::locate_zone(name)), tp_(st) {}

  // TimeZonePtr + local_time
  zoned_time(TimeZonePtr z, const local_time<Duration>& lt)
    requires requires(TimeZonePtr p, local_time<Duration> l) { p->to_sys(l); }
      : zone_(std::move(z)), tp_(zone_->to_sys(lt)) {}

  // name + local_time
  zoned_time(string_view name, const local_time<Duration>& lt)
    requires(tzdb_detail::zoned_has_locate_zone<TimeZonePtr> &&
             requires(TimeZonePtr p, local_time<Duration> l) { p->to_sys(l); })
      : zoned_time(traits::locate_zone(name), lt) {}

  // TimeZonePtr + local_time + choose
  zoned_time(TimeZonePtr z, const local_time<Duration>& lt, choose c)
    requires requires(TimeZonePtr p, local_time<Duration> l, choose ch) { p->to_sys(l, ch); }
      : zone_(std::move(z)), tp_(zone_->to_sys(lt, c)) {}

  // name + local_time + choose
  zoned_time(string_view name, const local_time<Duration>& lt, choose c)
    requires(tzdb_detail::zoned_has_locate_zone<TimeZonePtr> &&
             requires(TimeZonePtr p, local_time<Duration> l, choose ch) { p->to_sys(l, ch); })
      : zoned_time(traits::locate_zone(name), lt, c) {}

  // TimeZonePtr + zoned_time<Duration2, TimeZonePtr2>
  template <class Duration2, class TimeZonePtr2>
  zoned_time(TimeZonePtr z, const zoned_time<Duration2, TimeZonePtr2>& zt)
    requires is_convertible_v<sys_time<Duration2>, sys_time<Duration>>
      : zone_(std::move(z)), tp_(zt.get_sys_time()) {}

  // TimeZonePtr + zoned_time + choose
  template <class Duration2, class TimeZonePtr2>
  zoned_time(TimeZonePtr z, const zoned_time<Duration2, TimeZonePtr2>& zt, choose)
    requires is_convertible_v<sys_time<Duration2>, sys_time<Duration>>
      : zone_(std::move(z)), tp_(zt.get_sys_time()) {}

  // name + zoned_time<Duration2, TimeZonePtr2>
  template <class Duration2, class TimeZonePtr2>
  zoned_time(string_view name, const zoned_time<Duration2, TimeZonePtr2>& zt)
    requires(tzdb_detail::zoned_has_locate_zone<TimeZonePtr> &&
             is_convertible_v<sys_time<Duration2>, sys_time<Duration>>)
      : zoned_time(traits::locate_zone(name), zt) {}

  // name + zoned_time + choose
  template <class Duration2, class TimeZonePtr2>
  zoned_time(string_view name, const zoned_time<Duration2, TimeZonePtr2>& zt, choose c)
    requires(tzdb_detail::zoned_has_locate_zone<TimeZonePtr> &&
             is_convertible_v<sys_time<Duration2>, sys_time<Duration>>)
      : zoned_time(traits::locate_zone(name), zt, c) {}

  // operator=
  zoned_time& operator=(const sys_time<Duration>& st) {
    tp_ = st;
    return *this;
  }
  zoned_time& operator=(const local_time<Duration>& lt) {
    tp_ = zone_->to_sys(lt);
    return *this;
  }

  [[nodiscard]] operator sys_time<duration>() const { return get_sys_time(); }
  [[nodiscard]] explicit operator local_time<duration>() const { return get_local_time(); }

  [[nodiscard]] TimeZonePtr get_time_zone() const { return zone_; }
  [[nodiscard]] local_time<duration> get_local_time() const { return zone_->to_local(tp_); }
  [[nodiscard]] sys_time<duration> get_sys_time() const { return tp_; }
  [[nodiscard]] sys_info get_info() const { return zone_->get_info(tp_); }

private:
  TimeZonePtr zone_;
  sys_time<duration> tp_{};
};

zoned_time() -> zoned_time<seconds>;

template <class Duration>
zoned_time(sys_time<Duration>) -> zoned_time<common_type_t<Duration, seconds>>;

namespace tzdb_detail {
template <class TimeZonePtrOrName>
using time_zone_representation =
    conditional_t<is_convertible_v<TimeZonePtrOrName, string_view>,
                  const time_zone*,
                  remove_cvref_t<TimeZonePtrOrName>>;
} // namespace tzdb_detail

template <class TimeZonePtrOrName>
zoned_time(TimeZonePtrOrName&&)
    -> zoned_time<seconds, tzdb_detail::time_zone_representation<TimeZonePtrOrName>>;

template <class TimeZonePtrOrName, class Duration>
zoned_time(TimeZonePtrOrName&&, sys_time<Duration>)
    -> zoned_time<common_type_t<Duration, seconds>,
                  tzdb_detail::time_zone_representation<TimeZonePtrOrName>>;

template <class TimeZonePtrOrName, class Duration>
zoned_time(TimeZonePtrOrName&&, local_time<Duration>, choose = choose::earliest)
    -> zoned_time<common_type_t<Duration, seconds>,
                  tzdb_detail::time_zone_representation<TimeZonePtrOrName>>;

template <class Duration, class TimeZonePtrOrName, class TimeZonePtr2>
zoned_time(TimeZonePtrOrName&&, zoned_time<Duration, TimeZonePtr2>, choose = choose::earliest)
    -> zoned_time<common_type_t<Duration, seconds>,
                  tzdb_detail::time_zone_representation<TimeZonePtrOrName>>;

using zoned_seconds = zoned_time<seconds>;

template <class Duration1, class Duration2, class TimeZonePtr>
[[nodiscard]] bool operator==(const zoned_time<Duration1, TimeZonePtr>& x,
                              const zoned_time<Duration2, TimeZonePtr>& y) {
  return x.get_time_zone() == y.get_time_zone() && x.get_sys_time() == y.get_sys_time();
}

template <class CharT, class Traits, class Duration, class TimeZonePtr>
basic_ostream<CharT, Traits>&
operator<<(basic_ostream<CharT, Traits>& os, const zoned_time<Duration, TimeZonePtr>& t) {
  // Format the zoned_time directly: formatter<zoned_time> advertises a time zone
  // (so %Z is valid) and convert_to_tm/convert_to_time_zone supply the local
  // wall-clock fields and the abbreviation.
  return os << std::format(os.getloc(), CHRONO_SW(CharT, "{:L%F %T %Z}"), t);
}

} // namespace chrono

template <class Duration, class TimeZonePtr, class CharT>
struct formatter<chrono::zoned_time<Duration, TimeZonePtr>, CharT>
    : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    // chrono_flags::clock = date_time | time_zone, so %F %T %Z all validate.
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::clock);
  }
};

} // namespace std
