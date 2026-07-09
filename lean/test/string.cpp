// lean <string> — behaviour + invariants against the one-pointer rep.
//
// No printing; every check traps on failure (lean_test.h).  A >=10k-element
// randomized stress mirrors every structural operation against an independent
// reference model (a plain char array) and CHECKs full equality each step.
//
// Note on element types: basic_string's element is always a character type
// (trivially copyable), so the "move-only / non-trivial element" axis does not
// apply the way it does to vector; instead this suite exercises the 2-byte and
// 4-byte char widths and a custom char_traits (the non-default-Traits path that
// drives the CharT() terminator fix-up).
#include <string>
#include <string_view>
#include <cstring>
#include <cstdint>
#include "lean_test.h"

using namespace std;

// ---------------------------------------------------------------------------
// sizeof contract
// ---------------------------------------------------------------------------
static_assert(sizeof(string)    == sizeof(void*), "string is one pointer");
static_assert(sizeof(wstring)   == sizeof(void*), "wstring is one pointer");
static_assert(sizeof(u16string) == sizeof(void*), "u16string is one pointer");
static_assert(sizeof(u32string) == sizeof(void*), "u32string is one pointer");

// A NUL-terminated string equals the given C string exactly (length + bytes +
// terminator), so c_str() is always safe.
static void check_eq(const string& s, const char* lit) {
  size_t n = __builtin_strlen(lit);
  CHECK(s.size() == n);
  CHECK(s.length() == n);
  CHECK((s.empty()) == (n == 0));
  CHECK(__builtin_memcmp(s.data(), lit, n) == 0);
  CHECK(s.c_str()[n] == '\0');
  CHECK(s == lit);
}

// ---------------------------------------------------------------------------
// Empty-string invariants: shared static rep, capacity 0, no free/realloc.
// ---------------------------------------------------------------------------
void test_empty_invariants() {
  string a;
  string b;
  check_eq(a, "");
  CHECK(a.capacity() == 0);
  CHECK(a.c_str()[0] == '\0');
  // Two default-constructed strings point at the SAME immutable static rep.
  CHECK(a.data() == b.data());
  CHECK(a.c_str() == b.c_str());

  // clear() on an empty string keeps it empty and static (capacity stays 0).
  a.clear();
  CHECK(a.capacity() == 0);
  CHECK(a.data() == b.data());

  // Appending after default construction transitions static -> heap and never
  // touches the shared rep: b stays empty and static.
  a.append("x");
  check_eq(a, "x");
  CHECK(a.capacity() >= 1);
  CHECK(a.data() != b.data());
  check_eq(b, "");
  CHECK(b.data() == string().data());

  // shrink_to_fit / reserve(0) on empty stay static.
  string c;
  c.shrink_to_fit();
  CHECK(c.data() == b.data());
  c.reserve(0);
  CHECK(c.data() == b.data());
}

// ---------------------------------------------------------------------------
// Construction variants
// ---------------------------------------------------------------------------
void test_construct() {
  check_eq(string(), "");
  check_eq(string("abc"), "abc");
  check_eq(string("abcdef", 3), "abc");
  check_eq(string(4, 'z'), "zzzz");
  check_eq(string(0, 'z'), "");
  string src = "hello world";
  check_eq(string(src), "hello world");
  check_eq(string(src, 6), "world");
  check_eq(string(src, 0, 5), "hello");
  check_eq(string(src, 6, 100), "world");
  string_view sv = "view";
  check_eq(string(sv), "view");
  check_eq(string(sv, 1, 2), "ie");
  // iterator range (forward)
  const char* r = "range";
  check_eq(string(r, r + 5), "range");
  // input-iterator-ish: use a string_view iterator pair
  check_eq(string(src.begin(), src.begin() + 5), "hello");
  // initializer_list
  check_eq(string({'i', 'n', 'i', 't'}), "init");
  // long string (heap, forces alloc + capacity)
  string big(1000, 'q');
  CHECK(big.size() == 1000);
  CHECK(big.capacity() >= 1000);
  for (size_t i = 0; i < 1000; ++i) CHECK(big[i] == 'q');
  CHECK(big.c_str()[1000] == '\0');
}

