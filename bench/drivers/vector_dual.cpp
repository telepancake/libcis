// vector_dual.cpp — dual-path (constexpr-capable) erased vector, same exercise.
#include "../experiments/evector/evector_dual.hpp"
#include "../experiments/evector/exercise.hpp"

int main() { volatile long sink = exr::run_all<ev::dvector>(); (void)sink; return 0; }
