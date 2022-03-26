#ifndef TILER_SOLVERS_DLX_DLX_WRAPPER_HPP_
#define TILER_SOLVERS_DLX_DLX_WRAPPER_HPP_

#include <vector>

#include "print.hpp"

// DLX1 exact cover finding algorithm wrapper.
class DlxWrapper {
    using Option = std::vector<int>;
    using Tag = int;

public:
    inline int get_primary_item_count() { return static_cast<int>(primary_items_.size()); }
    inline int get_secondary_item_count() { return static_cast<int>(secondary_items_.size()); }
    inline int get_option_count() { return static_cast<int>(options_.size()); }
    inline int get_node_count() {
        int s = 0;
        for (const auto &o : options_) s += static_cast<int>(o.first.size());
        return s;
    }

    // Adds primary item (an item that should be covered *exactly* once).
    inline int new_primary_item() {
        primary_items_.push_back(next_item_);
        return next_item_++;
    }

    // Adds secondary item (an item that should be covered *at most* once).
    inline int new_secondary_item() {
        secondary_items_.push_back(next_item_);
        return next_item_++;
    }

    // Adds a subset of items that form a covering option. The `tag` is an integer that will
    // eventually be returned in the solution.
    inline void add_option(const Option &option, Tag tag) { options_.push_back({option, tag}); }

    // Prints current info about items and options.
    inline void print_stats() {
        print::stats() << get_primary_item_count() << " primary items\n"
                       << get_secondary_item_count() << " secondary items\n"
                       << get_option_count() << " options\n"
                       << get_node_count() << " nodes\n";
    }

    // Tries to find a set of options that form a cover. If a solution exists, this returns a vector
    // of option tags; empty vector otherwise. Should be called at most once.
    std::vector<int> solve();

protected:
    int next_item_ = 0;
    std::vector<int> primary_items_;
    std::vector<int> secondary_items_;
    std::vector<std::pair<Option, Tag>> options_;
};

#endif  // TILER_SOLVERS_DLX_DLX_WRAPPER_HPP_
