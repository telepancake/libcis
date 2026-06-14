// <bits/chrono_format.h> — chrono formatting + stream inserters.
// Ported from libc++ __chrono/{convert_to_tm,parser_std_format_spec,formatter,
// ostream,statically_widen}.h (release/20.x). Included by <chrono>.
//
// namespace std; chrono internals in std::detail; no underscore-prefixed names;
// exceptions OFF (throw_format_error traps). Delegates to <format>/<locale>.
#pragma once

#include <format>
#include <locale>
#include <sstream>
#include <ostream>
#include <stdexcept>
#include <string_view>
#include <ctime>
#include <cmath>
#include <limits>

namespace std {

// ---- STATICALLY-WIDEN ([time.general]/2) -----------------------------------
namespace detail {
template <class CharT>
constexpr const CharT* statically_widen(const char* s, const wchar_t* ws) {
  if constexpr (is_same_v<CharT, char>)
    return s;
  else
    return ws;
}
} // namespace detail
#define CHRONO_SW(CharT, str) ::std::detail::statically_widen<CharT>(str, L##str)

namespace chrono {
// Minimal sys_info / local_info (enough for the formatters/ostream inserters;
// the full time-zone database is not ported — see <chrono> banner).
struct sys_info {
  sys_seconds begin;
  sys_seconds end;
  seconds     offset;
  minutes     save;
  string      abbrev;
};

struct local_info {
  static constexpr int unique      = 0;
  static constexpr int nonexistent = 1;
  static constexpr int ambiguous   = 2;

  int      result;
  sys_info first;
  sys_info second;
};

class time_zone;  // defined in <bits/chrono_tzdb.h>
template <class Duration, class TimeZonePtr> class zoned_time;
} // namespace chrono

// ---------------------------------------------------------------------------
// convert_to_tm
// ---------------------------------------------------------------------------
namespace detail {

template <class Tp>
inline constexpr bool chrono_is_time_point_v = false;
template <class Clock, class Duration>
inline constexpr bool chrono_is_time_point_v<chrono::time_point<Clock, Duration>> = true;

// zoned_time is defined later (in <bits/chrono_tzdb.h>); forward-declare it so the
// convert_to_tm / convert_to_time_zone / formatter machinery can special-case it.
// The branches that touch its members are only instantiated from the
// formatter<zoned_time>, by which point the type is complete.
template <class Tp>
inline constexpr bool chrono_is_zoned_time_v = false;
template <class Duration, class TimeZonePtr>
inline constexpr bool chrono_is_zoned_time_v<chrono::zoned_time<Duration, TimeZonePtr>> = true;

template <class Tp>
inline constexpr bool chrono_is_hh_mm_ss_v = false;
template <class Duration>
inline constexpr bool chrono_is_hh_mm_ss_v<chrono::hh_mm_ss<Duration>> = true;

template <class Tp, template <class...> class Templ>
inline constexpr bool chrono_is_specialization_v = false;
template <template <class...> class Templ, class... Args>
inline constexpr bool chrono_is_specialization_v<Templ<Args...>, Templ> = true;

template <class Tm, class Date>
  requires(is_same_v<Date, chrono::year_month_day> || is_same_v<Date, chrono::year_month_day_last>)
Tm convert_to_tm(const Date& date, chrono::weekday weekday) {
  Tm result = {};
#ifdef __GLIBC__
  result.tm_zone = "UTC";
#endif
  result.tm_year = static_cast<int>(date.year()) - 1900;
  result.tm_mon  = static_cast<unsigned>(date.month()) - 1;
  result.tm_mday = static_cast<unsigned>(date.day());
  result.tm_wday = static_cast<unsigned>(weekday.c_encoding());
  result.tm_yday =
      (static_cast<chrono::sys_days>(date) -
       static_cast<chrono::sys_days>(chrono::year_month_day{date.year(), chrono::January, chrono::day{1}}))
          .count();
  return result;
}

template <class Tm, class Duration>
Tm convert_to_tm(const chrono::sys_time<Duration> tp) {
  chrono::sys_days days = chrono::floor<chrono::days>(tp);
  chrono::year_month_day ymd{days};

  Tm result = detail::convert_to_tm<Tm>(chrono::year_month_day{ymd}, chrono::weekday{days});

  uint64_t sec =
      chrono::duration_cast<chrono::seconds>(tp - chrono::time_point_cast<chrono::seconds>(days)).count();
  sec %= 24 * 3600;
  result.tm_hour = sec / 3600;
  sec %= 3600;
  result.tm_min = sec / 60;
  result.tm_sec = sec % 60;
  return result;
}

template <class Tm, class Duration>
Tm convert_to_tm(chrono::utc_time<Duration> tp) {
  Tm result = detail::convert_to_tm<Tm>(chrono::utc_clock::to_sys(tp));
  if (chrono::get_leap_second_info(tp).is_leap_second)
    ++result.tm_sec;
  return result;
}

template <class Tm, class ChronoT>
Tm convert_to_tm(const ChronoT& value) {
  Tm result = {};
#ifdef __GLIBC__
  result.tm_zone = "UTC";
#endif

  if constexpr (chrono_is_zoned_time_v<ChronoT>) {
    // Break the local wall-clock instant (sys + offset) into calendar fields:
    // feed it through the sys_time path, which decomposes a count of seconds.
    chrono::sys_info info = value.get_time_zone()->get_info(value.get_sys_time());
    auto local = value.get_sys_time() + info.offset;
    return detail::convert_to_tm<Tm>(
        chrono::sys_time<typename decltype(local)::duration>{local.time_since_epoch()});
  } else if constexpr (chrono_is_time_point_v<ChronoT>) {
    if constexpr (is_same_v<typename ChronoT::clock, chrono::system_clock>)
      return detail::convert_to_tm<Tm>(value);
    else if constexpr (is_same_v<typename ChronoT::clock, chrono::utc_clock>)
      return detail::convert_to_tm<Tm>(value);
    else if constexpr (is_same_v<typename ChronoT::clock, chrono::file_clock>)
      return detail::convert_to_tm<Tm>(ChronoT::clock::to_sys(value));
    else if constexpr (is_same_v<typename ChronoT::clock, chrono::local_t>)
      return detail::convert_to_tm<Tm>(chrono::sys_time<typename ChronoT::duration>{value.time_since_epoch()});
    else
      static_assert(sizeof(ChronoT) == 0, "TODO: Add the missing clock specialization");
  } else if constexpr (chrono::is_duration_v<ChronoT>) {
    if constexpr (is_convertible_v<ChronoT, chrono::hours>) {
      auto hour      = chrono::floor<chrono::hours>(value);
      auto sec       = chrono::duration_cast<chrono::seconds>(value - hour);
      result.tm_hour = hour.count() % 24;
      result.tm_min  = sec.count() / 60;
      result.tm_sec  = sec.count() % 60;
    } else {
      uint64_t sec = chrono::duration_cast<chrono::seconds>(value).count();
      sec %= 24 * 3600;
      result.tm_hour = sec / 3600;
      sec %= 3600;
      result.tm_min = sec / 60;
      result.tm_sec = sec % 60;
    }
  } else if constexpr (is_same_v<ChronoT, chrono::day>)
    result.tm_mday = static_cast<unsigned>(value);
  else if constexpr (is_same_v<ChronoT, chrono::month>)
    result.tm_mon = static_cast<unsigned>(value) - 1;
  else if constexpr (is_same_v<ChronoT, chrono::year>)
    result.tm_year = static_cast<int>(value) - 1900;
  else if constexpr (is_same_v<ChronoT, chrono::weekday>)
    result.tm_wday = value.c_encoding();
  else if constexpr (is_same_v<ChronoT, chrono::weekday_indexed> || is_same_v<ChronoT, chrono::weekday_last>)
    result.tm_wday = value.weekday().c_encoding();
  else if constexpr (is_same_v<ChronoT, chrono::month_day>) {
    result.tm_mday = static_cast<unsigned>(value.day());
    result.tm_mon  = static_cast<unsigned>(value.month()) - 1;
  } else if constexpr (is_same_v<ChronoT, chrono::month_day_last>) {
    result.tm_mon = static_cast<unsigned>(value.month()) - 1;
  } else if constexpr (is_same_v<ChronoT, chrono::month_weekday> || is_same_v<ChronoT, chrono::month_weekday_last>) {
    result.tm_wday = value.weekday_indexed().weekday().c_encoding();
    result.tm_mon  = static_cast<unsigned>(value.month()) - 1;
  } else if constexpr (is_same_v<ChronoT, chrono::year_month>) {
    result.tm_year = static_cast<int>(value.year()) - 1900;
    result.tm_mon  = static_cast<unsigned>(value.month()) - 1;
  } else if constexpr (is_same_v<ChronoT, chrono::year_month_day> || is_same_v<ChronoT, chrono::year_month_day_last>) {
    return detail::convert_to_tm<Tm>(chrono::year_month_day{value},
                                     chrono::weekday{static_cast<chrono::sys_days>(value)});
  } else if constexpr (is_same_v<ChronoT, chrono::year_month_weekday> ||
                       is_same_v<ChronoT, chrono::year_month_weekday_last>) {
    return detail::convert_to_tm<Tm>(chrono::year_month_day{static_cast<chrono::sys_days>(value)}, value.weekday());
  } else if constexpr (chrono_is_hh_mm_ss_v<ChronoT>) {
    result.tm_sec = value.seconds().count();
    result.tm_min = value.minutes().count();
    if constexpr (sizeof(chrono::hours::rep) > sizeof(result.tm_hour))
      if (value.hours().count() > std::numeric_limits<decltype(result.tm_hour)>::max())
        std::throw_format_error("Formatting hh_mm_ss, encountered an hour overflow");
    result.tm_hour = value.hours().count();
  } else if constexpr (is_same_v<ChronoT, chrono::sys_info>) {
    // no time information
  } else if constexpr (is_same_v<ChronoT, chrono::local_info>) {
    // no time information
  } else
    static_assert(sizeof(ChronoT) == 0, "Add the missing type specialization");

  return result;
}

} // namespace detail

// ---------------------------------------------------------------------------
// chrono format-spec parser (ported from __chrono/parser_std_format_spec.h)
// ---------------------------------------------------------------------------
namespace format_spec {

inline constexpr fields fields_chrono_fractional{.precision_ = true, .locale_specific_form_ = true, .type_ = false};
inline constexpr fields fields_chrono{.locale_specific_form_ = true, .type_ = false};

enum class chrono_flags {
  second = 0x1,
  minute = 0x2,
  hour   = 0x4,
  time   = hour | minute | second,

