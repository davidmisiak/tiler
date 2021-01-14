#ifndef TILER_PROBLEM_REGION_HPP_
#define TILER_PROBLEM_REGION_HPP_

#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

// Represents a (not necessarily continuous) region of unit squares.
class Region {
public:
    Region() = delete;

    // Initializes the region - make sure `matrix` is of dimensions `w` by `h`.
    Region(int w, int h, std::vector<std::vector<bool>> matrix);

    // Parses the shape definition (name, dimensions or map) and returns corresponding Region.
    // Note that it expects no leading or trailing whitespace (except of leading spaces when
    // parsing a shape defined by map).
    static Region parse(std::string s);

    // Returns copy of `region` rotated by 90 degrees counterclokwise.
    static Region rotate(Region region);

    // Returns copy of `region` mirrored by the y-axis.
    static Region reflect(Region region);

    bool operator==(const Region &other) const;
    friend std::ostream &operator<<(std::ostream &os, const Region &region);
    inline int get_width() const { return w_; };
    inline int get_height() const { return h_; };

    static const std::unordered_map<std::string, std::string> kNamedShapes;

private:
    int w_;
    int h_;
    std::vector<std::vector<bool>> matrix_;
};

#endif  // TILER_PROBLEM_REGION_HPP_
