// vector_erased.cpp — "after": the type-erased evector<T> over the same types.
#include "../experiments/evector/evector.hpp"
#include "../experiments/evector/exercise.hpp"

int main() { volatile long sink = exr::run_all<ev::evector>(); (void)sink; return 0; }
