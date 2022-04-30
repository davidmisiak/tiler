#define CATCH_CONFIG_RUNNER
#include "catch2/catch.hpp"

#ifdef MINIZINC
#include "minizinc/solver.hh"
#endif

int main(int argc, const char* argv[]) {
#ifdef MINIZINC
    MiniZinc::OverflowHandler::install(argv);
#endif

    int result = Catch::Session().run(argc, argv);
    return result;
}