// ---------------------------------------------------------------------------
// Copy / move semantics; moved-from becomes empty static.
// ---------------------------------------------------------------------------
void test_copy_move() {
  string a = "source string that is long enough";
  string b = a;                    // copy
  check_eq(b, "source string that is long enough");
  CHECK(a.data() != b.data());     // deep copy, distinct buffers

  string c = move(a);              // move
  check_eq(c, "source string that is long enough");
  check_eq(a, "");                 // moved-from empty
  CHECK(a.data() == string().data());  // ... and back on the static rep

  string d = "short";
  d = c;                           // copy assign
  check_eq(d, "source string that is long enough");
  d = move(c);                     // move assign
  check_eq(d, "source string that is long enough");
  check_eq(c, "");

  // self copy-assign and self move-assign are no-ops
  d = *&d;
  check_eq(d, "source string that is long enough");
  string& dr = d;
  d = move(dr);
  check_eq(d, "source string that is long enough");

  // assign from char / const char* / initializer_list / string_view
  string e;
  e = 'Q';                 check_eq(e, "Q");
  e = "reset";             check_eq(e, "reset");
  e = {'l', 'i', 's', 't'};check_eq(e, "list");
  string_view sv = "svass";
  e = sv;                  check_eq(e, "svass");
}

// ---------------------------------------------------------------------------
// append / operator+= / push_back — including growth from empty
// ---------------------------------------------------------------------------
void test_append() {
  string s;
  s.append("ab");
  s.append(string("cd"));
  s += "ef";
  s += string("gh");
  s += 'i';
  s.push_back('j');
  s.append(2, '!');
  s.append("XYZmore", 3);
  string t = "0123456789";
  s.append(t, 2, 3);        // "234"
  s.append({'k', 'l'});
  check_eq(s, "abcdefghij!!XYZ234kl");

  // amortized growth from empty: push_back many
  string g;
  const char* base = g.data();  // static rep
  for (int i = 0; i < 5000; ++i) g.push_back(char('a' + (i % 26)));
  CHECK(g.size() == 5000);
  CHECK(g.data() != base);
  for (int i = 0; i < 5000; ++i) CHECK(g[i] == char('a' + (i % 26)));
  CHECK(g.c_str()[5000] == '\0');

  // append(first,last)
  string h = "pre-";
  string add = "TAIL";
  h.append(add.begin(), add.end());
  check_eq(h, "pre-TAIL");
}

// ---------------------------------------------------------------------------
// assign overloads
// ---------------------------------------------------------------------------
void test_assign() {
  string s = "will be replaced entirely";
  s.assign("new");                 check_eq(s, "new");
  s.assign("abcdef", 4);           check_eq(s, "abcd");
  s.assign(5, 'm');                check_eq(s, "mmmmm");
  s.assign(string("STR"));         check_eq(s, "STR");
  string src = "0123456789";
  s.assign(src, 3, 4);             check_eq(s, "3456");
  s.assign(src.begin(), src.begin() + 3); check_eq(s, "012");
  s.assign({'i', 'l'});            check_eq(s, "il");
  s.assign(string_view("svv"));    check_eq(s, "svv");
  // assign shrinking to empty keeps c_str valid
  s.assign("");                    check_eq(s, "");
  CHECK(s.c_str()[0] == '\0');
}