  day   = 0x8,
  month = 0x10,
  year  = 0x20,

  weekday = 0x40,

  month_day     = day | month,
  month_weekday = weekday | month,
  year_month    = month | year,
  date          = day | month | year | weekday,

  date_time = date | time,

  duration = 0x80 | time,

  time_zone = 0x100,

  clock = date_time | time_zone
};

constexpr chrono_flags operator&(chrono_flags lhs, chrono_flags rhs) {
  return static_cast<chrono_flags>(static_cast<unsigned>(lhs) & static_cast<unsigned>(rhs));
}

constexpr void validate_second(chrono_flags f) {
  if ((f & chrono_flags::second) != chrono_flags::second)
    std::throw_format_error("The supplied date time doesn't contain a second");
}
constexpr void validate_minute(chrono_flags f) {
  if ((f & chrono_flags::minute) != chrono_flags::minute)
    std::throw_format_error("The supplied date time doesn't contain a minute");
}
constexpr void validate_hour(chrono_flags f) {
  if ((f & chrono_flags::hour) != chrono_flags::hour)
    std::throw_format_error("The supplied date time doesn't contain an hour");
}
constexpr void validate_time(chrono_flags f) {
  if ((f & chrono_flags::time) != chrono_flags::time)
    std::throw_format_error("The supplied date time doesn't contain a time");
}
constexpr void validate_day(chrono_flags f) {
  if ((f & chrono_flags::day) != chrono_flags::day)
    std::throw_format_error("The supplied date time doesn't contain a day");
}
constexpr void validate_month(chrono_flags f) {
  if ((f & chrono_flags::month) != chrono_flags::month)
    std::throw_format_error("The supplied date time doesn't contain a month");
}
constexpr void validate_year(chrono_flags f) {
  if ((f & chrono_flags::year) != chrono_flags::year)
    std::throw_format_error("The supplied date time doesn't contain a year");
}
constexpr void validate_date(chrono_flags f) {
  if ((f & chrono_flags::date) != chrono_flags::date)
    std::throw_format_error("The supplied date time doesn't contain a date");
}
constexpr void validate_date_or_duration(chrono_flags f) {
  if (((f & chrono_flags::date) != chrono_flags::date) && ((f & chrono_flags::duration) != chrono_flags::duration))
    std::throw_format_error("The supplied date time doesn't contain a date or duration");
}
constexpr void validate_date_time(chrono_flags f) {
  if ((f & chrono_flags::date_time) != chrono_flags::date_time)
    std::throw_format_error("The supplied date time doesn't contain a date and time");
}
constexpr void validate_weekday(chrono_flags f) {
  if ((f & chrono_flags::weekday) != chrono_flags::weekday)
    std::throw_format_error("The supplied date time doesn't contain a weekday");
}
constexpr void validate_duration(chrono_flags f) {
  if ((f & chrono_flags::duration) != chrono_flags::duration)
    std::throw_format_error("The supplied date time doesn't contain a duration");
}
constexpr void validate_time_zone(chrono_flags f) {
  if ((f & chrono_flags::time_zone) != chrono_flags::time_zone)
    std::throw_format_error("The supplied date time doesn't contain a time zone");
}

template <class CharT>
class parser_chrono {
  using ConstIterator = typename basic_format_parse_context<CharT>::const_iterator;

public:
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx, fields fields, chrono_flags flags) {
    ConstIterator begin = parser_.parse(ctx, fields);
    ConstIterator end   = ctx.end();
    if (begin == end)
      return begin;

    ConstIterator last = parse_chrono_specs(begin, end, flags);
    chrono_specs_      = basic_string_view<CharT>{begin, last};
    return last;
  }

