#include <iostream>

#include "CLI/CLI.hpp"
#include "rang.hpp"

int main(int argc, char **argv) {
    CLI::App app{"Tiler - tool for automated solving of polyomino tiling problems\n"};

    std::atexit([]() {
        std::cout << rang::style::reset;
        std::cerr << rang::style::reset;
    });
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        if (e.get_exit_code() == 0) {
            std::cout << rang::fg::blue;
        } else {
            std::cerr << rang::fg::red;
        }
        return app.exit(e);
    }
    return 0;
}
