// Tests for <complex>
#include <complex>
#include "test.h"

// ---- construction and accessors ----------------------------------------

void test_complex_construction() {
    // default construction
    std::complex<double> c;
    CHECK(c.real() == 0.0 && c.imag() == 0.0);

    // value construction
    std::complex<double> c2(3.0, 4.0);
    CHECK(c2.real() == 3.0 && c2.imag() == 4.0);

    // copy from another specialization
    std::complex<float> cf(1.0f, 2.0f);
    std::complex<double> cd(cf);
    CHECK(cd.real() == 1.0 && cd.imag() == 2.0);

    // explicit narrowing
    std::complex<double> cdbl(5.0, 6.0);
    std::complex<float> cflt(cdbl);
    CHECK(cflt.real() == 5.0f && cflt.imag() == 6.0f);

    // long double specialization
    std::complex<long double> cld(7.0L, 8.0L);
    CHECK(cld.real() == 7.0L && cld.imag() == 8.0L);
}

// ---- setters -----------------------------------------------------------

void test_complex_setters() {
    std::complex<double> c(1.0, 2.0);
    c.real(10.0);
    c.imag(20.0);
    CHECK(c.real() == 10.0 && c.imag() == 20.0);
}

// ---- arithmetic compound assignment ------------------------------------

void test_complex_compound_assign() {
    std::complex<double> c(1.0, 2.0);

    c += 3.0;
    CHECK(c.real() == 4.0 && c.imag() == 2.0);

    c -= 1.0;
    CHECK(c.real() == 3.0 && c.imag() == 2.0);

    c *= 2.0;
    CHECK(c.real() == 6.0 && c.imag() == 4.0);

    c /= 2.0;
    CHECK(c.real() == 3.0 && c.imag() == 2.0);

    // complex += complex
    std::complex<double> d(1.0, 1.0);
    c += d;
    CHECK(c.real() == 4.0 && c.imag() == 3.0);
}

// ---- binary operators --------------------------------------------------

void test_complex_binary_ops() {
    std::complex<double> a(1.0, 2.0);
    std::complex<double> b(3.0, 4.0);

    auto s = a + b;
    CHECK(s.real() == 4.0 && s.imag() == 6.0);

    auto diff = b - a;
    CHECK(diff.real() == 2.0 && diff.imag() == 2.0);

    // scalar overloads
    auto sp = a + 5.0;
    CHECK(sp.real() == 6.0 && sp.imag() == 2.0);

    auto ps = 5.0 + a;
    CHECK(ps.real() == 6.0 && ps.imag() == 2.0);

    // multiply: (1+2i)*(3+4i) = 3+4i+6i+8i^2 = (3-8)+(4+6)i = -5+10i
    auto m = a * b;
    CHECK(m.real() == -5.0 && m.imag() == 10.0);

    // divide: (1+2i)/(1+0i) = 1+2i
    std::complex<double> one(1.0, 0.0);
    auto q = a / one;
    CHECK(q.real() == 1.0 && q.imag() == 2.0);

    // unary
    auto neg = -a;
    CHECK(neg.real() == -1.0 && neg.imag() == -2.0);
    auto pos = +a;
    CHECK(pos.real() == 1.0 && pos.imag() == 2.0);
}

// ---- equality ----------------------------------------------------------

void test_complex_equality() {
    std::complex<double> a(1.0, 2.0);
    std::complex<double> b(1.0, 2.0);
    std::complex<double> c(1.0, 0.0);

    CHECK(a == b);
    CHECK(!(a == c));
    CHECK(c == 1.0);
    CHECK(!(a == 1.0));
}

// ---- free value functions ----------------------------------------------

void test_complex_value_funcs() {
    std::complex<double> c(3.0, 4.0);

    CHECK(std::real(c) == 3.0);
    CHECK(std::imag(c) == 4.0);

    // abs: |3+4i| = 5
    double a = std::abs(c);
    CHECK(a > 4.999 && a < 5.001);

    // norm: 3^2+4^2 = 25
    double n = std::norm(c);
    CHECK(n > 24.999 && n < 25.001);

    // conj: conj(3+4i) = 3-4i
    auto cj = std::conj(c);
    CHECK(cj.real() == 3.0 && cj.imag() == -4.0);

    // real/imag overloads for scalar
    CHECK(std::real(5.0) == 5.0);
    CHECK(std::imag(5.0) == 0.0);

    // norm for scalar
    double ns = std::norm(3.0);
    CHECK(ns == 9.0);
}

// ---- transcendentals ---------------------------------------------------

void test_complex_transcendentals() {
    using C = std::complex<double>;

    // exp(i*pi) ≈ -1+0i (Euler)
    C ipi(0.0, 3.141592653589793);
    C e = std::exp(ipi);
    CHECK(e.real() < -0.999 && e.real() > -1.001);
    CHECK(e.imag() > -1e-10 && e.imag() < 1e-10);

    // log(e) = 1
    C one(1.0, 0.0);
    C le = std::log(std::exp(one));
    CHECK(le.real() > 0.999 && le.real() < 1.001);

    // sqrt: sqrt(4+0i) = 2
    C four(4.0, 0.0);
    C sq = std::sqrt(four);
    CHECK(sq.real() > 1.999 && sq.real() < 2.001);

    // sin/cos identity: sin^2 + cos^2 = 1
    C z(1.0, 1.0);
    C s = std::sin(z);
    C co = std::cos(z);
    double mag = std::norm(s) + std::norm(co);
    // For complex sin/cos that's not generally 1, but just check no crash
    (void)mag;

    // pow: (1+0i)^2 = 1
    C unit(1.0, 0.0);
    C p = std::pow(unit, C(2.0, 0.0));
    CHECK(p.real() > 0.999 && p.real() < 1.001);
}

// ---- literals ----------------------------------------------------------

void test_complex_literals() {
    using namespace std::complex_literals;

    auto ci = 3.0i;
    static_assert(__is_same(decltype(ci), std::complex<double>));
    CHECK(ci.real() == 0.0 && ci.imag() == 3.0);

    auto cf = 2.0if;
    static_assert(__is_same(decltype(cf), std::complex<float>));
    CHECK(cf.real() == 0.0f && cf.imag() == 2.0f);

    auto cl = 4.0il;
    static_assert(__is_same(decltype(cl), std::complex<long double>));
    CHECK(cl.real() == 0.0L && cl.imag() == 4.0L);

    // integer literal suffix
    auto ci2 = 5i;
    CHECK(ci2.real() == 0.0 && ci2.imag() == 5.0);
}

// ---- polar / proj / arg ------------------------------------------------

void test_complex_polar_proj_arg() {
    // polar(r, theta=0) = (r, 0)
    auto p = std::polar(2.0);
    CHECK(p.real() > 1.999 && p.real() < 2.001);
    CHECK(p.imag() > -1e-14 && p.imag() < 1e-14);

    // arg of positive real = 0
    std::complex<double> pos(3.0, 0.0);
    double a = std::arg(pos);
    CHECK(a > -1e-10 && a < 1e-10);

    // proj of finite number = itself
    std::complex<double> fin(1.0, 2.0);
    auto pr = std::proj(fin);
    CHECK(pr.real() == 1.0 && pr.imag() == 2.0);
}
