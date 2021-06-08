#ifndef TILER_HELP_STRINGS_HPP_
#define TILER_HELP_STRINGS_HPP_

#include <string>

namespace help_strings {

const std::string kNamedTilesList =
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

const std::string kInputFormats =
        "A shape has to be in one of these formats:\n"
        "- name, eg. \"3L\" (see the 'list' command)\n"
        "- dimensions, eg. \"2x4\" (only for rectangles)\n"
        "- map, eg. \"  xx \"\n"
        "           \" xx  \" (useful with the -f option)\n"
        "- perimeter, eg. \"DDRUUL\" (no crossing allowed)\n"
        "You should provide list of shapes, where the\n"
        "first one represents the board and the others\n"
        "the tiles. Tile shape may be prefixed with \"N:\"\n"
        "where N is the available number of such tiles\n"
        "(unlimited if not provided). See the 'example'\n"
        "command for input examples.";

const std::string kExampleInput =
        "Here are a few input examples.\n"
        "\n"
        "8x10 4x2\n"
        "- board 8 by 10, unlimited number of 4 by 2 tiles\n"
        "\n"
        "50x5 2:1x1 4I\n"
        "- board 50 by 5, two 1 by 1 tiles and unlimited number of 4 by 1 tiles\n"
        "\n"
        "5P 1:1 2:4O\n"
        "- board in the \"5P\" shape (3 by 2 without one corner, see the 'list'\n"
        "  command), one 1 by 1 tile and two 2 by 2 tiles\n"
        "\n"
        "========== start of 'problem.txt' file ==========\n"
        "10x10\n"
        "\n"
        "3I\n"
        "\n"
        "5:6x1\n"
        "\n"
        "10:LLUURRDD\n"
        "\n"
        "5:\n"
        "xxxxx\n"
        "xxxx\n"
        "=========== end of 'problem.txt' file ===========\n"
        "- board 10 by 10, unlimited number of 3 by 1 tiles, five 6 by 1 tiles,\n"
        "  ten 2 by 2 tiles and five 5 by 2 tiles without one corner\n"
        "  - usage: 'tiler solve -f problem.txt'\n";

}  // namespace help_strings

#endif  // TILER_HELP_STRINGS_HPP_
