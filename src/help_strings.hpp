#ifndef TILER_HELP_STRINGS_HPP_
#define TILER_HELP_STRINGS_HPP_

#include <string>

namespace help_strings {

// Maximum width of line content. Not used for automatic word wrapping in help texts, but useful
// elsewhere.
const int kMaxWidth = 80;

const std::string kNamedShapesList =
        "Monomino          Domino             Trominoes                   \n"
        "                                                                 \n"
        " 1: x             2: x               3I: x    3L: x              \n"
        "                     x                   x        xx             \n"
        "                                         x                       \n"
        "                                                                 \n"
        "Tetrominoes                                                      \n"
        "                                                                 \n"
        " 4I: x   4J:  x   4L: x    4O:  xx   4S:  xx   4T: xxx   4Z: xx  \n"
        "     x        x       x         xx       xx         x         xx \n"
        "     x       xx       xx                                         \n"
        "     x                                                           \n"
        "                                                                 \n"
        "Pentominoes                                                      \n"
        "                                                                 \n"
        " 5A:  x     5F:  xx    5G: x      5J:  x     5P: xx     5S:  xx  \n"
        "      x         xx         xx          x         xx          x   \n"
        "     xx          x          x          x         x          xx   \n"
        "      x                     x         xx                         \n"
        "                                                                 \n"
        " 5Y:  x     5R: xx     5N:  x     5L: x      5Q: xx     5Z: xx   \n"
        "     xx          xx        xx         x          xx          x   \n"
        "      x          x         x          x           x          xx  \n"
        "      x                    x          xx                         \n"
        "                                                                 \n"
        " 5I: x      5T: xxx    5U: x x    5V: x      5W: x      5X:  x   \n"
        "     x           x         xxx        x          xx         xxx  \n"
        "     x           x                    xxx         xx         x   \n"
        "     x                                                           \n"
        "     x                                                           \n";

// clang-format off
const std::string kSolverBackends =
R"R(Each backend identifier consists of several parts separated by
an underscore. The leading part determines the backend category, and
the subsequent parts determine the specific backend configuration.
The categories and their configurations are:

'simple': simple backtracking
- first part: next tile selection heuristic, one of 'default',
  'frequent', 'rare', 'fill-row'

'dlx': Dancing Links
[no configuration options]

'sat': conversion to SAT
- first part: SAT solver, one of 'cadical', 'cms', 'kissat'
- second part: preprocessor, one of 'noprep', 'breakid', 'sbva'
- third part: problem encoding, one of 'amo-auto', 'amo-seq',
  'amo-bimander', 'amo-commander', 'amo-kproduct', 'amo-binary',
  'amo-ordered', 'amk-auto', 'amk-bdd', 'amk-card'

'ilp': conversion to ILP
- first part: ILP solver, one of 'cbc', 'gurobi'
- second part: solver parameters, one of 'default', 'adjusted'
- third part: solving objective, one of 'eq-ign', 'eq-min',
  'geq-min', 'leq-max'

'csp': conversion to MiniZinc CSP
- first part: MiniZinc flat solver, one of 'gecode', 'chuffed',
  'gurobi'

List of all available identifiers:
)R";
// clang-format on

// clang-format off
const std::string kInputFormats =
R"R(A shape has to be in one of these formats:
- name, eg. "3L" (see the 'shapes' command)
- dimensions, eg. "2x4" (only for rectangles)
- map, eg. "  xx "
           " xx  " (useful with the -i option)
- perimeter, eg. "DDRUUL" (no crossing allowed)
You should provide list of shapes, where the
first one represents the board and the others
the tiles. Tile shape may be prefixed with "N:"
where N is the available number of such tiles
(unlimited if not provided). See the 'example'
command for input examples.)R";
// clang-format on

// clang-format off
const std::string kExampleInput =
R"R(Here are a few input examples.

8x10 4x2
- board 8 by 10, unlimited number of 4 by 2 tiles

50x5 2:1x1 4I
- board 50 by 5, two 1 by 1 tiles and unlimited number of 4 by 1 tiles

5P 1:1 2:4O
- board in the "5P" shape (3 by 2 without one corner, see the 'list'
  command), one 1 by 1 tile and two 2 by 2 tiles

========== start of 'problem.txt' file ==========
10x10

3I

5:6x1

10:LLUURRDD

5:
xxxxx
xxxx
=========== end of 'problem.txt' file ===========
- board 10 by 10, unlimited number of 3 by 1 tiles, five 6 by 1 tiles,
  ten 2 by 2 tiles and five 5 by 2 tiles without one corner
  - usage: 'tiler solve -i problem.txt'
)R";
// clang-format on

}  // namespace help_strings

#endif  // TILER_HELP_STRINGS_HPP_
