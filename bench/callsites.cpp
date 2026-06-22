// callsites.cpp — representative call sites, one __noinline__ function each.
//
// Each function's whole compiled body IS the per-call code emitted at a use of
// that operation: the size of the symbol is the per-call code size, and its
// disassembly is the call site itself. The harness compiles this per target
// architecture (x86_64, i586, arm32, arm64, mips32) and for two libraries —
// libcis and the non-type-erased reference (the host's libstdc++) — then:
//   * reports size(libcis) - size(reference) as the per-call OVERHEAD, per arch;
//   * writes the source-annotated disassembly of every call site into the
//     results dir, so a change's effect on generated code is staring you in the
//     face, not buried behind a number.
//
// extern "C" gives stable, un-mangled symbol names (cs_*) so both the tooling
// and a human can find each call site without demangling.

#include <vector>
#include <string>
#include <algorithm>

struct H {                       // non-trivially-relocatable, heavy element
    char pad[7];
    std::string s;
    H() : s("x") {}
    bool operator<(const H& o) const { return s < o.s; }
};

#define CS extern "C" __attribute__((noinline, used))

CS void cs_vec_push_int (std::vector<int>* v, int x)            { v->push_back(x); }
CS void cs_vec_push_H   (std::vector<H>* v, const H* x)         { v->push_back(*x); }
CS void cs_vec_insert_H (std::vector<H>* v, const H* x)         { v->insert(v->begin() + 1, *x); }
CS void cs_vec_erase_H  (std::vector<H>* v)                     { v->erase(v->begin() + 1); }
CS void cs_vec_assign_H (std::vector<H>* a, std::vector<H>* b)  { *a = *b; }
CS void cs_vec_sort_H   (std::vector<H>* v)                     { std::sort(v->begin(), v->end()); }
CS void cs_str_append   (std::string* s, const char* p)        { *s += p; }
CS int  cs_str_compare  (std::string* a, std::string* b)       { return a->compare(*b); }
