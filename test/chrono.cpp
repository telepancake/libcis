// test/chrono.cpp — tests for <chrono>
// Uses static_assert for compile-time and CHECK() for runtime checks.
// No #pragma once; no main() — gen_main.sh generates it.

#include <chrono>
#include <cstdint>     // int64_t
#include <type_traits>
#include "test.h"

using namespace std::chrono;
using namespace std::chrono_literals;

//===----------------------------------------------------------------------===//
// duration arithmetic
//===----------------------------------------------------------------------===//
void test_chrono_duration_basic() {
    // construction and count()
    static_assert(seconds{5}.count() == 5LL);
    static_assert(milliseconds{1500}.count() == 1500LL);
    static_assert(nanoseconds{999}.count() == 999LL);

    // default zero-init
    static_assert(duration<int>{}.count() == 0);

    // unary - and +
    static_assert((-seconds{3}).count() == -3LL);
    static_assert((+seconds{3}).count() == 3LL);
}

void test_chrono_duration_arithmetic() {
    // binary + and -
    static_assert((seconds{3} + seconds{2}).count() == 5LL);
    static_assert((seconds{5} - seconds{3}).count() == 2LL);

    // * and /
    static_assert((seconds{4} * 3).count() == 12LL);
    static_assert((3 * seconds{4}).count() == 12LL);
    static_assert((seconds{12} / 4).count() == 3LL);

    // duration / duration
    static_assert(seconds{6} / seconds{2} == 3LL);

    // %
    static_assert((seconds{7} % 3).count() == 1LL);
    static_assert((seconds{7} % seconds{3}).count() == 1LL);

    // compound assignment
    constexpr auto f = [] {
        seconds s{10};
        s += seconds{5};
        s -= seconds{3};
        s *= 2;
        s /= 4;
        return s.count();
    };
    static_assert(f() == 6LL); // ((10+5-3)*2)/4 = 12/4*2... wait: (10+5)=15, -3=12, *2=24, /4=6
}

void test_chrono_duration_comparisons() {
    static_assert(seconds{1} == seconds{1});
    static_assert(!(seconds{1} == seconds{2}));
    static_assert(seconds{1} < seconds{2});
    static_assert(seconds{2} > seconds{1});
    static_assert(seconds{1} <= seconds{1});
    static_assert(seconds{2} >= seconds{1});

    // cross-unit comparison (ms vs s)
    static_assert(milliseconds{1000} == seconds{1});
    static_assert(milliseconds{999} < seconds{1});
    static_assert(seconds{1} > milliseconds{500});

    // <=> operator
    static_assert((seconds{1} <=> seconds{2}) < 0);
    static_assert((seconds{2} <=> seconds{1}) > 0);
    static_assert((seconds{2} <=> seconds{2}) == 0);
}

