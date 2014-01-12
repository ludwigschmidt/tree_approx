#include "treeexact_fulltable.h"

#include <algorithm>
#include <vector>

using std::min;
using std::vector;

namespace treeapprox {

bool treeexact_fulltable(const std::vector<double>& x,
                         size_t d,
                         size_t k,
                         std::vector<bool>* support) {
  if (k > x.size()) {
    return false;
  }

  support->resize(x.size());
  std::fill(supp.begin(), supp.end(), false);

  size_t num_leaves = (x.size() * (d - 1) + 1) / d;
  size_t last_parent = x.size() - num_leaves - 1;

  vector<vector<double> > table(x.size());

  for (size_t ii = last_parent + 1; ii < x.size(); ++ii) {
    table[ii].resize(2);
    table[ii][0] = 0.0;
    table[ii][1] = x[ii];
  }

  // bottom-up pass: compute best sum for each (node, tree size)

  for (size_t ii = last_parent; ; --ii) {
    size_t to_alloc = 0;
    size_t child_index = ii * d;
    for (size_t jj = 1; jj <= d; ++jj) {
      child_index += 1;
      to_alloc += (table[child_index].size() - 1);
    }
    to_alloc = min(to_alloc, k);

    table->resize(to_alloc + 1);

    table[ii][0] = 0.0;
    table[ii][1] = x[ii];
    size_t max_num = 1;
    size_t prev_maxnum = 0;

    size_t child_index = ii * d;
    for (size_t jj = 1; jj <= d; ++jj) {
      child_index += 1;

      prev_maxnum = max_num;
      max_num = min(k, max_num + table[child_index].size() - 1);

      for (size_t cur_num = max_num; ; --cur_num) {
        for (size_t in_child = min(table[child_index].size() - 1, cur_num); ;
            --in_child) {
          table[ii][cur_num] = max(table[ii][cur_num],
                                   table[ii][cur_num - in_child] +
                                      table[child_index][in_child]);
          if (in_child == 0) {
            break;
          }
          if (cur_num - in_child <= prev_maxnum) {
            break;
          }
        }

        if (cur_num <= 1) {
          break;
        }
      }
    }

    if (ii == 0) {
      break;
    }
  }

  // top-down pass: identify support (BFS)

  // TODO

  return true;
}

};  // namespace treeapprox
