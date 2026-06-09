// Tests for <numbers>.
// No #pragma once; no main() — gen_main.sh generates main().
#include <numbers>
#include "test.h"

// ── values match the hex floating-point literals from the libc++ test ────────

void test_numbers_e() {
    static_assert(std::numbers::e          == 0x1.5bf0a8b145769p+1);
    static_assert(std::numbers::e_v<double>      == 0x1.5bf0a8b145769p+1);
    static_assert(std::numbers::e_v<long double> == 0x1.5bf0a8b145769p+1l);
    static_assert(std::numbers::e_v<float>       == 0x1.5bf0a8p+1f);
}

void test_numbers_log2e() {
    static_assert(std::numbers::log2e          == 0x1.71547652b82fep+0);
    static_assert(std::numbers::log2e_v<double>      == 0x1.71547652b82fep+0);
    static_assert(std::numbers::log2e_v<long double> == 0x1.71547652b82fep+0l);
    static_assert(std::numbers::log2e_v<float>       == 0x1.715476p+0f);
}

void test_numbers_log10e() {
    static_assert(std::numbers::log10e          == 0x1.bcb7b1526e50ep-2);
    static_assert(std::numbers::log10e_v<double>      == 0x1.bcb7b1526e50ep-2);
    static_assert(std::numbers::log10e_v<long double> == 0x1.bcb7b1526e50ep-2l);
    static_assert(std::numbers::log10e_v<float>       == 0x1.bcb7b15p-2f);
}

void test_numbers_pi() {
    static_assert(std::numbers::pi          == 0x1.921fb54442d18p+1);
    static_assert(std::numbers::pi_v<double>      == 0x1.921fb54442d18p+1);
    static_assert(std::numbers::pi_v<long double> == 0x1.921fb54442d18p+1l);
    static_assert(std::numbers::pi_v<float>       == 0x1.921fb54p+1f);
}

void test_numbers_inv_pi() {
    static_assert(std::numbers::inv_pi          == 0x1.45f306dc9c883p-2);
    static_assert(std::numbers::inv_pi_v<double>      == 0x1.45f306dc9c883p-2);
    static_assert(std::numbers::inv_pi_v<long double> == 0x1.45f306dc9c883p-2l);
    static_assert(std::numbers::inv_pi_v<float>       == 0x1.45f306p-2f);
}

void test_numbers_inv_sqrtpi() {
    static_assert(std::numbers::inv_sqrtpi          == 0x1.20dd750429b6dp-1);
    static_assert(std::numbers::inv_sqrtpi_v<double>      == 0x1.20dd750429b6dp-1);
    static_assert(std::numbers::inv_sqrtpi_v<long double> == 0x1.20dd750429b6dp-1l);
    static_assert(std::numbers::inv_sqrtpi_v<float>       == 0x1.20dd76p-1f);
}

void test_numbers_ln2() {
    static_assert(std::numbers::ln2          == 0x1.62e42fefa39efp-1);
    static_assert(std::numbers::ln2_v<double>      == 0x1.62e42fefa39efp-1);
    static_assert(std::numbers::ln2_v<long double> == 0x1.62e42fefa39efp-1l);
    static_assert(std::numbers::ln2_v<float>       == 0x1.62e42fp-1f);
}

void test_numbers_ln10() {
    static_assert(std::numbers::ln10          == 0x1.26bb1bbb55516p+1);
    static_assert(std::numbers::ln10_v<double>      == 0x1.26bb1bbb55516p+1);
    static_assert(std::numbers::ln10_v<long double> == 0x1.26bb1bbb55516p+1l);
    static_assert(std::numbers::ln10_v<float>       == 0x1.26bb1bp+1f);
}

void test_numbers_sqrt2() {
    static_assert(std::numbers::sqrt2          == 0x1.6a09e667f3bcdp+0);
    static_assert(std::numbers::sqrt2_v<double>      == 0x1.6a09e667f3bcdp+0);
    static_assert(std::numbers::sqrt2_v<long double> == 0x1.6a09e667f3bcdp+0l);
    static_assert(std::numbers::sqrt2_v<float>       == 0x1.6a09e6p+0f);
}

void test_numbers_sqrt3() {
    static_assert(std::numbers::sqrt3          == 0x1.bb67ae8584caap+0);
    static_assert(std::numbers::sqrt3_v<double>      == 0x1.bb67ae8584caap+0);
    static_assert(std::numbers::sqrt3_v<long double> == 0x1.bb67ae8584caap+0l);
    static_assert(std::numbers::sqrt3_v<float>       == 0x1.bb67aep+0f);
}