// ---------------------------------------------------------------------------
// insert / erase / replace (index and iterator forms)
// ---------------------------------------------------------------------------
void test_insert_erase_replace() {
  string s = "Hello!";
  s.insert(5, ", world");          check_eq(s, "Hello, world!");
  s.insert(0, ">> ");             check_eq(s, ">> Hello, world!");
  s.insert(3, 3, '*');            check_eq(s, ">> ***Hello, world!");
  s.insert(s.size(), "<<");       check_eq(s, ">> ***Hello, world!<<");

  string i2 = "abcXYdef";
  i2.erase(3, 2);                  check_eq(i2, "abcdef");
  i2.erase(i2.begin() + 1);        check_eq(i2, "acdef");
  i2.erase(i2.begin() + 1, i2.begin() + 3); check_eq(i2, "aef");
  i2.erase();                      check_eq(i2, "");         // erase() default = clear whole

  string r = "the quick brown fox";
  r.replace(4, 5, "SLOW");         check_eq(r, "the SLOW brown fox");
  r.replace(0, 3, "A");            check_eq(r, "A SLOW brown fox");
  r.replace(2, 4, 6, '#');         check_eq(r, "A ###### brown fox");
  // iterator replace
  string r2 = "aabbcc";
  r2.replace(r2.begin() + 2, r2.begin() + 4, "ZZZ"); check_eq(r2, "aaZZZcc");

  // insert(iterator, ...) returns position
  string it = "xz";
  auto p = it.insert(it.begin() + 1, 'y');
  CHECK(*p == 'y');
  check_eq(it, "xyz");
  it.insert(it.end(), 2, '.');
  check_eq(it, "xyz..");

  // replace growing across a reallocation
  string grow = "small";
  grow.replace(1, 1, 500, 'Z');
  CHECK(grow.size() == 504);
  CHECK(grow[0] == 's');
  CHECK(grow[1] == 'Z' && grow[500] == 'Z');
  CHECK(grow[501] == 'a' && grow[503] == 'l');
  CHECK(grow.c_str()[504] == '\0');
}

// ---------------------------------------------------------------------------
// Aliasing: self-append, self-insert, overlapping replace.
// ---------------------------------------------------------------------------
void test_aliasing() {
  // s += s at small size (fits or not) and at a size that forces growth.
  string a = "abc";
  a += a;                          check_eq(a, "abcabc");
  a += a;                          check_eq(a, "abcabcabcabc");
  string b(50, 'x');
  b += b;                          CHECK(b.size() == 100);
  for (char c : b) CHECK(c == 'x');

  // append via a pointer into itself
  string c = "0123456789";
  c.append(c.c_str() + 2, 3);      check_eq(c, "0123456789234");

  // insert a substring of itself
  string d = "ABCDEF";
  d.insert(0, d);                  check_eq(d, "ABCDEFABCDEF");
  string e = "ABCDEF";
  e.insert(2, e.c_str() + 1, 3);   // insert "BCD" at 2
  check_eq(e, "ABBCDCDEF");

  // overlapping replace: replace region reading from an overlapping source.
  // "0123456789": replace(pos=2,n1=3 -> "234") with src=data+1,n2=4 -> "1234"
  // (captured from the original buffer) = "01" + "1234" + "56789".
  string f = "0123456789";
  f.replace(2, 3, f.c_str() + 1, 4);
  check_eq(f, "01123456789");
}

// Recompute the aliasing replace by hand to be certain of the expectation.
void test_aliasing_replace_exact() {
  // start: 0123456789 ; replace(pos=2,n1=3 -> "234") with src=data+1,n2=4 ->
  // source bytes captured from the ORIGINAL buffer = "1234".
  // result = "01" + "1234" + "56789" = "0112345 6789" = "01123456789"? length:
  // 2 + 4 + 5 = 11.
  string f = "0123456789";
  f.replace(2, 3, f.c_str() + 1, 4);
  CHECK(f.size() == 11);
  const char* want = "01123456789";  // "01" | "1234" | "56789"
  CHECK(f.size() == __builtin_strlen(want));
  CHECK(__builtin_memcmp(f.data(), want, f.size()) == 0);

  // self-append through append(first,last) with the string's own iterators
  string g = "wxyz";
  g.append(g.begin(), g.end());
  check_eq(g, "wxyzwxyz");
}

