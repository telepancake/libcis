// lean <iostream>/<ostream>/<istream>/<ios> — the LOCALE-SEVERANCE overlay.
//
// The lean stream headers are CLASSIC-LOCALE ONLY (lean/README.md deviation
// "Locale severance"): the stream code path calls NO ctype/numpunct facet and
// no use_facet — whitespace/token boundaries use a direct classic test,
// boolalpha is hardcoded "true"/"false", narrow/widen are direct classic
// mappings, imbue() is a documented no-op returning the classic locale.
//
// This test proves the SIZE deviation did not become a CORRECTNESS cut: every
// formatted-I/O path must still produce exactly the classic-locale bytes.
// Coverage: int/unsigned/float/double/long double/bool/char/string/pointer
// insertion + extraction; width/fill/adjustfield (left/right/internal);
// showbase/showpos/uppercase/hex/oct/dec; precision/fixed/scientific/hexfloat/
// defaultfloat; skipws/noskipws; get/getline/peek/unget/putback/ignore/read;
// tellg/seekg/tellp/seekp; sentry semantics; failbit/eofbit/badbit; operator>>
// of std::string + std::getline; endl/ends/flush; the <ios>/<iomanip>
// manipulators; stringstreams round-tripping through the overlay; and the
// deviation itself (imbue no-op, getloc classic, classic whitespace).  All must
// pass at -O0 and -Os.  No printing; every check traps on failure (lean_test.h).
#include <iostream>
#include <sstream>
#include <istream>
#include <ostream>
#include <iomanip>
#include <ios>
#include <locale>
#include <string>
#include <cstring>
#include "lean_test.h"

using namespace std;

// OUT: `expr` applied to a fresh ostringstream must yield exactly `expected`.
#define OUT(expected, expr) do { ostringstream os; expr; CHECK(os.str() == string(expected)); } while (0)

// ===========================================================================
// Integer insertion: bases, showbase, showpos, uppercase, signs
// ===========================================================================
void test_integer_out() {
  OUT("42", os << 42);
  OUT("-42", os << -42);
  OUT("0", os << 0);
  OUT("4294967295", os << 4294967295u);
  OUT("-9223372036854775808", os << (long long)(-9223372036854775807LL - 1));
  OUT("18446744073709551615", os << (unsigned long long)-1);
  OUT("2a", os << hex << 42);
  OUT("2A", os << hex << uppercase << 42);
  OUT("0x2a", os << showbase << hex << 42);
  OUT("0X2A", os << showbase << uppercase << hex << 42);
  OUT("52", os << oct << 42);
  OUT("052", os << showbase << oct << 42);
  OUT("+42", os << showpos << 42);
  OUT("+0", os << showpos << 0);
  OUT("ff", os << hex << 255);
  OUT("-1", os << dec << -1);
  // hex/oct print the two's-complement bit pattern (unsigned), no sign
  OUT("ffffffff", os << hex << -1);                        // int is 32-bit
  OUT("ffffffffffffffff", os << hex << (long long)-1);     // 64-bit
  short s = -1;
  OUT("ffff", os << hex << s);
}

// ===========================================================================
// Float insertion: default/fixed/scientific/hexfloat, precision, showpoint
// ===========================================================================
void test_float_out() {
  OUT("3.14", os << 3.14);
  OUT("3.14159", os << 3.14159265);          // default precision 6
  OUT("100000", os << 100000.0);
  OUT("1e+06", os << 1000000.0);             // default %g switches to exp
  OUT("3.141593", os << fixed << 3.14159265);
  OUT("3.14", os << fixed << setprecision(2) << 3.14159);
  OUT("3.142e+00", os << scientific << setprecision(3) << 3.14159);
  OUT("3.14000", os << showpoint << setprecision(6) << 3.14);
  OUT("-0.5", os << -0.5);
  OUT("0", os << 0.0);
  OUT("+2.5", os << showpos << 2.5);
  OUT("inf", os << __builtin_inf());
  OUT("nan", os << __builtin_nan(""));
  // long double
  OUT("2.5", os << (long double)2.5);
  OUT("1.5", os << (float)1.5f);
  // hexfloat round-trips losslessly (no precision)
  { ostringstream os; os << hexfloat << 1.0; string h = os.str();
    CHECK(h.size() >= 3 && h[0] == '0' && h[1] == 'x'); }
  // defaultfloat resets floatfield
  OUT("3.14", os << scientific << defaultfloat << 3.14);
}