  parser<CharT> parser_;
  basic_string_view<CharT> chrono_specs_;

private:
  constexpr ConstIterator parse_chrono_specs(ConstIterator begin, ConstIterator end, chrono_flags flags) {
    if (*begin != CharT('%') && *begin != CharT('}'))
      std::throw_format_error("The format specifier expects a '%' or a '}'");

    do {
      switch (*begin) {
      case CharT('{'):
        std::throw_format_error("The chrono specifiers contain a '{'");
      case CharT('}'):
        return begin;
      case CharT('%'):
        parse_conversion_spec(begin, end, flags);
        [[fallthrough]];
      default:
        ++begin;
      }
    } while (begin != end && *begin != CharT('}'));

    return begin;
  }

  constexpr void parse_conversion_spec(ConstIterator& begin, ConstIterator end, chrono_flags flags) {
    ++begin;
    if (begin == end)
      std::throw_format_error("End of input while parsing a conversion specifier");

    switch (*begin) {
    case CharT('n'):
    case CharT('t'):
    case CharT('%'):
      break;
    case CharT('S'):
      validate_second(flags);
      break;
    case CharT('M'):
      validate_minute(flags);
      break;
    case CharT('p'):
    case CharT('H'):
    case CharT('I'):
      parser_.hour_ = true;
      validate_hour(flags);
      break;
    case CharT('r'):
    case CharT('R'):
    case CharT('T'):
    case CharT('X'):
      parser_.hour_ = true;
      validate_time(flags);
      break;
    case CharT('d'):
    case CharT('e'):
      validate_day(flags);
      break;
    case CharT('b'):
    case CharT('h'):
    case CharT('B'):
      parser_.month_name_ = true;
      [[fallthrough]];
    case CharT('m'):
      validate_month(flags);
      break;
    case CharT('y'):
    case CharT('C'):
    case CharT('Y'):
      validate_year(flags);
      break;
    case CharT('j'):
      parser_.day_of_year_ = true;
      validate_date_or_duration(flags);
      break;
    case CharT('g'):
    case CharT('G'):
    case CharT('U'):
    case CharT('V'):
    case CharT('W'):
      parser_.week_of_year_ = true;
      [[fallthrough]];
    case CharT('x'):
    case CharT('D'):
    case CharT('F'):
      validate_date(flags);
      break;
    case CharT('c'):
      validate_date_time(flags);
      break;
    case CharT('a'):
    case CharT('A'):
      parser_.weekday_name_ = true;
      [[fallthrough]];
    case CharT('u'):
    case CharT('w'):
      parser_.weekday_ = true;
      validate_weekday(flags);
      break;
    case CharT('q'):
    case CharT('Q'):
      validate_duration(flags);
      break;
    case CharT('E'):
      parse_modifier_E(begin, end, flags);
      break;
    case CharT('O'):
      parse_modifier_O(begin, end, flags);
      break;
    case CharT('z'):
    case CharT('Z'):
      validate_time_zone(flags);
      break;
    default:
      std::throw_format_error("The date time type specifier is invalid");
    }
  }

  constexpr void parse_modifier_E(ConstIterator& begin, ConstIterator end, chrono_flags flags) {
    ++begin;
    if (begin == end)
      std::throw_format_error("End of input while parsing the modifier E");
    switch (*begin) {
    case CharT('X'):
      parser_.hour_ = true;
      validate_time(flags);
      break;
    case CharT('y'):
    case CharT('C'):
    case CharT('Y'):
      validate_year(flags);
      break;
    case CharT('x'):
      validate_date(flags);
      break;
    case CharT('c'):
      validate_date_time(flags);
      break;
    case CharT('z'):
      validate_time_zone(flags);
      break;
    default:
      std::throw_format_error("The date time type specifier for modifier E is invalid");
    }
  }

  constexpr void parse_modifier_O(ConstIterator& begin, ConstIterator end, chrono_flags flags) {
    ++begin;
    if (begin == end)
      std::throw_format_error("End of input while parsing the modifier O");
    switch (*begin) {
    case CharT('S'):
      validate_second(flags);
      break;
    case CharT('M'):
      validate_minute(flags);
      break;
    case CharT('I'):
    case CharT('H'):
      parser_.hour_ = true;
      validate_hour(flags);
      break;
    case CharT('d'):
    case CharT('e'):
      validate_day(flags);
      break;
    case CharT('m'):
      validate_month(flags);
      break;
    case CharT('y'):
      validate_year(flags);
      break;
    case CharT('U'):
    case CharT('V'):
    case CharT('W'):
      parser_.week_of_year_ = true;
      validate_date(flags);
      break;
    case CharT('u'):
    case CharT('w'):
      parser_.weekday_ = true;
      validate_weekday(flags);
      break;
    case CharT('z'):
      validate_time_zone(flags);
      break;
    default:
      std::throw_format_error("The date time type specifier for modifier O is invalid");
    }
  }
};

} // namespace format_spec

