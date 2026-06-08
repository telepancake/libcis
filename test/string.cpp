// test/string.cpp — basic_string tests for libcis
// Each test function is named test_string_<name>(). No main(). No progress printing.
// Crash on failure via CHECK or static_assert.
#include <string>
#include <string_view>
#include "test.h"

//-----------------------------------------------------------------------------
// test_string_basic — construction, size, capacity, c_str
//-----------------------------------------------------------------------------
void test_string_basic() {
    // default ctor
    std::string s;
    CHECK(s.empty());
    CHECK(s.size() == 0);
    CHECK(s.capacity() >= s.size()); // capacity always >= size
    CHECK(s[0] == '\0');   // null terminator always present

    // from const char*
    std::string a("hello");
    CHECK(a.size() == 5);
    CHECK(a[0] == 'h');
    CHECK(a[4] == 'o');
    CHECK(a[5] == '\0');

    // from pointer + length
    std::string b("world!", 5);
    CHECK(b.size() == 5);
    CHECK(b == "world");

    // count + char
    std::string c(5, 'x');
    CHECK(c.size() == 5);
    CHECK(c == "xxxxx");

    // copy ctor
    std::string d(a);
    CHECK(d == a);
    CHECK(d.data() != a.data()); // different storage

    // move ctor
    std::string e(std::move(d));
    CHECK(e == "hello");
}

//-----------------------------------------------------------------------------
// test_string_sso — small string optimization stays in-buffer
//-----------------------------------------------------------------------------
void test_string_sso() {
    // SSO typically holds 15 chars for char (sizeof(string)==32 on 64-bit)
    std::string s("short");
    CHECK(s.size() == 5);
    CHECK(!s.empty());

    // exact SSO boundary (depends on sizeof(long_rep)); just verify it works
    std::string t("1234567890123456789012"); // 22 chars — may or may not be long
    CHECK(t.size() == 22);
    CHECK(t[0] == '1');

    // long string
    std::string u("this string is definitely longer than any reasonable SSO buffer size");
    CHECK(u.size() == 68);
    CHECK(u[0] == 't');
    CHECK(u.capacity() >= 68);
}

//-----------------------------------------------------------------------------
// test_string_assign — assign overloads
//-----------------------------------------------------------------------------
void test_string_assign() {
    std::string s;
    s.assign("hello");
    CHECK(s == "hello");

    s.assign(3, 'z');
    CHECK(s == "zzz");

    s.assign("abcdef", 4);
    CHECK(s == "abcd");

    std::string t("world");
    s.assign(t);
    CHECK(s == "world");

    s = "copy_assign";
    CHECK(s.size() == 11);

    s = std::string("move");
    CHECK(s == "move");

    s = 'X';
    CHECK(s == "X");
}

//-----------------------------------------------------------------------------
// test_string_append — append and operator+=
//-----------------------------------------------------------------------------
void test_string_append() {
    std::string s("hello");
    s.append(" world");
    CHECK(s == "hello world");

    s += '!';
    CHECK(s.back() == '!');
    CHECK(s == "hello world!");

    s += " bye";
    CHECK(s.ends_with(" bye"));

    std::string t;
    t.append(5, '*');
    CHECK(t == "*****");

    t.append(s, 0, 5); // "hello"
    CHECK(t == "*****hello");
}

//-----------------------------------------------------------------------------
// test_string_insert — insert overloads
//-----------------------------------------------------------------------------
void test_string_insert() {
    std::string s("helloworld");
    s.insert(5, " ");
    CHECK(s == "hello world");

    s.insert(5, 3, '-');
    // s = "hello world" (len 11), insert 3 '-' at pos 5
    // => "hello" + "---" + " world" = "hello--- world"
    CHECK(s == "hello--- world");

    // iterator insert
    auto it = s.insert(s.begin(), 'X');
    CHECK(*it == 'X');
    CHECK(s[0] == 'X');

    // insert from iterators
    std::string src("ABC");
    s.insert(s.end(), src.begin(), src.end());
    CHECK(s.ends_with("ABC"));
}

//-----------------------------------------------------------------------------
// test_string_erase — erase overloads
//-----------------------------------------------------------------------------
void test_string_erase() {
    std::string s("hello world");
    s.erase(5, 6);  // remove " world"
    CHECK(s == "hello");

    s.erase(s.begin());
    CHECK(s == "ello");

    s.erase(s.begin() + 1, s.end());
    CHECK(s == "e");

    s.erase();
    CHECK(s.empty());
}

//-----------------------------------------------------------------------------
// test_string_replace — replace overloads
//-----------------------------------------------------------------------------
void test_string_replace() {
    std::string s("hello world");
    s.replace(6, 5, "there");
    CHECK(s == "hello there");

    s.replace(0, 5, 3, 'X');
    CHECK(s == "XXX there");

    s.replace(s.begin(), s.begin() + 3, "yy");
    CHECK(s == "yy there");
}

