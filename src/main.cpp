#include <vector>

#include "CLI/CLI.hpp"
#include "help_strings.hpp"
#include "print.hpp"

int main(int argc, char **argv) {
    CLI::App app{"Tiler - tool for automated solving of polyomino tiling problems\n"};
    app.require_subcommand(1);

    // solve command definition
    CLI::App *solve_command = app.add_subcommand("solve", "Solve a tiling problem");

    CLI::App *input_group = solve_command->add_option_group("input", "The problem assignment");
    input_group->require_option(1);
    std::vector<std::string> tiles;
    CLI::Option *tiles_option = input_group->add_option(
            "tiles", tiles, "Definition of the board and tile shapes. Format: TODO");
    tiles_option->expected(-2);  // at least 2 - one board and one or more tile shapes
    std::string input_file = "";
    CLI::Option *input_file_option = input_group->add_option(
            "-f,--file", input_file, "File containing the problem assignment");
    input_file_option->check(CLI::ExistingFile);

    // list command definition
    CLI::App *list_command = app.add_subcommand("list", "List all named tiles.");

    // example command definition
    CLI::App *example_command = app.add_subcommand("example", "Show an input example.");

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
    } else if (command == list_command) {
        print::normal() << help_strings::kNamedTilesList;
    } else if (command == example_command) {
    }

    return 0;
}
