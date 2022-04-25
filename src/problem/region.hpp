#ifndef TILER_PROBLEM_REGION_HPP_
#define TILER_PROBLEM_REGION_HPP_

#include <map>
#include <ostream>
#include <string>
#include <vector>

#include "utils.hpp"

// Represents a (not necessarily continuous) region of unit squares.
class Region {
public:
    Region() = delete;

    // Initializes the region - make sure `matrix` is of dimensions `w` by `h`.
    Region(int w, int h, utils::BoolMatrix matrix);

    // Returns copy of `this` rotated by 90 degrees counterclokwise.
    Region rotate() const;

    // Returns copy of `this` mirrored by the y-axis.
    Region reflect() const;

    inline bool operator==(const Region &other) const {
        return w_ == other.w_ && h_ == other.h_ && matrix_ == other.matrix_;
    };
    inline bool operator<(const Region &other) const { return matrix_ < other.matrix_; };

    void print() const;

    inline int get_width() const { return w_; };
    inline int get_height() const { return h_; };
    inline utils::BoolMatrix get_matrix() const { return matrix_; };

    // Returns the number of unit squares occupied by the region.
    inline int get_size() const { return size_; };

    // Returs the x coordinate of the left-most occupied unit square in the region's top-most
    // occupied row.
    inline int get_top_left_x() const { return top_left_x_; };

    // Returs the y coordinate of the region's top-most occupied row.
    inline int get_top_left_y() const { return top_left_y_; };

    // Checks if all the squares of `region` are also present in `this` region when the top-left
    // corner of the `region`'s surrounding rectangle is positioned at [`origin_x`, `origin_y`]
    // coordinates of `this`.
    bool has_subregion(int origin_x, int origin_y, const Region &region) const;

    // Removes `region` from `this` (see `has_subregion()` for the meaning of the parameters).
    // Note that you are responsible for checking if this is possible (by running `has_subregion`).
    void remove_subregion(int origin_x, int origin_y, const Region &region);

    // Adds `region` to `this` (see `has_subregion()` for the meaning of the parameters).
    // Note that you are responsible for checking if this is possible (by running `has_subregion`).
    void add_subregion(int origin_x, int origin_y, const Region &region);

    // Returns the list of coordinates of the region's cells. Top-left corner of the region's
    // surrounding rectangle has coordinates (0, 0).
    std::vector<std::pair<int, int>> get_cells() const;

    // Returns the list of coordinates of the cells missing from the region's surrounding rectangle.
    // Top-left corner of the region's surrounding rectangle has coordinates (0, 0).
    std::vector<std::pair<int, int>> get_missing_cells() const;

    // Returs the list of edges defining the region.
    std::vector<utils::Edge> get_edges() const;

    static const std::map<std::string, std::string> kNamedShapes;

private:
    // Updates the coordinates of the left-most occupied unit square in the region's top-most
    // occupied row.
    void update_top_left(int from_x, int from_y);

    int w_;
    int h_;
    utils::BoolMatrix matrix_;
    int size_;
    int top_left_x_;
    int top_left_y_;
};

#endif  // TILER_PROBLEM_REGION_HPP_