// ---------------------------------------------------------------------------
// resize / reserve / capacity / shrink_to_fit / pointer stability
// ---------------------------------------------------------------------------
void test_capacity() {
  string s = "grow";
  s.resize(8, '.');                check_eq(s, "grow....");
  s.resize(2);                     check_eq(s, "gr");
  s.resize(0);                     check_eq(s, "");
  CHECK(s.c_str()[0] == '\0');

  string r;
  r.reserve(100);
  CHECK(r.capacity() >= 100);
  const char* buf = r.data();
  // Appends that stay within capacity must NOT reallocate: the pointer (hence
  // every reference/iterator) stays stable.
  for (int i = 0; i < 100; ++i) r.push_back('a');
  CHECK(r.data() == buf);
  CHECK(r.size() == 100);

  // reserve smaller than current capacity is a no-op (no shrink).
  size_t cap = r.capacity();
  r.reserve(10);
  CHECK(r.capacity() == cap);
  CHECK(r.data() == buf);

  // shrink_to_fit reduces capacity toward size.
  string big(1000, 'Z');
  big.resize(3);
  big.shrink_to_fit();
  check_eq(big, "ZZZ");
  CHECK(big.capacity() >= 3);
  CHECK(big.capacity() < 1000);

  // shrink_to_fit on an emptied heap string reverts to the static rep.
  string emptyable(500, 'k');
  emptyable.clear();
  emptyable.shrink_to_fit();
  CHECK(emptyable.capacity() == 0);
  CHECK(emptyable.data() == string().data());
  emptyable.append("reuse");       // works after reverting to static
  check_eq(emptyable, "reuse");
}

// ---------------------------------------------------------------------------
// Element access + iterators
// ---------------------------------------------------------------------------
void test_access_iterators() {
  string s = "access";
  CHECK(s.front() == 'a');
  CHECK(s.back() == 's');
  CHECK(s.at(1) == 'c');
  CHECK(s[0] == 'a');
  s[0] = 'A';
  s.at(1) = 'C';
  check_eq(s, "ACcess");
  s.front() = 'x';
  s.back() = 'X';
  check_eq(s, "xCcesX");

  string it = "abcde";
  string fwd;
  for (char c : it) fwd.push_back(c);
  check_eq(fwd, "abcde");
  string rev;
  for (auto r = it.rbegin(); r != it.rend(); ++r) rev.push_back(*r);
  check_eq(rev, "edcba");
  CHECK(it.cbegin() + 5 == it.cend());
  CHECK(*(it.end() - 1) == 'e');

  // copy() out
  char out[8] = {};
  size_t n = it.copy(out, 3, 1);
  CHECK(n == 3);
  CHECK(__builtin_memcmp(out, "bcd", 3) == 0);

  // pop_back
  string pb = "abc";
  pb.pop_back();
  check_eq(pb, "ab");
}

// ---------------------------------------------------------------------------
// substr / swap
// ---------------------------------------------------------------------------
void test_substr_swap() {
  string s = "0123456789";
  check_eq(s.substr(), "0123456789");
  check_eq(s.substr(3), "3456789");
  check_eq(s.substr(3, 4), "3456");
  check_eq(s.substr(10), "");        // pos == size -> empty
  check_eq(s.substr(5, 100), "56789");

  string a = "left";
  string b = "rightside long enough to be heap";
  const char* ad = a.data();
  const char* bd = b.data();
  a.swap(b);
  check_eq(a, "rightside long enough to be heap");
  check_eq(b, "left");
  CHECK(a.data() == bd);
  CHECK(b.data() == ad);
  swap(a, b);
  check_eq(a, "left");
  check_eq(b, "rightside long enough to be heap");
}

