// Generic test driver for the AST-transferred libc++ suite.
// transfer.py renamed each test's `int main(...)` to `void test_<slug>()`.
// This driver is compiled once per test with -DTESTFN=test_<slug> and linked
// against it, so we need no per-test generated main file.
extern void TESTFN();
int main() { TESTFN(); return 0; }