// ---------------------------------------------------------------------------
// units_suffix (used by duration ostream + %q)
// ---------------------------------------------------------------------------
namespace chrono {

template <class CharT, class Period>
auto units_suffix() {
  if constexpr (is_same_v<typename Period::type, atto>)
    return CHRONO_SW(CharT, "as");
  else if constexpr (is_same_v<typename Period::type, femto>)
    return CHRONO_SW(CharT, "fs");
  else if constexpr (is_same_v<typename Period::type, pico>)
    return CHRONO_SW(CharT, "ps");
  else if constexpr (is_same_v<typename Period::type, nano>)
    return CHRONO_SW(CharT, "ns");
  else if constexpr (is_same_v<typename Period::type, micro>)
    return CHRONO_SW(CharT, "µs");
  else if constexpr (is_same_v<typename Period::type, milli>)
    return CHRONO_SW(CharT, "ms");
  else if constexpr (is_same_v<typename Period::type, centi>)
    return CHRONO_SW(CharT, "cs");
  else if constexpr (is_same_v<typename Period::type, deci>)
    return CHRONO_SW(CharT, "ds");
  else if constexpr (is_same_v<typename Period::type, ratio<1>>)
    return CHRONO_SW(CharT, "s");
  else if constexpr (is_same_v<typename Period::type, deca>)
    return CHRONO_SW(CharT, "das");
  else if constexpr (is_same_v<typename Period::type, hecto>)
    return CHRONO_SW(CharT, "hs");
  else if constexpr (is_same_v<typename Period::type, kilo>)
    return CHRONO_SW(CharT, "ks");
  else if constexpr (is_same_v<typename Period::type, mega>)
    return CHRONO_SW(CharT, "Ms");
  else if constexpr (is_same_v<typename Period::type, giga>)
    return CHRONO_SW(CharT, "Gs");
  else if constexpr (is_same_v<typename Period::type, tera>)
    return CHRONO_SW(CharT, "Ts");
  else if constexpr (is_same_v<typename Period::type, peta>)
    return CHRONO_SW(CharT, "Ps");
  else if constexpr (is_same_v<typename Period::type, exa>)
    return CHRONO_SW(CharT, "Es");
  else if constexpr (is_same_v<typename Period::type, ratio<60>>)
    return CHRONO_SW(CharT, "min");
  else if constexpr (is_same_v<typename Period::type, ratio<3600>>)
    return CHRONO_SW(CharT, "h");
  else if constexpr (is_same_v<typename Period::type, ratio<86400>>)
    return CHRONO_SW(CharT, "d");
  else if constexpr (Period::den == 1)
    return std::format(CHRONO_SW(CharT, "[{}]s"), Period::num);
  else
    return std::format(CHRONO_SW(CharT, "[{}/{}]s"), Period::num, Period::den);
}

} // namespace chrono