// ---------------------------------------------------------------------------
// find family
// ---------------------------------------------------------------------------
void test_find() {
  string s = "the quick brown fox the lazy dog";
  CHECK(s.find("quick") == 4);
  CHECK(s.find("the") == 0);
  CHECK(s.find("the", 1) == 20);
  CHECK(s.find('o') == 12);
  CHECK(s.find('o', 13) == 17);
  CHECK(s.find("nope") == string::npos);
  CHECK(s.find(string("brown")) == 10);
  CHECK(s.find(string_view("lazy")) == 24);

  CHECK(s.rfind("the") == 20);
  CHECK(s.rfind("the", 19) == 0);
  CHECK(s.rfind('o') == 30);
  CHECK(s.rfind("zzz") == string::npos);

  CHECK(s.find_first_of("xz") == 18);  // first 'x' or 'z': 'x' in "fox" at 18
  CHECK(s.find_first_of("aeiou") == 2);
  CHECK(s.find_last_of("aeiou") == 30);
  CHECK(s.find_first_not_of("the ") == 4);
  CHECK(s.find_last_not_of("dog ") == 27);
  CHECK(s.find_first_of("Q") == string::npos);

  // empty-string finds
  string e;
  CHECK(e.find("x") == string::npos);
  CHECK(e.find("") == 0);
  CHECK(e.rfind("") == 0);
  CHECK(s.find("") == 0);
}

// ---------------------------------------------------------------------------
// compare / relational / starts_with / ends_with / contains
// ---------------------------------------------------------------------------
void test_compare() {
  string a = "abc";
  string b = "abd";
  CHECK(a.compare(b) < 0);
  CHECK(b.compare(a) > 0);
  CHECK(a.compare("abc") == 0);
  CHECK(a.compare(0, 2, "ab") == 0);
  CHECK(a.compare(1, 2, string("bc")) == 0);
  CHECK((a < b));
  CHECK((b > a));
  CHECK((a <= string("abc")));
  CHECK((a >= string("abc")));
  CHECK((a == string("abc")));
  CHECK((a != b));
  CHECK((a == "abc"));
  CHECK((a < "abd"));
  CHECK(("abc" == a));
  CHECK((a <=> b) < 0);
  CHECK((a <=> string("abc")) == 0);

  string s = "prefix-middle-suffix";
  CHECK(s.starts_with("prefix"));
  CHECK(s.starts_with('p'));
  CHECK(s.starts_with(string_view("prefix-")));
  CHECK(!s.starts_with("Prefix"));
  CHECK(s.ends_with("suffix"));
  CHECK(s.ends_with('x'));
  CHECK(!s.ends_with("Suffix"));
  CHECK(s.contains("middle"));
  CHECK(s.contains('-'));
  CHECK(s.contains(string_view("fix-mid")));
  CHECK(!s.contains("zzz"));
}

// ---------------------------------------------------------------------------
// operator+ (all combinations)
// ---------------------------------------------------------------------------
void test_concat() {
  string a = "foo";
  string b = "bar";
  check_eq(a + b, "foobar");
  check_eq(a + "X", "fooX");
  check_eq("X" + a, "Xfoo");
  check_eq(a + '!', "foo!");
  check_eq('!' + a, "!foo");
  check_eq(string("l") + string("r"), "lr");
  check_eq(string("m") + move(b), "mbar");
  check_eq(move(a) + string("n"), "foon");
  // rvalue chains
  check_eq(string("a") + "b" + string("c") + 'd', "abcd");
}

// ---------------------------------------------------------------------------
// erase / erase_if free functions
// ---------------------------------------------------------------------------
void test_erase_free() {
  string s = "aXbXcXd";
  size_t n = erase(s, 'X');
  CHECK(n == 3);
  check_eq(s, "abcd");
  string t = "a1b2c3";
  size_t m = erase_if(t, [](char c) { return c >= '0' && c <= '9'; });
  CHECK(m == 3);
  check_eq(t, "abc");
}