//-----------------------------------------------------------------------------
// test_string_resize_reserve — resize, reserve, shrink_to_fit
//-----------------------------------------------------------------------------
void test_string_resize_reserve() {
    std::string s("hello");
    s.resize(10, '.');
    CHECK(s.size() == 10);
    CHECK(s == "hello.....");

    s.resize(3);
    CHECK(s == "hel");

    s.reserve(100);
    CHECK(s.capacity() >= 100);
    CHECK(s == "hel");

    s.shrink_to_fit();
    CHECK(s == "hel"); // content unchanged
}

//-----------------------------------------------------------------------------
// test_string_access — operator[], at, front, back, data
//-----------------------------------------------------------------------------
void test_string_access() {
    std::string s("abcde");
    CHECK(s[0] == 'a');
    CHECK(s[4] == 'e');
    CHECK(s.front() == 'a');
    CHECK(s.back()  == 'e');
    CHECK(s.at(2)   == 'c');
    CHECK(s.data()[0] == 'a');
    CHECK(s.c_str()[5] == '\0');
    s[0] = 'A';
    CHECK(s.front() == 'A');
}

//-----------------------------------------------------------------------------
// test_string_iterators — begin/end/rbegin/rend
//-----------------------------------------------------------------------------
void test_string_iterators() {
    std::string s("abc");
    std::string r;
    for (auto c : s) r += c;
    CHECK(r == "abc");

    r.clear();
    for (auto it = s.rbegin(); it != s.rend(); ++it)
        r += *it;
    CHECK(r == "cba");
}

//-----------------------------------------------------------------------------
// test_string_compare — compare overloads
//-----------------------------------------------------------------------------
void test_string_compare() {
    std::string a("apple");
    std::string b("banana");
    CHECK(a.compare(b) < 0);
    CHECK(b.compare(a) > 0);
    CHECK(a.compare("apple") == 0);
    CHECK(a.compare(0, 3, "app") == 0);
}

//-----------------------------------------------------------------------------
// test_string_find_family — find, rfind, find_first/last_of, not_of
//-----------------------------------------------------------------------------
void test_string_find_family() {
    std::string s("hello world hello");

    CHECK(s.find("hello") == 0);
    CHECK(s.rfind("hello") == 12);
    CHECK(s.find('o') == 4);
    CHECK(s.rfind('o') == 16);

    CHECK(s.find_first_of("aeiou") == 1); // 'e'
    CHECK(s.find_last_of("aeiou")  == 16); // last 'o' at position 16

    std::string digits("abc123def");
    CHECK(digits.find_first_not_of("abc") == 3);
    CHECK(digits.find_last_not_of("def")  == 5);

    CHECK(s.find("xyz") == std::string::npos);
}

//-----------------------------------------------------------------------------
// test_string_substr — substr
//-----------------------------------------------------------------------------
void test_string_substr() {
    std::string s("hello world");
    std::string sub = s.substr(6, 5);
    CHECK(sub == "world");
    std::string sub2 = s.substr(6);
    CHECK(sub2 == "world");
}

//-----------------------------------------------------------------------------
// test_string_copy — copy
//-----------------------------------------------------------------------------
void test_string_copy() {
    std::string s("hello world");
    char buf[6];
    size_t n = s.copy(buf, 5, 6);
    CHECK(n == 5);
    buf[5] = '\0';
    CHECK(std::string_view(buf, 5) == "world");
}

//-----------------------------------------------------------------------------
// test_string_swap — swap
//-----------------------------------------------------------------------------
void test_string_swap() {
    std::string a("hello");
    std::string b("world");
    a.swap(b);
    CHECK(a == "world");
    CHECK(b == "hello");
    std::swap(a, b);
    CHECK(a == "hello");
    CHECK(b == "world");
}

//-----------------------------------------------------------------------------
// test_string_starts_ends_contains — C++20 predicates
//-----------------------------------------------------------------------------
void test_string_starts_ends_contains() {
    std::string s("hello world");
    CHECK(s.starts_with("hello"));
    CHECK(!s.starts_with("world"));
    CHECK(s.ends_with("world"));
    CHECK(!s.ends_with("hello"));
    CHECK(s.contains("lo wo"));
    CHECK(!s.contains("xyz"));
    CHECK(s.starts_with('h'));
    CHECK(s.ends_with('d'));
    CHECK(s.contains('o'));
}

//-----------------------------------------------------------------------------
// test_string_operators_plus — non-member operator+
//-----------------------------------------------------------------------------
void test_string_operators_plus() {
    std::string a("hello");
    std::string b(" world");
    std::string c = a + b;
    CHECK(c == "hello world");

    std::string d = a + " there";
    CHECK(d == "hello there");

    std::string e = "greet: " + a;
    CHECK(e == "greet: hello");

    std::string f = a + '!';
    CHECK(f == "hello!");

    std::string g = '!' + a;
    CHECK(g == "!hello");
}