// ---------------------------------------------------------------------------
// formatter implementation (ported from __chrono/formatter.h)
// ---------------------------------------------------------------------------
namespace detail {

template <class CharT, class Rep, class Period>
void format_sub_seconds(basic_stringstream<CharT>& sstr, const chrono::duration<Rep, Period>& value) {
  sstr << std::use_facet<numpunct<CharT>>(sstr.getloc()).decimal_point();
  using Dur = chrono::duration<Rep, Period>;
  auto fraction = value - chrono::duration_cast<chrono::seconds>(value);
  if (value < chrono::seconds{0} && fraction != Dur{0})
    fraction += chrono::seconds{1};
  if constexpr (chrono::treat_as_floating_point_v<Rep>)
    std::format_to(std::ostreambuf_iterator<CharT>{sstr},
                   CHRONO_SW(CharT, "{:0{}.0f}"),
                   chrono::duration_cast<typename chrono::hh_mm_ss<Dur>::precision>(fraction).count(),
                   chrono::hh_mm_ss<Dur>::fractional_width);
  else
    std::format_to(std::ostreambuf_iterator<CharT>{sstr},
                   CHRONO_SW(CharT, "{:0{}}"),
                   chrono::duration_cast<typename chrono::hh_mm_ss<Dur>::precision>(fraction).count(),
                   chrono::hh_mm_ss<Dur>::fractional_width);
}

template <class CharT, class Tp>
  requires chrono_is_time_point_v<Tp>
void format_sub_seconds(basic_stringstream<CharT>& sstr, const Tp& value) {
  detail::format_sub_seconds(sstr, value.time_since_epoch());
}

template <class CharT, class Tp>
  requires chrono_is_zoned_time_v<Tp>
void format_sub_seconds(basic_stringstream<CharT>& sstr, const Tp& value) {
  // The sub-second part is unaffected by the (whole-second) zone offset.
  detail::format_sub_seconds(sstr, value.get_sys_time().time_since_epoch());
}

template <class CharT, class Duration>
void format_sub_seconds(basic_stringstream<CharT>& sstr, const chrono::hh_mm_ss<Duration>& value) {
  sstr << std::use_facet<numpunct<CharT>>(sstr.getloc()).decimal_point();
  if constexpr (chrono::treat_as_floating_point_v<typename Duration::rep>)
    std::format_to(std::ostreambuf_iterator<CharT>{sstr},
                   CHRONO_SW(CharT, "{:0{}.0f}"), value.subseconds().count(), value.fractional_width);
  else
    std::format_to(std::ostreambuf_iterator<CharT>{sstr},
                   CHRONO_SW(CharT, "{:0{}}"), value.subseconds().count(), value.fractional_width);
}

template <class Tp>
consteval bool use_fraction() {
  if constexpr (chrono_is_time_point_v<Tp>)
    return chrono::hh_mm_ss<typename Tp::duration>::fractional_width;
  else if constexpr (chrono_is_zoned_time_v<Tp>)
    return chrono::hh_mm_ss<typename Tp::duration>::fractional_width;
  else if constexpr (chrono::is_duration_v<Tp>)
    return chrono::hh_mm_ss<Tp>::fractional_width;
  else if constexpr (chrono_is_hh_mm_ss_v<Tp>)
    return Tp::fractional_width;
  else
    return false;
}

template <class CharT>
void format_year(basic_stringstream<CharT>& sstr, int year) {
  if (year < 0) {
    sstr << CharT('-');
    year = -year;
  }
  sstr << std::format(CHRONO_SW(CharT, "{:04}"), year);
}

template <class CharT>
void format_century(basic_stringstream<CharT>& sstr, int year) {
  bool negative = year < 0;
  int century   = (year - (99 * negative)) / 100;
  sstr << std::format(CHRONO_SW(CharT, "{:02}"), century);
}

template <class CharT>
void format_zone_offset(basic_stringstream<CharT>& sstr, chrono::seconds offset, bool modifier) {
  if (offset < chrono::seconds{0}) {
    sstr << CharT('-');
    offset = -offset;
  } else {
    sstr << CharT('+');
  }
  chrono::hh_mm_ss hms{offset};
  std::ostreambuf_iterator<CharT> out_it{sstr};
  std::format_to(out_it, CHRONO_SW(CharT, "{:02}"), hms.hours().count());
  if (modifier)
    sstr << CharT(':');
  std::format_to(out_it, CHRONO_SW(CharT, "{:02}"), hms.minutes().count());
}

struct chrono_time_zone {
  string abbrev;
  chrono::seconds offset;
};

template <class Tp>
chrono_time_zone convert_to_time_zone(const Tp& value) {
  if constexpr (is_same_v<Tp, chrono::sys_info>)
    return {value.abbrev, value.offset};
  else if constexpr (chrono_is_zoned_time_v<Tp>) {
    chrono::sys_info info = value.get_time_zone()->get_info(value.get_sys_time());
    return {info.abbrev, info.offset};
  } else
    return {"UTC", chrono::seconds{0}};
}

template <class CharT, class Tp>
void format_chrono_using_chrono_specs(
    basic_stringstream<CharT>& sstr, const Tp& value, basic_string_view<CharT> chrono_specs) {
  tm t                = detail::convert_to_tm<tm>(value);
  chrono_time_zone z  = detail::convert_to_time_zone(value);
  const auto& facet   = std::use_facet<time_put<CharT>>(sstr.getloc());
  for (auto it = chrono_specs.begin(); it != chrono_specs.end(); ++it) {
    if (*it == CharT('%')) {
      auto s = it;
      ++it;
      switch (*it) {
      case CharT('n'):
        sstr << CharT('\n');
        break;
      case CharT('t'):
        sstr << CharT('\t');
        break;
      case CharT('%'):
        sstr << *it;
        break;
      case CharT('C'): {
        int year = t.tm_year + 1900;
        if (year < 1000 || year > 9999)
          detail::format_century(sstr, year);
        else
          facet.put({sstr}, sstr, CharT(' '), std::addressof(t), std::to_address(s), std::to_address(it + 1));
      } break;
      case CharT('j'):
        if constexpr (chrono::is_duration_v<Tp>)
          sstr << chrono::duration_cast<chrono::days>(chrono::duration_cast<chrono::seconds>(value)).count();
        else
          facet.put({sstr}, sstr, CharT(' '), std::addressof(t), std::to_address(s), std::to_address(it + 1));
        break;
      case CharT('q'):
        if constexpr (chrono::is_duration_v<Tp>) {
          sstr << chrono::units_suffix<CharT, typename Tp::period>();
          break;
        }
        __builtin_unreachable();
      case CharT('Q'):
        if constexpr (chrono::is_duration_v<Tp>) {
          sstr << std::format(CHRONO_SW(CharT, "{}"), value.count());
          break;
        }
        __builtin_unreachable();
      case CharT('S'):
      case CharT('T'):
        facet.put({sstr}, sstr, CharT(' '), std::addressof(t), std::to_address(s), std::to_address(it + 1));
        if constexpr (use_fraction<Tp>())
          detail::format_sub_seconds(sstr, value);
        break;
#if defined(__GLIBC__)
      case CharT('y'):
        sstr << std::format(CHRONO_SW(CharT, "{:02}"), (std::abs(t.tm_year + 1900)) % 100);
        break;
#endif
      case CharT('Y'):
        detail::format_year(sstr, t.tm_year + 1900);
        break;
      case CharT('F'):
        detail::format_year(sstr, t.tm_year + 1900);
        sstr << std::format(CHRONO_SW(CharT, "-{:02}-{:02}"), t.tm_mon + 1, t.tm_mday);
        break;
      case CharT('z'):
        detail::format_zone_offset(sstr, z.offset, false);
        break;
      case CharT('Z'):
        std::ranges::copy(z.abbrev, std::ostreambuf_iterator<CharT>{sstr});
        break;
      case CharT('O'):
        if constexpr (use_fraction<Tp>()) {
          if (*(it + 1) == CharT('S')) {
            ++it;
            facet.put({sstr}, sstr, CharT(' '), std::addressof(t), std::to_address(s), std::to_address(it + 1));
            detail::format_sub_seconds(sstr, value);
            break;
          }
        }
        [[fallthrough]];
      case CharT('E'):
        ++it;
        if (*it == CharT('z')) {
          detail::format_zone_offset(sstr, z.offset, true);
          break;
        }
        [[fallthrough]];
      default:
        facet.put({sstr}, sstr, CharT(' '), std::addressof(t), std::to_address(s), std::to_address(it + 1));
        break;
      }
    } else {
      sstr << *it;
    }
  }
}

template <class Tp>
constexpr bool weekday_ok(const Tp& value) {
  if constexpr (chrono_is_time_point_v<Tp>) return true;
  else if constexpr (is_same_v<Tp, chrono::day>) return true;
  else if constexpr (is_same_v<Tp, chrono::month>) return value.ok();
  else if constexpr (is_same_v<Tp, chrono::year>) return true;
  else if constexpr (is_same_v<Tp, chrono::weekday>) return true;
  else if constexpr (is_same_v<Tp, chrono::weekday_indexed>) return true;
  else if constexpr (is_same_v<Tp, chrono::weekday_last>) return true;
  else if constexpr (is_same_v<Tp, chrono::month_day>) return true;
  else if constexpr (is_same_v<Tp, chrono::month_day_last>) return true;
  else if constexpr (is_same_v<Tp, chrono::month_weekday>) return true;
  else if constexpr (is_same_v<Tp, chrono::month_weekday_last>) return true;
  else if constexpr (is_same_v<Tp, chrono::year_month>) return true;
  else if constexpr (is_same_v<Tp, chrono::year_month_day>) return value.ok();
  else if constexpr (is_same_v<Tp, chrono::year_month_day_last>) return value.ok();
  else if constexpr (is_same_v<Tp, chrono::year_month_weekday>) return value.weekday().ok();
  else if constexpr (is_same_v<Tp, chrono::year_month_weekday_last>) return value.weekday().ok();
  else if constexpr (chrono_is_hh_mm_ss_v<Tp>) return true;
  else if constexpr (is_same_v<Tp, chrono::sys_info>) return true;
  else if constexpr (is_same_v<Tp, chrono::local_info>) return true;
  else if constexpr (chrono_is_zoned_time_v<Tp>) return true;
  else static_assert(sizeof(Tp) == 0, "Add the missing type specialization");
}

template <class Tp>
constexpr bool weekday_name_ok(const Tp& value) {
  if constexpr (chrono_is_time_point_v<Tp>) return true;
  else if constexpr (is_same_v<Tp, chrono::day>) return true;
  else if constexpr (is_same_v<Tp, chrono::month>) return value.ok();
  else if constexpr (is_same_v<Tp, chrono::year>) return true;
  else if constexpr (is_same_v<Tp, chrono::weekday>) return value.ok();
  else if constexpr (is_same_v<Tp, chrono::weekday_indexed>) return value.weekday().ok();
  else if constexpr (is_same_v<Tp, chrono::weekday_last>) return value.weekday().ok();
  else if constexpr (is_same_v<Tp, chrono::month_day>) return true;
  else if constexpr (is_same_v<Tp, chrono::month_day_last>) return true;
  else if constexpr (is_same_v<Tp, chrono::month_weekday>) return value.weekday_indexed().ok();
  else if constexpr (is_same_v<Tp, chrono::month_weekday_last>) return value.weekday_indexed().ok();
  else if constexpr (is_same_v<Tp, chrono::year_month>) return true;
  else if constexpr (is_same_v<Tp, chrono::year_month_day>) return value.ok();
  else if constexpr (is_same_v<Tp, chrono::year_month_day_last>) return value.ok();
  else if constexpr (is_same_v<Tp, chrono::year_month_weekday>) return value.weekday().ok();
  else if constexpr (is_same_v<Tp, chrono::year_month_weekday_last>) return value.weekday().ok();
  else if constexpr (chrono_is_hh_mm_ss_v<Tp>) return true;
  else if constexpr (is_same_v<Tp, chrono::sys_info>) return true;
  else if constexpr (is_same_v<Tp, chrono::local_info>) return true;
  else if constexpr (chrono_is_zoned_time_v<Tp>) return true;
  else static_assert(sizeof(Tp) == 0, "Add the missing type specialization");
}

template <class Tp>
constexpr bool date_ok(const Tp& value) {
  if constexpr (chrono_is_time_point_v<Tp>) return true;
  else if constexpr (is_same_v<Tp, chrono::day>) return true;
  else if constexpr (is_same_v<Tp, chrono::month>) return value.ok();
  else if constexpr (is_same_v<Tp, chrono::year>) return true;
  else if constexpr (is_same_v<Tp, chrono::weekday>) return true;
  else if constexpr (is_same_v<Tp, chrono::weekday_indexed>) return true;
  else if constexpr (is_same_v<Tp, chrono::weekday_last>) return true;
  else if constexpr (is_same_v<Tp, chrono::month_day>) return true;
  else if constexpr (is_same_v<Tp, chrono::month_day_last>) return true;
  else if constexpr (is_same_v<Tp, chrono::month_weekday>) return true;
  else if constexpr (is_same_v<Tp, chrono::month_weekday_last>) return true;
  else if constexpr (is_same_v<Tp, chrono::year_month>) return true;
  else if constexpr (is_same_v<Tp, chrono::year_month_day>) return value.ok();
  else if constexpr (is_same_v<Tp, chrono::year_month_day_last>) return value.ok();
  else if constexpr (is_same_v<Tp, chrono::year_month_weekday>) return value.ok();
  else if constexpr (is_same_v<Tp, chrono::year_month_weekday_last>) return value.ok();
  else if constexpr (chrono_is_hh_mm_ss_v<Tp>) return true;
  else if constexpr (is_same_v<Tp, chrono::sys_info>) return true;
  else if constexpr (is_same_v<Tp, chrono::local_info>) return true;
  else if constexpr (chrono_is_zoned_time_v<Tp>) return true;
  else static_assert(sizeof(Tp) == 0, "Add the missing type specialization");
}

template <class Tp>
constexpr bool month_name_ok(const Tp& value) {
  if constexpr (chrono_is_time_point_v<Tp>) return true;
  else if constexpr (is_same_v<Tp, chrono::day>) return true;
  else if constexpr (is_same_v<Tp, chrono::month>) return value.ok();
  else if constexpr (is_same_v<Tp, chrono::year>) return true;
  else if constexpr (is_same_v<Tp, chrono::weekday>) return true;
  else if constexpr (is_same_v<Tp, chrono::weekday_indexed>) return true;
  else if constexpr (is_same_v<Tp, chrono::weekday_last>) return true;
  else if constexpr (is_same_v<Tp, chrono::month_day>) return value.month().ok();
  else if constexpr (is_same_v<Tp, chrono::month_day_last>) return value.month().ok();
  else if constexpr (is_same_v<Tp, chrono::month_weekday>) return value.month().ok();
  else if constexpr (is_same_v<Tp, chrono::month_weekday_last>) return value.month().ok();
  else if constexpr (is_same_v<Tp, chrono::year_month>) return value.month().ok();
  else if constexpr (is_same_v<Tp, chrono::year_month_day>) return value.month().ok();
  else if constexpr (is_same_v<Tp, chrono::year_month_day_last>) return value.month().ok();
  else if constexpr (is_same_v<Tp, chrono::year_month_weekday>) return value.month().ok();
  else if constexpr (is_same_v<Tp, chrono::year_month_weekday_last>) return value.month().ok();
  else if constexpr (chrono_is_hh_mm_ss_v<Tp>) return true;
  else if constexpr (is_same_v<Tp, chrono::sys_info>) return true;
  else if constexpr (is_same_v<Tp, chrono::local_info>) return true;
  else if constexpr (chrono_is_zoned_time_v<Tp>) return true;
  else static_assert(sizeof(Tp) == 0, "Add the missing type specialization");
}

template <class CharT, class Tp, class FormatContext>
auto format_chrono(const Tp& value,
                   FormatContext& ctx,
                   format_spec::parsed_specifications<CharT> specs,
                   basic_string_view<CharT> chrono_specs) {
  basic_stringstream<CharT> sstr;
  if (specs.chrono_.locale_specific_form_)
    sstr.imbue(ctx.locale());
  else
    sstr.imbue(locale::classic());

  if (chrono_specs.empty())
    sstr << value;
  else {
    if constexpr (chrono::is_duration_v<Tp>) {
      if constexpr (numeric_limits<typename Tp::rep>::is_signed) {
        if (value < value.zero()) {
          sstr << CharT('-');
          detail::format_chrono_using_chrono_specs(sstr, -value, chrono_specs);
        } else
          detail::format_chrono_using_chrono_specs(sstr, value, chrono_specs);
      } else
        detail::format_chrono_using_chrono_specs(sstr, value, chrono_specs);
      specs.precision_ = -1;
    } else {
      if (specs.chrono_.weekday_name_ && !detail::weekday_name_ok(value))
        std::throw_format_error("Formatting a weekday name needs a valid weekday");
      if (specs.chrono_.weekday_ && !detail::weekday_ok(value))
        std::throw_format_error("Formatting a weekday needs a valid weekday");
      if (specs.chrono_.day_of_year_ && !detail::date_ok(value))
        std::throw_format_error("Formatting a day of year needs a valid date");
      if (specs.chrono_.week_of_year_ && !detail::date_ok(value))
        std::throw_format_error("Formatting a week of year needs a valid date");
      if (specs.chrono_.month_name_ && !detail::month_name_ok(value))
        std::throw_format_error("Formatting a month name from an invalid month number");

      if constexpr (chrono_is_hh_mm_ss_v<Tp>) {
        if (specs.chrono_.hour_ && value.hours().count() > 23)
          std::throw_format_error("Formatting a hour needs a valid value");
        if (value.is_negative())
          sstr << CharT('-');
      }

      detail::format_chrono_using_chrono_specs(sstr, value, chrono_specs);
    }
  }

  return fmtops::write_string(basic_string_view<CharT>{sstr.str()}, ctx.out(), specs);
}

template <class CharT>
struct formatter_chrono {
public:
  template <class ParseContext>
  constexpr typename ParseContext::iterator
  parse_impl(ParseContext& ctx, format_spec::fields fields, format_spec::chrono_flags flags) {
    return parser_.parse(ctx, fields, flags);
  }