// ---------------------------------------------------------------------------
// to_string / sto* round trips
// ---------------------------------------------------------------------------
void test_numeric() {
  check_eq(to_string(0), "0");
  check_eq(to_string(12345), "12345");
  check_eq(to_string(-42), "-42");
  check_eq(to_string((unsigned)7), "7");
  check_eq(to_string(123456789012345LL), "123456789012345");
  CHECK(stoi("42") == 42);
  CHECK(stoi("-17") == -17);
  CHECK(stol("1000000") == 1000000L);
  CHECK(stoul("4000000000") == 4000000000UL);
  CHECK(stoll("9000000000") == 9000000000LL);
  CHECK(stoull("18000000000") == 18000000000ULL);
  size_t idx = 0;
  CHECK(stoi("123abc", &idx) == 123);
  CHECK(idx == 3);
  CHECK(stoi("ff", nullptr, 16) == 255);
  double d = stod("3.5");
  CHECK(d > 3.49 && d < 3.51);
  float f = stof("2.25");
  CHECK(f > 2.24f && f < 2.26f);
  // round trip
  CHECK(stoll(to_string(987654321LL)) == 987654321LL);
}

// ---------------------------------------------------------------------------
// literals + hash
// ---------------------------------------------------------------------------
void test_literals_hash() {
  using namespace std::literals;
  auto s = "hi"s;
  check_eq(s, "hi");
  auto w = L"wide"s;
  CHECK(w.size() == 4);

  hash<string> h;
  string a = "hash me please, a reasonably long string";
  string b = a;
  CHECK(h(a) == h(b));
  string c = "different";
  // Not a strict guarantee, but these two differ under any sane hash.
  CHECK(h(a) != h(c));
}

// ---------------------------------------------------------------------------
// Wide / 16-bit / 32-bit char widths (byte kernel scaled by sizeof)
// ---------------------------------------------------------------------------
template<class S, class C>
static void width_suite(const C* lit3 /*len 3*/, C x) {
  S s;
  CHECK(s.size() == 0);
  CHECK(s.capacity() == 0);
  s.append(lit3);
  CHECK(s.size() == 3);
  CHECK(s[0] == lit3[0] && s[2] == lit3[2]);
  CHECK(s.c_str()[3] == C());
  for (int i = 0; i < 3000; ++i) s.push_back(x);
  CHECK(s.size() == 3003);
  for (int i = 3; i < 3003; ++i) CHECK(s[i] == x);
  CHECK(s.c_str()[3003] == C());
  S t = s;                 // deep copy
  CHECK(t == s);
  CHECK(t.data() != s.data());
  s.insert(0, lit3);
  CHECK(s.size() == 3006);
  CHECK(s[0] == lit3[0]);
  s.erase(0, 3);
  CHECK(s == t);
  S self = t;
  self += self;
  CHECK(self.size() == t.size() * 2);
}

void test_wide_widths() {
  width_suite<wstring, wchar_t>(L"abc", L'Z');
  width_suite<u16string, char16_t>(u"abc", u'Z');
  width_suite<u32string, char32_t>(U"abc", U'Z');
}

// ---------------------------------------------------------------------------
// Custom char_traits: exercises the non-default-Traits path (terminator fix-up)
// and confirms Traits-driven comparison/search still route through Traits.
// ---------------------------------------------------------------------------
struct ci_traits : char_traits<char> {
  static char lower(char c) { return (c >= 'A' && c <= 'Z') ? char(c + 32) : c; }
  static bool eq(char a, char b) { return lower(a) == lower(b); }
  static bool lt(char a, char b) { return lower(a) < lower(b); }
  static int compare(const char* a, const char* b, size_t n) {
    for (size_t i = 0; i < n; ++i) {
      if (lt(a[i], b[i])) return -1;
      if (lt(b[i], a[i])) return 1;
    }
    return 0;
  }
  static const char* find(const char* s, size_t n, char c) {
    for (size_t i = 0; i < n; ++i)
      if (eq(s[i], c)) return s + i;
    return nullptr;
  }
};