//-----------------------------------------------------------------------------
// test_string_comparison_ops — == != < > <= >=
//-----------------------------------------------------------------------------
void test_string_comparison_ops() {
    std::string a("apple");
    std::string b("banana");
    std::string c("apple");

    CHECK(a == c);
    CHECK(!(a == b));
    CHECK(a != b);
    CHECK(a < b);
    CHECK(b > a);
    CHECK(a <= c);
    CHECK(a >= c);
    CHECK(a == "apple");
    CHECK("apple" == a);
}

//-----------------------------------------------------------------------------
// test_string_conversion — stoi, stod, to_string
//-----------------------------------------------------------------------------
void test_string_conversion() {
    std::string s("42");
    CHECK(std::stoi(s) == 42);
    CHECK(std::stol(s) == 42L);
    CHECK(std::stoll(s) == 42LL);
    CHECK(std::stoul(s) == 42UL);
    CHECK(std::stoull(s) == 42ULL);

    std::string f("3.14");
    CHECK(std::stod(f) > 3.13 && std::stod(f) < 3.15);

    std::string ts = std::to_string(12345);
    CHECK(ts == "12345");

    std::string td = std::to_string(3.14159);
    CHECK(!td.empty());

    // with idx
    size_t idx = 0;
    std::string s2("123abc");
    int v = std::stoi(s2, &idx);
    CHECK(v == 123);
    CHECK(idx == 3);
}

//-----------------------------------------------------------------------------
// test_string_literals — operator""s
//-----------------------------------------------------------------------------
void test_string_literals() {
    using namespace std::literals;
    auto s = "hello"s;
    static_assert(std::is_same_v<decltype(s), std::string>);
    CHECK(s == "hello");
    CHECK(s.size() == 5);
}

//-----------------------------------------------------------------------------
// test_string_hash — hash specialisations
//-----------------------------------------------------------------------------
void test_string_hash() {
    std::hash<std::string> h;
    std::string a("hello");
    std::string b("hello");
    std::string c("world");
    CHECK(h(a) == h(b));
    CHECK(h(a) != h(c)); // not guaranteed but virtually certain
}

//-----------------------------------------------------------------------------
// test_string_push_pop_back — push_back, pop_back
//-----------------------------------------------------------------------------
void test_string_push_pop_back() {
    std::string s;
    s.push_back('a');
    s.push_back('b');
    s.push_back('c');
    CHECK(s == "abc");
    s.pop_back();
    CHECK(s == "ab");
}

//-----------------------------------------------------------------------------
// test_string_string_view_ctor — construction from string_view
//-----------------------------------------------------------------------------
void test_string_string_view_ctor() {
    std::string_view sv("hello world", 5);
    std::string s(sv);
    CHECK(s == "hello");
    CHECK(s.size() == 5);
}

//-----------------------------------------------------------------------------
// test_string_init_list — initializer_list ctor and assign
//-----------------------------------------------------------------------------
void test_string_init_list() {
    std::string s({'h','e','l','l','o'});
    CHECK(s == "hello");

    s = {'w','o','r','l','d'};
    CHECK(s == "world");
}

//-----------------------------------------------------------------------------
// test_string_iter_ctor — range construction
//-----------------------------------------------------------------------------
void test_string_iter_ctor() {
    const char src[] = "hello";
    std::string s(src, src + 5);
    CHECK(s == "hello");

    std::string t(s.begin(), s.end());
    CHECK(t == "hello");
}

//-----------------------------------------------------------------------------
// test_string_erase_free_fn — free erase/erase_if
//-----------------------------------------------------------------------------
void test_string_erase_free_fn() {
    std::string s("aabbcc");
    auto n = std::erase(s, 'b');
    CHECK(n == 2);
    CHECK(s == "aacc");

    std::string t("hello world");
    auto m = std::erase_if(t, [](char c){ return c == 'l'; });
    CHECK(m == 3);
    CHECK(t == "heo word");
}

//-----------------------------------------------------------------------------
// test_string_clear — clear
//-----------------------------------------------------------------------------
void test_string_clear() {
    std::string s("hello");
    s.clear();
    CHECK(s.empty());
    CHECK(s.size() == 0);
    CHECK(s.c_str()[0] == '\0');
}

//-----------------------------------------------------------------------------
// test_string_long_string — stress test beyond SSO
//-----------------------------------------------------------------------------
void test_string_long_string() {
    // Build a long string by appending
    std::string s;
    for (int i = 0; i < 200; ++i) s += 'a';
    CHECK(s.size() == 200);
    CHECK(s[0] == 'a');
    CHECK(s[199] == 'a');

    // erase half
    s.erase(100);
    CHECK(s.size() == 100);

    // resize up
    s.resize(150, 'b');
    CHECK(s.size() == 150);
    CHECK(s[100] == 'b');
}
