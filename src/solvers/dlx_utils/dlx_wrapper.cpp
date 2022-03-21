
#include "solvers/dlx_utils/dlx_wrapper.hpp"

#include <algorithm>
#include <vector>

// #include "icecream.hpp"

// external/dlx1/dlx1-lib.cpp
extern std::vector<std::vector<int>> dlx_solve(const std::vector<int> &p_items,
                                               const std::vector<int> &s_items,
                                               const std::vector<std::vector<int>> &in_options);

std::vector<int> DlxWrapper::solve() {
    std::sort(primary_items_.begin(), primary_items_.end());
    std::sort(secondary_items_.begin(), secondary_items_.end());
    for (auto &[option, _] : options_) {
        std::sort(option.begin(), option.end());
    }
    std::sort(options_.begin(), options_.end());

    std::vector<Option> dlx_options;
    std::transform(options_.begin(), options_.end(), std::back_inserter(dlx_options),
                   [](const std::pair<Option, int> &tagged_option) { return tagged_option.first; });

    // Uncomment if you want to print the covering problem in DLX1's almost CLI-compatible format.
    // IC(primary_items_);
    // IC(secondary_items_);
    // IC(dlx_options);

    auto cover = dlx_solve(primary_items_, secondary_items_, dlx_options);
    if (cover.empty()) return {};

    for (auto &option : cover) {
        std::sort(option.begin(), option.end());
    }
    std::sort(cover.begin(), cover.end());

    std::vector<Tag> tags;
    int cover_index = 0;
    for (int opt_index = 0; opt_index < static_cast<int>(options_.size()); opt_index++) {
        if (cover[cover_index] != options_[opt_index].first) continue;
        tags.push_back(options_[opt_index].second);
        cover_index++;
        if (cover_index > static_cast<int>(cover.size())) break;
    }
    return tags;
};