//===----------------------------------------------------------------------===//
// duration_cast
//===----------------------------------------------------------------------===//
void test_chrono_duration_cast() {
    static_assert(duration_cast<seconds>(milliseconds{2500}).count() == 2LL);
    static_assert(duration_cast<milliseconds>(seconds{3}).count() == 3000LL);
    static_assert(duration_cast<microseconds>(milliseconds{5}).count() == 5000LL);
    static_assert(duration_cast<seconds>(nanoseconds{2'000'000'000LL}).count() == 2LL);

    // truncates towards zero
    static_assert(duration_cast<seconds>(milliseconds{1999}).count() == 1LL);
    static_assert(duration_cast<seconds>(milliseconds{-1999}).count() == -1LL);
}

//===----------------------------------------------------------------------===//
// floor / ceil / round / abs
//===----------------------------------------------------------------------===//
void test_chrono_duration_floor_ceil_round_abs() {
    // floor truncates towards negative infinity
    static_assert(floor<seconds>(milliseconds{1500}).count() == 1LL);
    static_assert(floor<seconds>(milliseconds{-1500}).count() == -2LL);

    // ceil rounds towards positive infinity
    static_assert(ceil<seconds>(milliseconds{1001}).count() == 2LL);
    static_assert(ceil<seconds>(milliseconds{-1001}).count() == -1LL);

    // round rounds to nearest, ties to even
    static_assert(round<seconds>(milliseconds{1500}).count() == 2LL); // tie: 1 or 2 → even→2
    static_assert(round<seconds>(milliseconds{2500}).count() == 2LL); // tie: 2 or 3 → even→2
    static_assert(round<seconds>(milliseconds{1400}).count() == 1LL);
    static_assert(round<seconds>(milliseconds{1600}).count() == 2LL);

    // abs
    static_assert(abs(seconds{-5}).count() == 5LL);
    static_assert(abs(seconds{5}).count() == 5LL);
    static_assert(abs(nanoseconds{-1}).count() == 1LL);
}

//===----------------------------------------------------------------------===//
// duration_values + special values
//===----------------------------------------------------------------------===//
void test_chrono_duration_values() {
    static_assert(duration_values<int>::zero() == 0);
    static_assert(seconds::zero() == seconds{0});
    // min/max just need to compile and be sensible
    static_assert(seconds::min() < seconds::zero());
    static_assert(seconds::max() > seconds::zero());
}

//===----------------------------------------------------------------------===//
// treat_as_floating_point
//===----------------------------------------------------------------------===//
void test_chrono_treat_as_floating_point() {
    static_assert(treat_as_floating_point_v<float>);
    static_assert(treat_as_floating_point_v<double>);
    static_assert(!treat_as_floating_point_v<int>);
    static_assert(!treat_as_floating_point_v<long long>);
}

//===----------------------------------------------------------------------===//
// convenience typedefs
//===----------------------------------------------------------------------===//
void test_chrono_typedefs() {
    // rep types are implementation-defined; the standard only requires signed
    // integral types of sufficient width.  Test the portable properties.
    static_assert(std::is_signed_v<nanoseconds::rep>  && sizeof(nanoseconds::rep)  >= 8);
    static_assert(std::is_signed_v<microseconds::rep> && sizeof(microseconds::rep) >= 8);
    static_assert(std::is_signed_v<milliseconds::rep> && sizeof(milliseconds::rep) >= 8);
    static_assert(std::is_signed_v<seconds::rep>      && sizeof(seconds::rep)      >= 4);
    static_assert(std::is_signed_v<minutes::rep>      && sizeof(minutes::rep)      >= 4);
    static_assert(std::is_signed_v<hours::rep>        && sizeof(hours::rep)        >= 4);
    static_assert(std::is_signed_v<days::rep>         && sizeof(days::rep)         >= 4);
    static_assert(std::is_signed_v<weeks::rep>        && sizeof(weeks::rep)        >= 4);
    static_assert(std::is_signed_v<months::rep>       && sizeof(months::rep)       >= 4);
    static_assert(std::is_signed_v<years::rep>        && sizeof(years::rep)        >= 4);

    // period sanity checks
    static_assert(std::ratio_equal_v<nanoseconds::period,  std::nano>);
    static_assert(std::ratio_equal_v<microseconds::period, std::micro>);
    static_assert(std::ratio_equal_v<milliseconds::period, std::milli>);
    static_assert(std::ratio_equal_v<seconds::period,      std::ratio<1>>);
}

//===----------------------------------------------------------------------===//
// literals
//===----------------------------------------------------------------------===//
void test_chrono_literals() {
    static_assert((1h).count() == 1L);
    static_assert((60min).count() == 60L);
    static_assert((1s).count() == 1LL);
    static_assert((1000ms).count() == 1000LL);
    static_assert((1000us).count() == 1000LL);
    static_assert((1000ns).count() == 1000LL);

    // literals via chrono namespace
    using namespace std::chrono;
    auto d = 1h + 30min;
    CHECK(duration_cast<minutes>(d).count() == 90L);
}

//===----------------------------------------------------------------------===//
// time_point
//===----------------------------------------------------------------------===//
void test_chrono_time_point() {
    using tp = time_point<system_clock>;

    // default-constructed is epoch
    constexpr tp epoch;
    static_assert(epoch.time_since_epoch() == seconds{0});

    // arithmetic
    constexpr tp t1(seconds{100});
    constexpr tp t2(seconds{200});
    static_assert((t2 - t1) == microseconds{100'000'000LL});
    static_assert((t1 + microseconds{50'000'000LL}).time_since_epoch()
                  == microseconds{150'000'000LL});
    static_assert((t2 - microseconds{50'000'000LL}).time_since_epoch()
                  == microseconds{150'000'000LL});

    // comparisons
    static_assert(t1 < t2);
    static_assert(t2 > t1);
    static_assert(t1 <= t1);
    static_assert(t1 >= t1);
    static_assert(t1 == t1);
    static_assert(t1 != t2);

    // time_point_cast
    constexpr tp t3(microseconds{5'000'000LL}); // 5 seconds
    static_assert(time_point_cast<seconds>(t3).time_since_epoch() == seconds{5});

    // min / max
    static_assert(tp::min() < tp::max());
}

void test_chrono_time_point_floor_ceil_round() {
    using tp_us = time_point<system_clock, microseconds>;

    constexpr tp_us t(microseconds{1'500'000LL}); // 1.5 seconds
    static_assert(floor<seconds>(t).time_since_epoch()   == seconds{1});
    static_assert(ceil<seconds>(t).time_since_epoch()    == seconds{2});
    static_assert(round<seconds>(t).time_since_epoch()   == seconds{2}); // tie→even
}

//===----------------------------------------------------------------------===//
// clocks: now() returns monotonic-ish (runtime check)
//===----------------------------------------------------------------------===//
void test_chrono_clock_now() {
    auto t1 = steady_clock::now();
    // Do a little work so time actually advances
    int x = 0;
    for (int i = 0; i < 1000; ++i) x += i;
    (void)x;
    auto t2 = steady_clock::now();
    CHECK(t2 >= t1);

    // system_clock now() should also work
    auto st = system_clock::now();
    // should be after 2020-01-01 epoch = 1577836800s
    CHECK(duration_cast<seconds>(st.time_since_epoch()).count() > 1577836800LL);

    // high_resolution_clock
    auto hr1 = high_resolution_clock::now();
    auto hr2 = high_resolution_clock::now();
    CHECK(hr2 >= hr1);
}

void test_chrono_system_clock_to_time_t() {
    auto t = system_clock::now();
    time_t tt = system_clock::to_time_t(t);
    auto t2 = system_clock::from_time_t(tt);
    // from_time_t has second resolution, so t and t2 should be within 1 second
    auto diff = duration_cast<seconds>(t - t2);
    CHECK(diff.count() >= 0 && diff.count() <= 1);
}

//===----------------------------------------------------------------------===//
// calendar: day
//===----------------------------------------------------------------------===//
void test_chrono_calendar_day() {
    static_assert(day{1}.ok());
    static_assert(day{31}.ok());
    static_assert(!day{0}.ok());
    static_assert(!day{32}.ok());
    static_assert(static_cast<unsigned>(day{5}) == 5u);

    static_assert(day{5} == day{5});
    static_assert(day{5} < day{6});
    static_assert(day{7} > day{6});
    static_assert((day{5} + days{3}) == day{8});
    static_assert((day{10} - days{3}) == day{7});
    static_assert((day{10} - day{7}) == days{3});
}

//===----------------------------------------------------------------------===//
// calendar: month
//===----------------------------------------------------------------------===//
void test_chrono_calendar_month() {
    static_assert(month{1}.ok());
    static_assert(month{12}.ok());
    static_assert(!month{0}.ok());
    static_assert(!month{13}.ok());
    static_assert(static_cast<unsigned>(month{3}) == 3u);
    static_assert(January == month{1});
    static_assert(December == month{12});

    // arithmetic: month wrapping
    static_assert((November + months{2}) == January);
    static_assert((January - months{1}) == December);
}

//===----------------------------------------------------------------------===//
// calendar: year
//===----------------------------------------------------------------------===//
void test_chrono_calendar_year() {
    static_assert(year{2000}.is_leap());
    static_assert(!year{1900}.is_leap()); // div 100 but not 400
    static_assert(year{2400}.is_leap());
    static_assert(!year{2023}.is_leap());
    static_assert(year{2024}.is_leap());

    static_assert(static_cast<int>(year{2000}) == 2000);
    static_assert(year{2000} == year{2000});
    static_assert(year{2000} < year{2001});

    static_assert((year{2000} + years{5}) == year{2005});
    static_assert((year{2005} - years{5}) == year{2000});
    static_assert((year{2005} - year{2000}) == years{5});
}

//===----------------------------------------------------------------------===//
// calendar: weekday
//===----------------------------------------------------------------------===//
void test_chrono_calendar_weekday() {
    static_assert(Sunday.c_encoding()    == 0u);
    static_assert(Monday.c_encoding()    == 1u);
    static_assert(Saturday.c_encoding()  == 6u);

    static_assert(Monday.iso_encoding()  == 1u);
    static_assert(Sunday.iso_encoding()  == 7u);

    static_assert(Sunday.ok());
    // weekday(7) wraps to 0 (Sunday) which is ok
    static_assert(weekday{7} == Sunday);
    static_assert(weekday{7}.ok());

    // weekday arithmetic
    static_assert((Monday + days{6}) == Sunday);
    static_assert((Sunday - days{1}) == Saturday);
    static_assert((Tuesday - Monday) == days{1});

    // operator[] -> weekday_indexed, weekday_last
    constexpr auto wi = Monday[2];
    static_assert(wi.weekday() == Monday);
    static_assert(wi.index() == 2u);

    constexpr auto wl = Friday[last];
    static_assert(wl.weekday() == Friday);
    static_assert(wl.ok());
}

//===----------------------------------------------------------------------===//
// calendar: sys_days, weekday from sys_days
//===----------------------------------------------------------------------===//
void test_chrono_calendar_weekday_from_sys_days() {
    // 1970-01-01 was a Thursday
    sys_days epoch{days{0}};
    CHECK(weekday{epoch} == Thursday);

    // 2024-01-01 is Monday
    // Days from 1970-01-01 to 2024-01-01:
    //   Using year_month_day constructor:
    year_month_day ymd2024jan1{year{2024}, January, day{1}};
    sys_days sd2024{ymd2024jan1};
    CHECK(weekday{sd2024} == Monday);
}

//===----------------------------------------------------------------------===//
// calendar: year_month_day, round-trip to sys_days
//===----------------------------------------------------------------------===//
void test_chrono_calendar_year_month_day() {
    // Construction and accessors
    constexpr year_month_day ymd{year{2024}, month{3}, day{15}};
    static_assert(ymd.year()  == year{2024});
    static_assert(ymd.month() == March);
    static_assert(ymd.day()   == day{15});
    static_assert(ymd.ok());

    // / operator syntax
    constexpr auto ymd2 = year{2024} / March / day{15};
    static_assert(ymd2 == ymd);

    constexpr auto ymd3 = year{2024} / 3 / 15;
    static_assert(ymd3 == ymd);

    // Round-trip: ymd -> sys_days -> ymd
    year_month_day rt{sys_days{ymd}};
    CHECK(rt.year()  == year{2024});
    CHECK(rt.month() == March);
    CHECK(rt.day()   == day{15});

    // Another round-trip: epoch
    year_month_day epoch_ymd{sys_days{days{0}}};
    CHECK(epoch_ymd.year()  == year{1970});
    CHECK(epoch_ymd.month() == January);
    CHECK(epoch_ymd.day()   == day{1});

    // Forward: 1970-01-01 + 365 days = 1971-01-01
    year_month_day next{sys_days{days{365}}};
    CHECK(next.year()  == year{1971});
    CHECK(next.month() == January);
    CHECK(next.day()   == day{1});
}

void test_chrono_calendar_is_leap() {
    CHECK(year{2000}.is_leap());
    CHECK(!year{1900}.is_leap());
    CHECK(year{2024}.is_leap());
    CHECK(!year{2023}.is_leap());
    CHECK(year{400}.is_leap());
    CHECK(!year{100}.is_leap());
}

//===----------------------------------------------------------------------===//
// calendar: year_month_day_last and last
//===----------------------------------------------------------------------===//
void test_chrono_calendar_last() {
    // last day of January 2024 = 31
    year_month_day_last ymdl{year{2024}, month_day_last{January}};
    CHECK(static_cast<unsigned>(ymdl.day()) == 31u);

    // last day of February 2024 (leap) = 29
    year_month_day_last feb2024{year{2024}, month_day_last{February}};
    CHECK(static_cast<unsigned>(feb2024.day()) == 29u);

    // last day of February 2023 (non-leap) = 28
    year_month_day_last feb2023{year{2023}, month_day_last{February}};
    CHECK(static_cast<unsigned>(feb2023.day()) == 28u);

    // operator/ syntax: year{2024} / February / last
    auto ymdl2 = year{2024} / February / last;
    CHECK(static_cast<unsigned>(ymdl2.day()) == 29u);
}

//===----------------------------------------------------------------------===//
// calendar: month_day operator/
//===----------------------------------------------------------------------===//
void test_chrono_calendar_month_day() {
    constexpr auto md = January / day{15};
    static_assert(md.month() == January);
    static_assert(md.day()   == day{15});
    static_assert(md.ok());

    // alternate syntax
    constexpr auto md2 = day{15} / January;
    static_assert(md2 == md);

    // year_month_day via operator/
    constexpr auto ymd = year{2024} / md;
    static_assert(ymd == (year{2024} / January / day{15}));
}

//===----------------------------------------------------------------------===//
// calendar: year_month_weekday
//===----------------------------------------------------------------------===//
void test_chrono_calendar_year_month_weekday() {
    // 2024-01-01 is Monday — first Monday in January 2024
    sys_days sd{year_month_day{year{2024}, January, day{1}}};
    year_month_weekday ymw{sd};
    CHECK(ymw.year()    == year{2024});
    CHECK(ymw.month()   == January);
    CHECK(ymw.weekday() == Monday);
    CHECK(ymw.index()   == 1u);
    CHECK(ymw.ok());

    // Convert back to sys_days should give 2024-01-01
    sys_days back{ymw};
    year_month_day ymd_back{back};
    CHECK(ymd_back.year()  == year{2024});
    CHECK(ymd_back.month() == January);
    CHECK(ymd_back.day()   == day{1});
}

//===----------------------------------------------------------------------===//
// hh_mm_ss
//===----------------------------------------------------------------------===//
void test_chrono_hh_mm_ss() {
    // 1h 30min 15s 500ms
    auto dur = hours{1} + minutes{30} + seconds{15} + milliseconds{500};
    hh_mm_ss hms{dur};
    CHECK(!hms.is_negative());
    CHECK(hms.hours()   == hours{1});
    CHECK(hms.minutes() == minutes{30});
    CHECK(hms.seconds() == seconds{15});
    CHECK(hms.subseconds() == milliseconds{500});

    // negative duration
    hh_mm_ss hms2{-seconds{90}};
    CHECK(hms2.is_negative());
    CHECK(hms2.hours()   == hours{0});
    CHECK(hms2.minutes() == minutes{1});
    CHECK(hms2.seconds() == seconds{30});
}

void test_chrono_is_am_pm_make12_make24() {
    static_assert(is_am(hours{0}));
    static_assert(is_am(hours{11}));
    static_assert(!is_am(hours{12}));
    static_assert(is_pm(hours{12}));
    static_assert(is_pm(hours{23}));
    static_assert(!is_pm(hours{24}));

    static_assert(make12(hours{0})  == hours{12});
    static_assert(make12(hours{12}) == hours{12});
    static_assert(make12(hours{13}) == hours{1});
    static_assert(make12(hours{23}) == hours{11});

    static_assert(make24(hours{12}, true)  == hours{12}); // 12 PM = 12
    static_assert(make24(hours{1},  true)  == hours{13}); // 1 PM  = 13
    static_assert(make24(hours{12}, false) == hours{0});  // 12 AM = 0
    static_assert(make24(hours{1},  false) == hours{1});  // 1 AM  = 1
}

//===----------------------------------------------------------------------===//
// common_type specialization
//===----------------------------------------------------------------------===//
void test_chrono_common_type() {
    using CT = std::common_type_t<seconds, milliseconds>;
    static_assert(std::is_same_v<CT, milliseconds>);

    using CT2 = std::common_type_t<hours, minutes>;
    static_assert(std::is_same_v<CT2, minutes>);
}
