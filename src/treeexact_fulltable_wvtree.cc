#include "treeexact_fulltable_wvtree.h"

#include <algorithm>
#include <map>
#include <queue>
#include <vector>

using std::make_pair;
using std::min;
using std::pair;
using std::queue;
using std::vector;

namespace treeapprox {

bool treeexact_fulltable_wvtree(const std::vector<double>& x,
                                size_t d,
                                size_t k,
                                std::vector<bool>* support) {
  if (k > x.size()) {
    return false;
  }
  if (x.size() < d) {
    return false;
  }

  std::vector<bool>& supp = *support;
  supp.resize(x.size());
  std::fill(supp.begin(), supp.end(), false);

  size_t last_parent = (x.size() - 1) / d;

  vector<vector<double> > table(x.size());
  vector<vector<vector<size_t> > > num_allocated(x.size());

  for (size_t ii = last_parent + 1; ii < x.size(); ++ii) {
    table[ii].resize(2);
    table[ii][0] = 0.0;
    table[ii][1] = x[ii];
  }

  // bottom-up pass: compute best sum for each (node, tree size)

  for (size_t ii = last_parent; ; --ii) {
    size_t to_alloc = 1;
    size_t num_children = d;
    size_t child_index = ii * d;
    if (ii == 0) {
      num_children = d - 1;
      child_index = 1;
    }
    if (ii == last_parent) {
      num_children = (x.size() - 1) - child_index + 1;
    }

    for (size_t jj = 0; jj < num_children; ++jj) {
      to_alloc += (table[child_index].size() - 1);
      child_index += 1;
    }
    to_alloc = min(to_alloc, k);

    table[ii].resize(to_alloc + 1, 0.0);
    num_allocated[ii].resize(to_alloc + 1);
    for (size_t jj = 0; jj < num_allocated[ii].size(); ++jj) {
      num_allocated[ii][jj].resize(num_children);
    }

    size_t max_num = 0;
    size_t prev_maxnum = 0;

    child_index = ii * d;
    if (ii == 0) {
      child_index = 1;
    }
    for (size_t jj = 0; jj < num_children; ++jj) {
      prev_maxnum = max_num;
      max_num = min(k - 1, max_num + table[child_index].size() - 1);

      for (size_t cur_num = max_num; ; --cur_num) {
        for (size_t in_child = min(table[child_index].size() - 1, cur_num); ;
            --in_child) {
          if (table[ii][cur_num] < table[ii][cur_num - in_child]
                                      + table[child_index][in_child]) {
            table[ii][cur_num] = table[ii][cur_num - in_child]
                                    + table[child_index][in_child];
            num_allocated[ii][cur_num][jj] = in_child;
            //printf("new best entry for (%lu, %lu): allocating %lu to %lu "
            //    "value: %lf\n", ii, cur_num, in_child, child_index,
            //    table[ii][cur_num]);
          }

          if (in_child == 0) {
            break;
          }
          if (cur_num - in_child >= prev_maxnum) {
            break;
          }
        }

        if (cur_num <= 1) {
          break;
        }
      }
      
      child_index += 1;
    }

    for (size_t jj = table[ii].size() - 1; jj >= 1; --jj) {
      table[ii][jj] = table[ii][jj - 1] + x[ii];
    }

    if (ii == 0) {
      break;
    }
  }

  /*for (size_t ii = 0; ii < x.size(); ++ii) {
    printf("ii = %lu\n", ii);
    for (size_t jj = 0; jj < table[ii].size(); ++jj) {
      printf("  k = %lu: %lf\n", jj, table[ii][jj]);
    }
    printf("\n");
  }*/

  // top-down pass: identify support (BFS)

  queue<pair<size_t, size_t> > q;
  if (table[0][k] > 0.0) {
    q.push(make_pair(0, k));
  }

  while (!q.empty()) {
    size_t cur_node = q.front().first;
    size_t cur_k = q.front().second;
    q.pop();
    
    //printf("Allocating %lu to node %lu.\n", cur_k, cur_node);

    supp[cur_node] = true;
    cur_k -= 1;

    if (cur_node > last_parent) {
      continue;
    }
    
    size_t child_index = cur_node * d + d - 1;
    size_t num_children = d;
    if (child_index >= x.size()) {
      child_index = x.size() - 1;
      num_children = child_index - cur_node * d + 1;
    }
    if (cur_node == 0) {
      num_children = d - 1;
    }
    for (size_t jj = num_children - 1; ; --jj) {
      size_t allocated = num_allocated[cur_node][cur_k][jj];
      if (allocated > 0) {
        q.push(make_pair(child_index, allocated));
        cur_k -= allocated;
      }

      if (jj == 0) {
        break;
      }
      child_index -= 1;
    }
  }

  return true;
}

};  // namespace treeapprox