  template <class Tp, class FormatContext>
  typename FormatContext::iterator format(const Tp& value, FormatContext& ctx) const {
    return detail::format_chrono(
        value, ctx, parser_.parser_.get_parsed_chrono_specifications(ctx), parser_.chrono_specs_);
  }

  format_spec::parser_chrono<CharT> parser_;
};

} // namespace detail

// ---------------------------------------------------------------------------
// formatter specializations
// ---------------------------------------------------------------------------
template <class Duration, class CharT>
struct formatter<chrono::sys_time<Duration>, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::clock);
  }
};

template <class Duration, class CharT>
struct formatter<chrono::utc_time<Duration>, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::clock);
  }
};

template <class Duration, class CharT>
struct formatter<chrono::file_time<Duration>, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::clock);
  }
};

template <class Duration, class CharT>
struct formatter<chrono::local_time<Duration>, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::date_time);
  }
};

template <class Rep, class Period, class CharT>
struct formatter<chrono::duration<Rep, Period>, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    if constexpr (std::is_floating_point_v<Rep>)
      return Base::parse_impl(ctx, format_spec::fields_chrono_fractional, format_spec::chrono_flags::duration);
    else
      return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::duration);
  }
};

template <class CharT>
struct formatter<chrono::day, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::day);
  }
};