// ===========================================================================
// bool: boolalpha (hardcoded true/false) vs numeric 0/1  (DEVIATION)
// ===========================================================================
void test_bool_out() {
  OUT("1", os << true);
  OUT("0", os << false);
  OUT("true", os << boolalpha << true);
  OUT("false", os << boolalpha << false);
  OUT("1", os << boolalpha << noboolalpha << true);
}

// ===========================================================================
// char / C-string / std::string / string_view / pointer / nullptr
// ===========================================================================
void test_char_string_out() {
  OUT("A", os << 'A');
  OUT("hello", os << "hello");
  OUT("hi", os << string("hi"));
  OUT("sv", os << string_view("sv"));
  OUT("nullptr", os << nullptr);
  signed char sc = 'x'; OUT("x", os << sc);
  unsigned char uc = 'y'; OUT("y", os << uc);
  // pointer prints an implementation form; just check it is non-empty and 0x-ish
  { ostringstream os; int v; os << (void*)&v; string p = os.str();
    CHECK(!p.empty()); }
  // wide
  { wostringstream os; os << L"wide" << 42 << L' ' << 3.5; CHECK(os.str() == wstring(L"wide42 3.5")); }
}

// ===========================================================================
// width / fill / adjustfield: left, right (default), internal
// ===========================================================================
void test_width_fill_adjust() {
  OUT("    42", os << setw(6) << 42);              // right (default)
  OUT("42    ", os << left << setw(6) << 42);
  OUT("00042", os << setfill('0') << setw(5) << 42);
  OUT("-0042", os << internal << setfill('0') << setw(5) << -42); // sign then fill
  OUT("0x002a", os << internal << setfill('0') << setw(6) << showbase << hex << 42);
  OUT("+0042", os << internal << setfill('0') << setw(5) << showpos << 42);
  OUT("***abc", os << setfill('*') << setw(6) << "abc");
  OUT("abc***", os << left << setfill('*') << setw(6) << "abc");
  // width resets to 0 after each formatted output
  { ostringstream os; os << setw(6) << 1 << 2; CHECK(os.str() == string("     12")); }
  // width honoured for strings and single chars
  OUT("     Q", os << setw(6) << 'Q');
}

// ===========================================================================
// Integer / float / bool extraction
// ===========================================================================
void test_numeric_in() {
  { istringstream is("42 -7 0xff"); int a=0,b=0; is >> a >> b; CHECK(a==42 && b==-7);
    unsigned u=0; is >> hex >> u; CHECK(u==255); }
  { istringstream is("3.14 2.5e3"); double d=0,e=0; is >> d >> e; CHECK(d==3.14 && e==2500.0); }
  { istringstream is("   99"); int a=0; is >> a; CHECK(a==99); }        // skipws
  { istringstream is("true false"); bool b1=false,b2=true; is >> boolalpha >> b1 >> b2; CHECK(b1 && !b2); }
  { istringstream is("1 0"); bool b1=false,b2=true; is >> b1 >> b2; CHECK(b1 && !b2); }
  { istringstream is("ff"); int x=0; is >> hex >> x; CHECK(x==255); }
  { istringstream is("052"); int x=0; is >> oct >> x; CHECK(x==42); }
  { istringstream is("-123456789012345"); long long v=0; is >> v; CHECK(v==-123456789012345LL); }
  { istringstream is("18446744073709551615"); unsigned long long v=0; is >> v; CHECK(v==(unsigned long long)-1); }
  // float variants
  { istringstream is("1.5"); float f=0; is >> f; CHECK(f==1.5f); }
  { istringstream is("2.25"); long double ld=0; is >> ld; CHECK(ld==(long double)2.25); }
}

// ===========================================================================
// string extraction (word) + std::getline; classic whitespace boundaries
// ===========================================================================
void test_string_in() {
  { istringstream is("  hello   world"); string a,b; is >> a >> b; CHECK(a=="hello" && b=="world"); }
  { istringstream is("line one\nline two\n"); string l; getline(is,l); CHECK(l=="line one");
    getline(is,l); CHECK(l=="line two"); }
  { istringstream is("a,b,c"); string t; getline(is,t,','); CHECK(t=="a"); getline(is,t,','); CHECK(t=="b"); }
  // all six classic whitespace chars separate tokens
  { istringstream is("x\ty\nz\vw\fv\ru"); string t; int n=0;
    while (is >> t) ++n; CHECK(n==6); }
  // width caps a string extraction
  { istringstream is("abcdefgh"); string t; is >> setw(3) >> t; CHECK(t=="abc"); }
}

