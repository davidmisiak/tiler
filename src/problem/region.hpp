#ifndef TILER_PROBLEM_REGION_HPP_
#define TILER_PROBLEM_REGION_HPP_

#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "utils.hpp"

// Represents a (not necessarily continuous) region of unit squares.
class Region {
public:
    Region() = delete;

    // Initializes the region - make sure `matrix` is of dimensions `w` by `h`.
    Region(int w, int h, utils::BoolMatrix matrix);

    // Parses the shape definition (name, dimensions or map) and returns corresponding Region.
    // No extra-whitespace stripping nor continuity/hole checks are done - use this function
    // carefully and prefer `Region::parse` if possible.
    static Region parse_raw(std::string s);

    // Parses the shape definition (name, dimensions or map) and returns corresponding Region.
    // Extra spaces around map-defined shapes are stripped and a continuity and hole check is
    // performed (resulting in an error if failed).
    static Region parse(std::string s);

    // Returns copy of `region` rotated by 90 degrees counterclokwise.
    static Region rotate(const Region &region);

    // Returns copy of `region` mirrored by the y-axis.
    static Region reflect(const Region &region);

    bool operator==(const Region &other) const;
    friend std::ostream &operator<<(std::ostream &os, const Region &region);
    inline int get_width() const { return w_; };
    inline int get_height() const { return h_; };

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

    static const std::unordered_map<std::string, std::string> kNamedShapes;

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
