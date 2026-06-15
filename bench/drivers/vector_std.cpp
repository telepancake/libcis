// vector_std.cpp — "before": std::vector<T> over many element types.
// Compare against vector_erased.cpp (same exercise, type-erased container).
#include <vector>
#include "../experiments/evector/exercise.hpp"

template<class T> using StdVec = std::vector<T>;

int main() { volatile long sink = exr::run_all<StdVec>(); (void)sink; return 0; }