void test_custom_traits() {
  using cistring = basic_string<char, ci_traits>;
  static_assert(sizeof(cistring) == sizeof(void*), "custom-traits string still one pointer");
  cistring s = "Hello";
  s.append(", World");
  CHECK(s.size() == 12);
  CHECK(s.c_str()[12] == '\0');            // terminator fix-up wrote CharT()
  CHECK(__builtin_memcmp(s.data(), "Hello, World", 12) == 0);
  // case-insensitive comparison via Traits::compare
  cistring a = "ABC";
  cistring b = "abc";
  CHECK(a.compare(b) == 0);
  CHECK((a == b));
  // grow past a reallocation and keep the terminator correct
  for (int i = 0; i < 500; ++i) s.push_back('q');
  CHECK(s.size() == 512);
  CHECK(s.c_str()[512] == '\0');
  CHECK(s.find('w') == 7);                 // case-insensitive find of 'W'
}

// ---------------------------------------------------------------------------
// npos / boundary edge cases
// ---------------------------------------------------------------------------
void test_npos_edges() {
  string s = "edge";
  CHECK(string::npos == size_t(-1));
  check_eq(s.substr(s.size()), "");         // pos == size
  s.erase(s.size());                        // erase at end: no-op
  check_eq(s, "edge");
  s.erase(2);                               // erase to end
  check_eq(s, "ed");
  s.append("ge");
  s.replace(2, string::npos, "!");          // n1 clamped to remaining
  check_eq(s, "ed!");
  string big = "0123456789";
  CHECK(big.find_last_of("0") == 0);
  CHECK(big.find_first_of("9") == 9);
  // insert/erase with n == npos-ish large clamp via substrings
  string sub = "0123456789";
  string dst = "[]";
  dst.insert(1, sub, 3);                    // insert sub[3:] = "3456789"
  check_eq(dst, "[3456789]");
}

// ---------------------------------------------------------------------------
// >= 10k randomized stress mirrored against a plain-array reference model.
// ---------------------------------------------------------------------------
namespace {
uint32_t rng = 0x9e3779b9u;
uint32_t rnd() { rng = rng * 1664525u + 1013904223u; return rng; }

struct Ref {
  static constexpr size_t CAP = 200000;
  char buf[CAP];
  size_t len = 0;
  void ins(size_t pos, const char* s, size_t n) {
    __builtin_memmove(buf + pos + n, buf + pos, len - pos);
    __builtin_memcpy(buf + pos, s, n);
    len += n;
  }
  void del(size_t pos, size_t n) {
    __builtin_memmove(buf + pos, buf + pos + n, len - pos - n);
    len -= n;
  }
  void rep(size_t pos, size_t nd, const char* s, size_t na) {
    __builtin_memmove(buf + pos + na, buf + pos + nd, len - pos - nd);
    __builtin_memcpy(buf + pos, s, na);
    len = len - nd + na;
  }
};
} // namespace

