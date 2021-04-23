#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "CLI/CLI.hpp"
#include "help_strings.hpp"
#include "parse_error.hpp"
#include "print.hpp"
#include "problem/problem.hpp"
#include "solution/solution.hpp"
#include "solvers/simple_solver.hpp"
#include "solvers/solver.hpp"

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

    std::string image_file = "";
    solve_command->add_option("-s,--save", image_file,
                              "Path to file where the solution (if it exists)\n"
                              "will be saved as an SVG image. If the file exists,\n"
                              "it will be overwritten.");

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
            Problem problem = tiles.empty() ? Problem::create_from_file(input_file, reflection)
                                            : Problem::create(tiles, reflection);
            if (!quiet) {
                print::normal() << problem << std::endl;
            }
            std::unique_ptr<Solver> solver = std::make_unique<SimpleSolver>(problem);
            Solution solution = solver->solve();
            if (solution.empty()) {
                print::warning() << "FALSE" << std::endl;
            } else {
                print::success() << "TRUE" << std::endl;
                if (!quiet) {
                    print::normal() << solution;
                }
                if (!image_file.empty()) {
                    solution.save_image(image_file, problem);
                }
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