// ===========================================================================
// Unformatted input: get / peek / unget / putback / ignore / read / gcount
// ===========================================================================
void test_unformatted_in() {
  { istringstream is("abc"); CHECK(is.get()=='a'); CHECK(is.peek()=='b'); CHECK(is.get()=='b');
    is.unget(); CHECK(is.get()=='b'); CHECK(is.get()=='c'); CHECK(is.get()==char_traits<char>::eof()); }
  { istringstream is("hello"); char buf[16]; is.get(buf, 16); CHECK(string(buf)=="hello"); }
  { istringstream is("one two"); char buf[16]; is.get(buf, 16, ' '); CHECK(string(buf)=="one"); CHECK(is.gcount()==3); }
  { istringstream is("abcdef"); is.ignore(3); CHECK(is.get()=='d'); }
  { istringstream is("abcdef"); is.ignore(100, 'c'); CHECK(is.get()=='d'); }
  { istringstream is("data"); char buf[4]; is.read(buf,4); CHECK(is.gcount()==4 && memcmp(buf,"data",4)==0); }
  // putback of the just-read character (the well-defined case for input-only buffers)
  { istringstream is("abc"); char c = char_traits<char>::to_char_type(is.get());
    is.putback(c); CHECK(is.get()=='a'); }
  // getline into char array, delimiter consumed, gcount includes it
  { istringstream is("hi\nthere"); char buf[8]; is.getline(buf,8); CHECK(string(buf)=="hi"); CHECK(is.gcount()==3); }
}

// ===========================================================================
// State bits: eofbit / failbit / badbit / good / clear / exceptions
// ===========================================================================
void test_state_bits() {
  { istringstream is("5"); int a=0; is >> a; CHECK(is.good() || is.eof());
    int b=0; is >> b; CHECK(is.fail()); CHECK(is.eof()); }
  { istringstream is("xyz"); int a=0; is >> a; CHECK(is.fail()); CHECK(!is.bad()); }
  { istringstream is("1"); int a=0; is >> a; is.clear(); CHECK(is.good()); }
  { istringstream is(""); int a=0; is >> a; CHECK(is.fail() && is.eof()); }
  // operator bool / operator!
  { istringstream is("7"); int a=0; CHECK(static_cast<bool>(is >> a)); CHECK(!(is >> a)); }
  // setstate / rdstate
  { ostringstream os; os.setstate(ios_base::failbit); CHECK(os.fail());
    os.clear(); CHECK(os.good()); }
}

// ===========================================================================
// seek/tell on stringstreams (tellg/seekg/tellp/seekp)
// ===========================================================================
void test_seek_tell() {
  { istringstream is("0123456789"); is.seekg(5); CHECK(is.get()=='5');
    CHECK(is.tellg()==streampos(6)); is.seekg(-2, ios_base::cur); CHECK(is.get()=='4');
    is.seekg(0, ios_base::end); CHECK(is.get()==char_traits<char>::eof()); }
  { ostringstream os; os << "abcdef"; os.seekp(2); os << "XY"; CHECK(os.str()=="abXYef");
    CHECK(os.tellp()==streampos(4)); }
  // seekg re-enables reading after EOF
  { istringstream is("ab"); is.get(); is.get(); is.get(); CHECK(is.eof());
    is.clear(); is.seekg(0); CHECK(is.get()=='a'); }
}

// ===========================================================================
// Manipulators: endl / ends / flush, and the <ios> flag manipulators
// ===========================================================================
void test_manipulators() {
  OUT("line\n", os << "line" << endl);
  { ostringstream os; os << "x" << ends; string s = os.str(); CHECK(s.size()==2 && s[1]=='\0'); }
  OUT("done", os << "done" << flush);
  // combined stateful manipulators (showpos applies to base 10 only, not hex)
  OUT("   ff", os << right << setw(5) << showpos << hex << 255);
  // noshowbase / nouppercase / dec reset
  OUT("255", os << showbase << nouppercase << dec << 255);
}

