#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "CLI/CLI.hpp"
#include "help_strings.hpp"
#include "parse_error.hpp"
#include "parsers/problem_parser.hpp"
#include "print.hpp"
#include "problem/problem.hpp"
#include "solution/solution.hpp"
#include "solve_error.hpp"
#include "solvers/solver.hpp"
#include "solvers/solver_factory.hpp"

namespace {

struct Options {
    std::vector<std::string> tiles;
    std::string input_file = "";
    std::string image_file = "";
    std::string solver_name = solver_factory::kSimpleSolver;
    bool reflection = false;
    bool quiet = false;
};

void solve_action(Options options) {
    Problem problem =
            options.tiles.empty()
                    ? problem_parser::parse_from_file(options.input_file, options.reflection)
                    : problem_parser::parse(options.tiles, options.reflection);
    if (!options.quiet) {
        problem.print();
        print::normal() << std::endl;
    }

    std::unique_ptr<Solver> solver = solver_factory::create(options.solver_name, problem);
    Solution solution = solver->solve();
    if (solution.empty()) {
        print::warning_bold() << "FALSE\n" << std::endl;
    } else {
        print::success_bold() << "TRUE\n" << std::endl;
        if (!options.quiet) {
            solution.print();
        }
        if (!options.image_file.empty()) {
            solution.save_image(options.image_file, problem);
        }
    }
}

}  // namespace

int main(int argc, char **argv) {
    Options options;

    CLI::App app{"Tiler - tool for automated solving of polyomino tiling problems\n"};
    app.require_subcommand(1);

    // solve command definition
    CLI::App *solve_command = app.add_subcommand("solve", "Solve a tiling problem");

    CLI::App *input_group = solve_command->add_option_group("input", "The problem assignment");
    input_group->require_option(1);
    CLI::Option *tiles_option = input_group->add_option(
            "tiles", options.tiles,
            "Definition of the board and tile shapes.\n" + help_strings::kInputFormats);
    tiles_option->expected(-2);  // at least 2 - one board and one or more tile shapes
    CLI::Option *input_file_option =
            input_group->add_option("-f,--file", options.input_file,
                                    "Path to file containing the problem assignment.\n"
                                    "Same input format, but shapes must be separated\n"
                                    "by an empty line and the optional \"N:\" may be\n"
                                    "on a separate line.");
    input_file_option->check(CLI::ExistingFile);

    solve_command->add_option("-s,--save", options.image_file,
                              "Path to file where the solution (if it exists)\n"
                              "will be saved as an SVG image. If the file exists,\n"
                              "it will be overwritten.");

    solve_command
            ->add_option("-b,--backend", options.solver_name,
                         "Selected solver backend (default is " + options.solver_name + ").")
            ->transform(CLI::IsMember(solver_factory::solver_names));

    solve_command->add_flag(
            "-r,--allow-reflection", options.reflection,
            "If present, the solver will be allowed to reflect\n(flip over) the tiles.");

    solve_command->add_flag(
            "-q,--quiet", options.quiet,
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
            solve_action(options);
        } catch (const ParseError &e) {
            print::error() << e.what() << "\nRun with --help for more information." << std::endl;
            return 1;
        } catch (const SolveError &e) {
            print::error() << e.what() << std::endl;
            return 2;
        }
    } else if (command == list_command) {
        print::normal() << help_strings::kNamedTilesList;
    } else if (command == example_command) {
        print::normal() << help_strings::kExampleInput;
    }

    return 0;
}