void test_stress() {
  static Ref ref;      // large; keep off the stack
  string s;
  ref.len = 0;
  s.clear();

  char scratch[512];
  bool grew_past_10k = false;

  for (int iter = 0; iter < 40000; ++iter) {
    // Occasionally reset to keep the size bounded but still churn a lot.
    if (ref.len > 60000) {
      size_t keep = rnd() % 20000;
      if (keep > ref.len) keep = ref.len;
      s.erase(keep);
      ref.del(keep, ref.len - keep);
    }
    if (ref.len > 10000) grew_past_10k = true;

    uint32_t op = rnd() % 7;
    // Bias toward growth while small so the run reliably climbs past 10k; the
    // full op mix (including the shrinking erase/replace/pop paths) runs above.
    if (ref.len < 12000) {
      uint32_t g = rnd() % 3;
      op = g == 0 ? 0 : (g == 1 ? 3 : 6);  // append / insert / fill-insert
    }
    switch (op) {
      case 0: {  // append random bytes
        size_t n = rnd() % 200;
        for (size_t i = 0; i < n; ++i) scratch[i] = char('!' + (rnd() % 90));
        s.append(scratch, n);
        __builtin_memcpy(ref.buf + ref.len, scratch, n);
        ref.len += n;
        break;
      }
      case 1: {  // push_back
        char c = char('!' + (rnd() % 90));
        s.push_back(c);
        ref.buf[ref.len++] = c;
        break;
      }
      case 2: {  // pop_back / erase-at-end
        if (ref.len > 0) {
          s.pop_back();
          ref.len--;
        }
        break;
      }
      case 3: {  // insert
        size_t pos = ref.len ? (rnd() % (ref.len + 1)) : 0;
        size_t n = rnd() % 100;
        for (size_t i = 0; i < n; ++i) scratch[i] = char('!' + (rnd() % 90));
        s.insert(pos, scratch, n);
        ref.ins(pos, scratch, n);
        break;
      }
      case 4: {  // erase a random range
        if (ref.len > 0) {
          size_t pos = rnd() % ref.len;
          size_t n = rnd() % (ref.len - pos + 1);
          s.erase(pos, n);
          ref.del(pos, n);
        }
        break;
      }
      case 5: {  // replace a random range with random bytes
        if (ref.len > 0) {
          size_t pos = rnd() % ref.len;
          size_t nd = rnd() % (ref.len - pos + 1);
          size_t na = rnd() % 120;
          for (size_t i = 0; i < na; ++i) scratch[i] = char('!' + (rnd() % 90));
          s.replace(pos, nd, scratch, na);
          ref.rep(pos, nd, scratch, na);
        }
        break;
      }
      case 6: {  // fill insert (n copies of a char)
        size_t pos = ref.len ? (rnd() % (ref.len + 1)) : 0;
        size_t n = rnd() % 80;
        char c = char('0' + (rnd() % 10));
        s.insert(pos, n, c);
        __builtin_memmove(ref.buf + pos + n, ref.buf + pos, ref.len - pos);
        __builtin_memset(ref.buf + pos, c, n);
        ref.len += n;
        break;
      }
    }

    // Full equality against the reference model, plus the NUL invariant.
    CHECK(s.size() == ref.len);
    CHECK(__builtin_memcmp(s.data(), ref.buf, ref.len) == 0);
    CHECK(s.c_str()[ref.len] == '\0');
    // capacity is always enough to hold the content plus terminator.
    CHECK(s.capacity() >= s.size());
  }
  CHECK(grew_past_10k);   // the run really did exceed 10k elements

  // A final round-trip through copy, move and shrink keeps the content.
  string copy = s;
  CHECK(copy == s);
  CHECK(copy.size() == ref.len);
  CHECK(__builtin_memcmp(copy.data(), ref.buf, ref.len) == 0);
  string moved = move(copy);
  CHECK(moved == s);
  check_eq(string(), "");
  moved.shrink_to_fit();
  CHECK(moved.size() == ref.len);
  CHECK(__builtin_memcmp(moved.data(), ref.buf, ref.len) == 0);
  CHECK(moved.c_str()[ref.len] == '\0');
}

int main() {
  test_empty_invariants();
  test_construct();
  test_copy_move();
  test_append();
  test_assign();
  test_insert_erase_replace();
  test_aliasing();
  test_aliasing_replace_exact();
  test_capacity();
  test_access_iterators();
  test_substr_swap();
  test_find();
  test_compare();
  test_concat();
  test_erase_free();
  test_numeric();
  test_literals_hash();
  test_wide_widths();
  test_custom_traits();
  test_npos_edges();
  test_stress();
  return 0;
}