template <class CharT>
struct formatter<chrono::month, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::month);
  }
};

template <class CharT>
struct formatter<chrono::year, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::year);
  }
};

template <class CharT>
struct formatter<chrono::weekday, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::weekday);
  }
};

template <class CharT>
struct formatter<chrono::weekday_indexed, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::weekday);
  }
};

template <class CharT>
struct formatter<chrono::weekday_last, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::weekday);
  }
};

template <class CharT>
struct formatter<chrono::month_day, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::month_day);
  }
};

template <class CharT>
struct formatter<chrono::month_day_last, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::month);
  }
};

template <class CharT>
struct formatter<chrono::month_weekday, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::month_weekday);
  }
};

template <class CharT>
struct formatter<chrono::month_weekday_last, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::month_weekday);
  }
};

template <class CharT>
struct formatter<chrono::year_month, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::year_month);
  }
};

template <class CharT>
struct formatter<chrono::year_month_day, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::date);
  }
};

template <class CharT>
struct formatter<chrono::year_month_day_last, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::date);
  }
};

template <class CharT>
struct formatter<chrono::year_month_weekday, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::date);
  }
};

template <class CharT>
struct formatter<chrono::year_month_weekday_last, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::date);
  }
};

template <class Duration, class CharT>
struct formatter<chrono::hh_mm_ss<Duration>, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::time);
  }
};

template <class CharT>
struct formatter<chrono::sys_info, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags::time_zone);
  }
};

template <class CharT>
struct formatter<chrono::local_info, CharT> : public detail::formatter_chrono<CharT> {
  using Base = detail::formatter_chrono<CharT>;
  template <class ParseContext>
  constexpr typename ParseContext::iterator parse(ParseContext& ctx) {
    return Base::parse_impl(ctx, format_spec::fields_chrono, format_spec::chrono_flags{});
  }
};

// ---------------------------------------------------------------------------
// stream inserters (ported from __chrono/ostream.h)
// ---------------------------------------------------------------------------
namespace chrono {

template <class CharT, class Traits, class Duration>
  requires(!treat_as_floating_point_v<typename Duration::rep> && Duration{1} < days{1})
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const sys_time<Duration>& tp) {
  return os << std::format(os.getloc(), CHRONO_SW(CharT, "{:L%F %T}"), tp);
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const sys_days& dp) {
  return os << year_month_day{dp};
}

template <class CharT, class Traits, class Duration>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const utc_time<Duration>& tp) {
  return os << std::format(os.getloc(), CHRONO_SW(CharT, "{:L%F %T}"), tp);
}

template <class CharT, class Traits, class Duration>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const file_time<Duration> tp) {
  return os << std::format(os.getloc(), CHRONO_SW(CharT, "{:L%F %T}"), tp);
}

template <class CharT, class Traits, class Duration>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const local_time<Duration> tp) {
  return os << sys_time<Duration>{tp.time_since_epoch()};
}

