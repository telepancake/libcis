// Tests for <system_error>
// Ported from libc++ test/std/diagnostics/syserr/
#include <system_error>
#include "test.h"

//===----------------------------------------------------------------------===//
// errc enum
//===----------------------------------------------------------------------===//
void test_system_error_errc() {
    // errc values correspond to errno values
    CHECK(static_cast<int>(std::errc::invalid_argument) == EINVAL);
    CHECK(static_cast<int>(std::errc::no_such_file_or_directory) == ENOENT);
    CHECK(static_cast<int>(std::errc::permission_denied) == EACCES);
    CHECK(static_cast<int>(std::errc::result_out_of_range) == ERANGE);

    // is_error_condition_enum<errc> is true
    static_assert(std::is_error_condition_enum<std::errc>::value);
    static_assert(std::is_error_condition_enum_v<std::errc>);

    // is_error_code_enum<errc> is false
    static_assert(!std::is_error_code_enum<std::errc>::value);
    static_assert(!std::is_error_code_enum_v<std::errc>);
}

//===----------------------------------------------------------------------===//
// error_category
//===----------------------------------------------------------------------===//
void test_system_error_categories() {
    const std::error_category& gen = std::generic_category();
    const std::error_category& sys = std::system_category();

    // identity (same object returned each call)
    CHECK(&gen == &std::generic_category());
    CHECK(&sys == &std::system_category());

    // categories are distinct
    CHECK(!(gen == sys));
    CHECK(gen != sys);  // Uses synthesized != from ==

    // name
    CHECK(gen.name() != nullptr);
    CHECK(sys.name() != nullptr);
    std::string gen_name = gen.name();
    std::string sys_name = sys.name();
    CHECK(gen_name == "generic");
    CHECK(sys_name == "system");

    // message returns a non-empty string for known errno
    std::string msg = gen.message(EINVAL);
    CHECK(!msg.empty());
}

//===----------------------------------------------------------------------===//
// error_condition
//===----------------------------------------------------------------------===//
void test_system_error_condition() {
    // default constructor → value 0, generic_category
    std::error_condition ec0;
    CHECK(ec0.value() == 0);
    CHECK(&ec0.category() == &std::generic_category());
    CHECK(!ec0);  // bool conversion

    // value + category constructor
    std::error_condition ec1(5, std::generic_category());
    CHECK(ec1.value() == 5);
    CHECK(&ec1.category() == &std::generic_category());
    CHECK(!!ec1);

    // make_error_condition(errc)
    std::error_condition ec2 = std::make_error_condition(std::errc::invalid_argument);
    CHECK(ec2.value() == EINVAL);
    CHECK(&ec2.category() == &std::generic_category());
    CHECK(!!ec2);

    // Construct from errc (via is_error_condition_enum)
    std::error_condition ec3(std::errc::no_such_file_or_directory);
    CHECK(ec3.value() == ENOENT);
    CHECK(&ec3.category() == &std::generic_category());

    // assign + clear
    ec1.assign(7, std::generic_category());
    CHECK(ec1.value() == 7);
    ec1.clear();
    CHECK(ec1.value() == 0);
    CHECK(!ec1);

    // message
    std::error_condition ec4(EINVAL, std::generic_category());
    CHECK(!ec4.message().empty());

    // equality
    std::error_condition ec5(EINVAL, std::generic_category());
    std::error_condition ec6(EINVAL, std::generic_category());
    std::error_condition ec7(ENOENT, std::generic_category());
    CHECK(ec5 == ec6);
    CHECK(!(ec5 == ec7));
}

//===----------------------------------------------------------------------===//
// error_code
//===----------------------------------------------------------------------===//
void test_system_error_code() {
    // default constructor → value 0, system_category
    std::error_code ec0;
    CHECK(ec0.value() == 0);
    CHECK(&ec0.category() == &std::system_category());
    CHECK(!ec0);

    // value + category constructor
    std::error_code ec1(5, std::generic_category());
    CHECK(ec1.value() == 5);
    CHECK(&ec1.category() == &std::generic_category());
    CHECK(!!ec1);

    // assign + clear
    ec1.assign(7, std::system_category());
    CHECK(ec1.value() == 7);
    ec1.clear();
    CHECK(ec1.value() == 0);
    CHECK(&ec1.category() == &std::system_category());
    CHECK(!ec1);

    // message
    std::error_code ec2(EINVAL, std::generic_category());
    CHECK(!ec2.message().empty());

    // default_error_condition
    std::error_code ec3(EINVAL, std::system_category());
    std::error_condition cond = ec3.default_error_condition();
    CHECK(cond.value() == EINVAL);

    // equality
    std::error_code ec4(EINVAL, std::generic_category());
    std::error_code ec5(EINVAL, std::generic_category());
    std::error_code ec6(ENOENT, std::generic_category());
    CHECK(ec4 == ec5);
    CHECK(!(ec4 == ec6));

    // error_code == error_condition via equivalent
    std::error_code ec7(EINVAL, std::generic_category());
    std::error_condition ec8(EINVAL, std::generic_category());
    CHECK(ec7 == ec8);
}

//===----------------------------------------------------------------------===//
// make_error_code / make_error_condition (errc)
//===----------------------------------------------------------------------===//
void test_system_error_make() {
    // make_error_code(errc) goes to generic_category per standard
    std::error_code ec = std::make_error_code(std::errc::permission_denied);
    CHECK(ec.value() == EACCES);
    CHECK(&ec.category() == &std::generic_category());

    // make_error_condition(errc) goes to generic_category
    std::error_condition cond = std::make_error_condition(std::errc::permission_denied);
    CHECK(cond.value() == EACCES);
    CHECK(&cond.category() == &std::generic_category());
}

//===----------------------------------------------------------------------===//
// system_error exception type
//===----------------------------------------------------------------------===//
void test_system_error_system_error() {
    std::error_code ec(EINVAL, std::generic_category());

    // Constructors do not throw (we're -fno-exceptions); we just check that
    // the code() accessor is correct and what() is non-empty.
    std::system_error se1(ec, "test message");
    CHECK(se1.code() == ec);
    CHECK(se1.what() != nullptr);
    // what() should contain at least something
    std::string w1 = se1.what();
    CHECK(!w1.empty());

    std::system_error se2(ec, std::string("another message"));
    CHECK(se2.code() == ec);
    CHECK(se2.what() != nullptr);

    std::system_error se3(ec);
    CHECK(se3.code() == ec);

    std::system_error se4(EINVAL, std::generic_category(), "test");
    CHECK(se4.code().value() == EINVAL);
    CHECK(&se4.code().category() == &std::generic_category());

    std::system_error se5(EINVAL, std::generic_category(), std::string("hello"));
    CHECK(se5.code().value() == EINVAL);

    std::system_error se6(EINVAL, std::generic_category());
    CHECK(se6.code().value() == EINVAL);
}

//===----------------------------------------------------------------------===//
// hash<error_code> / hash<error_condition>
//===----------------------------------------------------------------------===//
void test_system_error_hash() {
    std::hash<std::error_code> h_ec;
    std::hash<std::error_condition> h_econd;

    std::error_code ec1(EINVAL, std::generic_category());
    std::error_code ec2(EINVAL, std::generic_category());
    std::error_code ec3(ENOENT, std::generic_category());

    // Same error_code should hash the same
    CHECK(h_ec(ec1) == h_ec(ec2));
    // Different values typically hash differently (not guaranteed but true here)
    (void)h_ec(ec3);

    std::error_condition econd1(EINVAL, std::generic_category());
    (void)h_econd(econd1);
}
