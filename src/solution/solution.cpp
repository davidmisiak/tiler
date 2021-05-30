#include "solution/solution.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "parsers/region_parser.hpp"
#include "print.hpp"
#include "problem/problem.hpp"
#include "problem/region.hpp"
#include "problem/tile.hpp"
#include "svgwrite/writer.hpp"

namespace {
std::string random_color() {
    char s[20];
    sprintf(s, "rgb(%d,%d,%d)", rand() % 256, rand() % 256, rand() % 256);
    return s;
}

}  // namespace

void Solution::save_image(std::string filepath, Problem problem) {
    // compute the color of each region in the solution
    std::map<Region, std::string> colors;
    // each tile gets a random color (same for each rotation/reflection)
    for (Tile tile : problem.tiles_) {
        std::string color = random_color();
        for (Region region : tile) {
            colors[region] = color;
        }
    }
    // but named shapes have fixed colors
    int i = 0;
    for (auto [_, m] : Region::kNamedShapes) {
        Region region = region_parser::parse(m);
        for (int rot = 0; rot < 4; rot++) {
            colors[region] = kColors[i];
            region = Region::rotate(region);
        }
        i++;
    }

    std::ofstream file;
    file.open(filepath);
    svgw::writer writer(file);

    const int w = problem.board_.get_width();
    const int h = problem.board_.get_height();
    writer.start_svg((w + 2) * kCellSize, (h + 2) * kCellSize, {});

    for (auto [x, y, region] : *this) {
        for (auto [cx, cy] : region.get_cells()) {
            writer.rect((x + cx + 1) * kCellSize, (y + cy + 1) * kCellSize, kCellSize, kCellSize,
                        {{"fill", colors[region]}});
        }
        for (auto [ex, ey, dx, dy] : region.get_edges()) {
            writer.line((x + ex + 1) * kCellSize, (y + ey + 1) * kCellSize,
                        (x + ex + 1 + dx) * kCellSize, (y + ey + 1 + dy) * kCellSize,
                        {{"stroke", "black"}, {"stroke-width", 2}});
        }
    }

    writer.end_svg();
    file.close();
}

std::ostream &operator<<(std::ostream &os, const Solution &solution) {
    for (auto [x, y, region] : solution) {
        os << "\n" << region << "\n";
        os << "at (" << x << ", " << y << ")\n";
    }
    return os;
}

const int Solution::kCellSize = 30;

// generated by https://mokole.com/palette.html
const std::vector<std::string> Solution::kColors{
        "#00008b", "#0000ff", "#1e90ff", "#00bfff", "#00ffff", "#008b8b", "#228b22", "#90ee90",
        "#00ff7f", "#00ff00", "#adff2f", "#ffff54", "#f0e68c", "#ffa500", "#e9967a", "#ff4500",
        "#8b4513", "#dc143c", "#b03060", "#ff1493", "#ff00ff", "#dda0dd", "#8a2be2", "#8b008b",
        "#483d8b", "#696969", "#b0c4de", "#8fbc8f", "#556b2f"};