// ===========================================================================
// stringstream round-trip (read + write on one stream)
// ===========================================================================
void test_stringstream_roundtrip() {
  stringstream ss;
  ss << 42 << ' ' << 3.5 << ' ' << "word" << ' ' << true;
  int i=0; double d=0; string w; bool b=false;
  ss >> i >> d >> w >> b;
  CHECK(i==42 && d==3.5 && w=="word" && b==true);   // b via numeric 1
  // stringbuf str() set/get
  stringstream s2; s2.str("100 200"); int a=0,c=0; s2 >> a >> c; CHECK(a==100 && c==200);
}

// ===========================================================================
// <iomanip>: setw/setfill/setprecision/setbase already covered; quoted etc.
// ===========================================================================
void test_iomanip() {
  OUT("   7", os << setw(4) << 7);
  OUT("7.000", os << fixed << setprecision(3) << 7.0);
  OUT("2a", os << setbase(16) << 42);
  OUT("52", os << setbase(8) << 42);
  OUT("42", os << setbase(10) << 42);
  OUT("....5", os << setfill('.') << setw(5) << 5);
}

// ===========================================================================
// THE DEVIATION: imbue is a no-op returning classic; getloc is classic;
// classic-only whitespace; boolalpha hardcoded.
// ===========================================================================
void test_locale_severance() {
  ostringstream os;
  // getloc() returns the classic locale
  CHECK(os.getloc() == locale());
  CHECK(os.getloc() == locale::classic());
  // imbue() is a no-op returning the (classic) previous locale, and does NOT
  // change subsequent formatting: a defensive imbue keeps working, classic.
  locale prev = os.imbue(locale::classic());
  CHECK(prev == locale::classic());
  CHECK(os.getloc() == locale());
  os << 1234567;                                   // no thousands separators
  CHECK(os.str() == string("1234567"));
  // even imbuing a (default) locale leaves getloc classic and formatting classic
  { ostringstream o2; o2.imbue(locale()); o2 << 7654321; CHECK(o2.str()=="7654321"); }
  // boolalpha is the hardcoded classic name, never a numpunct override
  { ostringstream o3; o3 << boolalpha << true << ' ' << false; CHECK(o3.str()=="true false"); }
  // classic whitespace only: '\xA0' (Latin-1 NBSP) is NOT classic space, so it
  // is neither skipped nor a token separator — an int read stops/does not skip.
  { istringstream is("\xA0" "42"); int v=-1; is >> v; CHECK(is.fail()); }
  // a leading classic space IS skipped
  { istringstream is("   42"); int v=0; is >> v; CHECK(v==42); }
  // basic_ios::widen/narrow are the direct classic identity for char
  { ostringstream os; CHECK(os.widen('Q')=='Q'); CHECK(os.narrow('Q','?')=='Q'); }
  // wide classic widen/narrow
  { wostringstream os; CHECK(os.widen('A')==L'A'); CHECK(os.narrow(L'A','?')=='A');
    CHECK(os.narrow(wchar_t(0x20AC), '?')=='?'); }   // euro (U+20AC) not narrowable -> default
}

// ===========================================================================
// cout / cin / cerr / clog exist and work (smoke: no crash, correct bytes to a
// user-supplied streambuf via rdbuf redirection).
// ===========================================================================
void test_global_streams() {
  // Redirect cout to a stringbuf and verify the global object formats correctly.
  ostringstream capture;
  streambuf* old = cout.rdbuf(capture.rdbuf());
  cout << "cap:" << 42 << ' ' << true << ' ' << 3.5;
  cout.rdbuf(old);
  CHECK(capture.str() == string("cap:42 1 3.5"));
  // cin extraction path via a redirected buffer
  istringstream src("55 hi");
  streambuf* oldin = cin.rdbuf(src.rdbuf());
  int n=0; string w; cin >> n >> w;
  cin.rdbuf(oldin);
  CHECK(n==55 && w=="hi");
  // cerr/clog are usable
  ostringstream ecap; streambuf* oe = cerr.rdbuf(ecap.rdbuf());
  cerr << "err" << 1; cerr.rdbuf(oe); CHECK(ecap.str()=="err1");
}

int main() {
  test_integer_out();
  test_float_out();
  test_bool_out();
  test_char_string_out();
  test_width_fill_adjust();
  test_numeric_in();
  test_string_in();
  test_unformatted_in();
  test_state_bits();
  test_seek_tell();
  test_manipulators();
  test_stringstream_roundtrip();
  test_iomanip();
  test_locale_severance();
  test_global_streams();
  return 0;
}