void test_numbers_inv_sqrt3() {
    static_assert(std::numbers::inv_sqrt3          == 0x1.279a74590331cp-1);
    static_assert(std::numbers::inv_sqrt3_v<double>      == 0x1.279a74590331cp-1);
    static_assert(std::numbers::inv_sqrt3_v<long double> == 0x1.279a74590331cp-1l);
    static_assert(std::numbers::inv_sqrt3_v<float>       == 0x1.279a74p-1f);
}

void test_numbers_egamma() {
    static_assert(std::numbers::egamma          == 0x1.2788cfc6fb619p-1);
    static_assert(std::numbers::egamma_v<double>      == 0x1.2788cfc6fb619p-1);
    static_assert(std::numbers::egamma_v<long double> == 0x1.2788cfc6fb619p-1l);
    static_assert(std::numbers::egamma_v<float>       == 0x1.2788cfp-1f);
}

void test_numbers_phi() {
    static_assert(std::numbers::phi          == 0x1.9e3779b97f4a8p+0);
    static_assert(std::numbers::phi_v<double>      == 0x1.9e3779b97f4a8p+0);
    static_assert(std::numbers::phi_v<long double> == 0x1.9e3779b97f4a8p+0l);
    static_assert(std::numbers::phi_v<float>       == 0x1.9e3779ap+0f);
}

// ── addressability: the constants must have linkage (odr-usable) ─────────────

void test_numbers_defined() {
    [[maybe_unused]] const double* p0  = &std::numbers::e;
    [[maybe_unused]] const double* p1  = &std::numbers::log2e;
    [[maybe_unused]] const double* p2  = &std::numbers::log10e;
    [[maybe_unused]] const double* p3  = &std::numbers::pi;
    [[maybe_unused]] const double* p4  = &std::numbers::inv_pi;
    [[maybe_unused]] const double* p5  = &std::numbers::inv_sqrtpi;
    [[maybe_unused]] const double* p6  = &std::numbers::ln2;
    [[maybe_unused]] const double* p7  = &std::numbers::ln10;
    [[maybe_unused]] const double* p8  = &std::numbers::sqrt2;
    [[maybe_unused]] const double* p9  = &std::numbers::sqrt3;
    [[maybe_unused]] const double* p10 = &std::numbers::inv_sqrt3;
    [[maybe_unused]] const double* p11 = &std::numbers::egamma;
    [[maybe_unused]] const double* p12 = &std::numbers::phi;

    [[maybe_unused]] const float* f0  = &std::numbers::e_v<float>;
    [[maybe_unused]] const float* f1  = &std::numbers::log2e_v<float>;
    [[maybe_unused]] const float* f2  = &std::numbers::log10e_v<float>;
    [[maybe_unused]] const float* f3  = &std::numbers::pi_v<float>;
    [[maybe_unused]] const float* f4  = &std::numbers::inv_pi_v<float>;
    [[maybe_unused]] const float* f5  = &std::numbers::inv_sqrtpi_v<float>;
    [[maybe_unused]] const float* f6  = &std::numbers::ln2_v<float>;
    [[maybe_unused]] const float* f7  = &std::numbers::ln10_v<float>;
    [[maybe_unused]] const float* f8  = &std::numbers::sqrt2_v<float>;
    [[maybe_unused]] const float* f9  = &std::numbers::sqrt3_v<float>;
    [[maybe_unused]] const float* f10 = &std::numbers::inv_sqrt3_v<float>;
    [[maybe_unused]] const float* f11 = &std::numbers::egamma_v<float>;
    [[maybe_unused]] const float* f12 = &std::numbers::phi_v<float>;

    [[maybe_unused]] const long double* ld0  = &std::numbers::e_v<long double>;
    [[maybe_unused]] const long double* ld1  = &std::numbers::log2e_v<long double>;
    [[maybe_unused]] const long double* ld2  = &std::numbers::log10e_v<long double>;
    [[maybe_unused]] const long double* ld3  = &std::numbers::pi_v<long double>;
    [[maybe_unused]] const long double* ld4  = &std::numbers::inv_pi_v<long double>;
    [[maybe_unused]] const long double* ld5  = &std::numbers::inv_sqrtpi_v<long double>;
    [[maybe_unused]] const long double* ld6  = &std::numbers::ln2_v<long double>;
    [[maybe_unused]] const long double* ld7  = &std::numbers::ln10_v<long double>;
    [[maybe_unused]] const long double* ld8  = &std::numbers::sqrt2_v<long double>;
    [[maybe_unused]] const long double* ld9  = &std::numbers::sqrt3_v<long double>;
    [[maybe_unused]] const long double* ld10 = &std::numbers::inv_sqrt3_v<long double>;
    [[maybe_unused]] const long double* ld11 = &std::numbers::egamma_v<long double>;
    [[maybe_unused]] const long double* ld12 = &std::numbers::phi_v<long double>;
}
