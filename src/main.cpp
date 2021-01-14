#include <iostream>
#include <string>
#include <vector>

#include "CLI/CLI.hpp"
#include "help_strings.hpp"
#include "parse_error.hpp"
#include "print.hpp"
#include "problem/problem.hpp"
#include "solvers/simple_solver.hpp"

int main(int argc, char **argv) {
    CLI::App app{"Tiler - tool for automated solving of polyomino tiling problems\n"};
    app.require_subcommand(1);

    // solve command definition
    CLI::App *solve_command = app.add_subcommand("solve", "Solve a tiling problem");

    CLI::App *input_group = solve_command->add_option_group("input", "The problem assignment");
    input_group->require_option(1);
    std::vector<std::string> tiles;
    CLI::Option *tiles_option = input_group->add_option(
            "tiles", tiles,
            "Definition of the board and tile shapes.\n" + help_strings::kInputFormats);
    tiles_option->expected(-2);  // at least 2 - one board and one or more tile shapes
    std::string input_file = "";
    CLI::Option *input_file_option =
            input_group->add_option("-f,--file", input_file,
                                    "Path to file containing the problem assignment.\n"
                                    "Same input format, but shapes must be separated\n"
                                    "by an empty line and the optional \"N:\" may be\n"
                                    "on a separate line.");
    input_file_option->check(CLI::ExistingFile);

    bool reflection = false;
    solve_command->add_flag(
            "-r,--allow-reflection", reflection,
            "If present, the solver will be allowed to reflect\n(flip over) the tiles.");

    bool quiet = false;
    solve_command->add_flag(
            "-q,--quiet", quiet,
            "If present, the problem summarization before\nsolving will be silenced.");

    // list command definition
    CLI::App *list_command = app.add_subcommand("list", "List all named tiles");

    // example command definition
    CLI::App *example_command = app.add_subcommand("example", "Show an input example");

    // parse
    std::atexit([]() { print::normal(); });
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        e.get_exit_code() == 0 ? print::help() : print::error();
        return app.exit(e);
    }

    // execute
    CLI::App *command = app.get_subcommands()[0];  // there is always exactly one command
    if (command == solve_command) {
        try {
            Problem problem = tiles.empty() ? Problem::create(input_file, reflection)
                                            : Problem::create(tiles, reflection);
            if (!quiet) {
                print::normal() << problem << std::endl;
            }
            SimpleSolver solver(problem);
            if (solver.solve()) {
                print::success() << "\nTRUE" << std::endl;
            } else {
                print::warning() << "\nFALSE" << std::endl;
            }
        } catch (const ParseError &e) {
            print::error() << e.what() << "\nRun with --help for more information." << std::endl;
            return 1;
        }
    } else if (command == list_command) {
        print::normal() << help_strings::kNamedTilesList;
    } else if (command == example_command) {
        print::normal() << help_strings::kExampleInput;
    }

    return 0;
}