template <class CharT, class Traits, class Rep, class Period>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const duration<Rep, Period>& d) {
  basic_ostringstream<CharT, Traits> s;
  s.flags(os.flags());
  s.imbue(os.getloc());
  s.precision(os.precision());
  s << d.count() << chrono::units_suffix<CharT, Period>();
  return os << s.str();
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const day& d) {
  return os << (d.ok() ? std::format(CHRONO_SW(CharT, "{:%d}"), d)
                       : std::format(CHRONO_SW(CharT, "{:02} is not a valid day"), static_cast<unsigned>(d)));
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const month& m) {
  return os << (m.ok() ? std::format(os.getloc(), CHRONO_SW(CharT, "{:L%b}"), m)
                       : std::format(os.getloc(), CHRONO_SW(CharT, "{} is not a valid month"),
                                     static_cast<unsigned>(m)));
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const year& y) {
  return os << (y.ok() ? std::format(CHRONO_SW(CharT, "{:%Y}"), y)
                       : std::format(CHRONO_SW(CharT, "{:%Y} is not a valid year"), y));
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const weekday& wd) {
  return os << (wd.ok() ? std::format(os.getloc(), CHRONO_SW(CharT, "{:L%a}"), wd)
                        : std::format(os.getloc(), CHRONO_SW(CharT, "{} is not a valid weekday"),
                                      static_cast<unsigned>(wd.c_encoding())));
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const weekday_indexed& wdi) {
  auto i = wdi.index();
  return os << (i >= 1 && i <= 5
                    ? std::format(os.getloc(), CHRONO_SW(CharT, "{:L}[{}]"), wdi.weekday(), i)
                    : std::format(os.getloc(), CHRONO_SW(CharT, "{:L}[{} is not a valid index]"), wdi.weekday(), i));
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const weekday_last& wdl) {
  return os << std::format(os.getloc(), CHRONO_SW(CharT, "{:L}[last]"), wdl.weekday());
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const month_day& md) {
  return os << std::format(os.getloc(), CHRONO_SW(CharT, "{:L}/{}"), md.month(), md.day());
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const month_day_last& mdl) {
  return os << std::format(os.getloc(), CHRONO_SW(CharT, "{:L}/last"), mdl.month());
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const month_weekday& mwd) {
  return os << std::format(os.getloc(), CHRONO_SW(CharT, "{:L}/{:L}"), mwd.month(), mwd.weekday_indexed());
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const month_weekday_last& mwdl) {
  return os << std::format(os.getloc(), CHRONO_SW(CharT, "{:L}/{:L}"), mwdl.month(), mwdl.weekday_last());
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const year_month& ym) {
  return os << std::format(os.getloc(), CHRONO_SW(CharT, "{}/{:L}"), ym.year(), ym.month());
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const year_month_day& ymd) {
  return os << (ymd.ok() ? std::format(CHRONO_SW(CharT, "{:%F}"), ymd)
                         : std::format(CHRONO_SW(CharT, "{:%F} is not a valid date"), ymd));
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const year_month_day_last& ymdl) {
  return os << std::format(os.getloc(), CHRONO_SW(CharT, "{}/{:L}"), ymdl.year(), ymdl.month_day_last());
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const year_month_weekday& ymwd) {
  return os << std::format(os.getloc(), CHRONO_SW(CharT, "{}/{:L}/{:L}"), ymwd.year(), ymwd.month(),
                           ymwd.weekday_indexed());
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const year_month_weekday_last& ymwdl) {
  return os << std::format(os.getloc(), CHRONO_SW(CharT, "{}/{:L}/{:L}"), ymwdl.year(), ymwdl.month(),
                           ymwdl.weekday_last());
}

template <class CharT, class Traits, class Duration>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const hh_mm_ss<Duration> hms) {
  return os << std::format(os.getloc(), CHRONO_SW(CharT, "{:L%T}"), hms);
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const sys_info& info) {
  std::basic_string<CharT> abbrev{info.abbrev.begin(), info.abbrev.end()};
  return os << std::format(CHRONO_SW(CharT, "[{:%F %T}, {:%F %T}) {:%T} {:%Q%q} \"{}\""),
                           info.begin, info.end, hh_mm_ss{info.offset}, info.save, abbrev);
}

template <class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const local_info& info) {
  auto result = [&]() -> basic_string<CharT> {
    switch (info.result) {
    case local_info::unique:
      return CHRONO_SW(CharT, "unique");
    case local_info::nonexistent:
      return CHRONO_SW(CharT, "non-existent");
    case local_info::ambiguous:
      return CHRONO_SW(CharT, "ambiguous");
    default:
      return std::format(CHRONO_SW(CharT, "unspecified result ({})"), info.result);
    }
  };
  return os << std::format(CHRONO_SW(CharT, "{}: {{{}, {}}}"), result(), info.first, info.second);
}

//===----------------------------------------------------------------------===//
// nonexistent_local_time / ambiguous_local_time ([time.zone.exception])
//===----------------------------------------------------------------------===//
class nonexistent_local_time : public runtime_error {
public:
  template <class Duration>
  nonexistent_local_time(const local_time<Duration>& time, const local_info& info)
      : runtime_error{create_message(time, info)} {}

  nonexistent_local_time(const nonexistent_local_time&)            = default;
  nonexistent_local_time& operator=(const nonexistent_local_time&) = default;

private:
  template <class Duration>
  string create_message(const local_time<Duration>& time, const local_info& info) {
    return std::format(
        R"({} is in a gap between
{} {} and
{} {} which are both equivalent to
{} UTC)",
        time,
        local_seconds{info.first.end.time_since_epoch()} + info.first.offset,
        info.first.abbrev,
        local_seconds{info.second.begin.time_since_epoch()} + info.second.offset,
        info.second.abbrev,
        info.first.end);
  }
};

class ambiguous_local_time : public runtime_error {
public:
  template <class Duration>
  ambiguous_local_time(const local_time<Duration>& time, const local_info& info)
      : runtime_error{create_message(time, info)} {}

  ambiguous_local_time(const ambiguous_local_time&)            = default;
  ambiguous_local_time& operator=(const ambiguous_local_time&) = default;

private:
  template <class Duration>
  string create_message(const local_time<Duration>& time, const local_info& info) {
    return std::format(
        R"({0} is ambiguous.  It could be
{0} {1} == {2} UTC or
{0} {3} == {4} UTC)",
        time,
        info.first.abbrev,
        time - info.first.offset,
        info.second.abbrev,
        time - info.second.offset);
  }
};

} // namespace chrono

} // namespace std

#include <bits/chrono_tzdb.h>
