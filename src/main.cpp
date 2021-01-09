#include <iostream>

#include "CLI/CLI.hpp"
#include "rang.hpp"

int main(int argc, char **argv) {
    CLI::App app{"Tiler - tool for automated solving of polyomino tiling problems\n"};

    std::atexit([]() { print::normal(); });
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        e.get_exit_code() == 0 ? print::help() : print::error();
        return app.exit(e);
    }
    return 0;
}
