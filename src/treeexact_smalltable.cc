#include "treeexact_smalltable.h"

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

size_t compute_node_table(const std::vector<double>& x,
                          size_t d,
                          size_t ii,
                          size_t last_parent,
                          size_t depth,
                          size_t k,
                          vector<double>* table,
                          vector<vector<size_t> >* num_allocated,
                          vector<vector<vector<double> > >* best) {
  vector<double>& table_ref = *table;
  vector<vector<size_t> >& num_allocated_ref = *num_allocated;
  vector<vector<vector<double> > >& best_ref = *best;

  //printf("call with ii = %lu  depth = %lu\n", ii, depth);

  if (ii > last_parent) {
    table_ref[0] = 0.0;
    table_ref[1] = x[ii];
    //printf("  in a child, returning 1.\n");
    return 1;
  } else {
    size_t to_alloc = 1;
    vector<size_t> child_size(d);

    size_t child_index = ii * d;
    for (size_t jj = 0; jj < d; ++jj) {
      child_index += 1;
      if (child_index >= x.size()) {
        break;
      }

      child_size[jj] = compute_node_table(x, d, child_index, last_parent,
                                          depth + 1, k, &(best_ref[depth][jj]),
                                          num_allocated, best);
      to_alloc += child_size[jj];
    }

    to_alloc = min(k, to_alloc);

    for (size_t jj = 0; jj < to_alloc; ++jj) {
      table_ref[jj] = 0.0;
      for (size_t cc = 0; cc < d; ++cc) {
        num_allocated_ref[jj][cc] = 0;
      }
    }

    child_index = ii * d;

    size_t max_num = 0;
    size_t prev_maxnum = 0;

    for (size_t jj = 0; jj < d; ++jj) {
      child_index += 1;
      if (child_index >= x.size()) {
        break;
      }

      prev_maxnum = max_num;
      max_num = min(k - 1, max_num + child_size[jj]);

      for (size_t cur_num = max_num; ; --cur_num) {
        for (size_t in_child = min(child_size[jj], cur_num); ; --in_child) {
          if (table_ref[cur_num] < table_ref[cur_num - in_child]
                                      + best_ref[depth][jj][in_child]) {
              table_ref[cur_num] = table_ref[cur_num - in_child]
                                      + best_ref[depth][jj][in_child];
              num_allocated_ref[cur_num][jj] = in_child;
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
    }

    for (size_t jj = to_alloc; jj >= 1; --jj) {
      table_ref[jj] = table_ref[jj - 1] + x[ii];
    }

    //printf("  returning %lu.\n", to_alloc);
    return to_alloc;
  }
}


bool treeexact_smalltable(const std::vector<double>& x,
                          size_t d,
                          size_t k,
                          std::vector<bool>* support) {
  if (k > x.size()) {
    return false;
  }

  std::vector<bool>& supp = *support;
  supp.resize(x.size());
  std::fill(supp.begin(), supp.end(), false);

  size_t node_sum = 0;
  size_t cur_level = 1;
  size_t depth = 0;
  while (node_sum < x.size()) {
    node_sum += cur_level;
    cur_level *= d;
    depth += 1;
  }
  size_t last_parent = (x.size() - 2) / d;

  vector<double> table(k + 1, 0.0);
  vector<vector<size_t> > num_allocated(k + 1);
  for (size_t jj = 0; jj <= k; ++jj) {
    num_allocated[jj].resize(d);
    std::fill(num_allocated[jj].begin(), num_allocated[jj].end(), 0);
  }
  vector<vector<vector<double> > > best(depth);
  for (size_t jj = 0; jj < depth; ++jj) {
    best[jj].resize(d);
    for (size_t cc = 0; cc < d; ++cc) {
      best[jj][cc].resize(k + 1);
      std::fill(best[jj][cc].begin(), best[jj][cc].end(), 0.0);
    }
  }

  // top-down pass: identify support (BFS)

  queue<pair<size_t, size_t> > q;
  if (k > 0) {
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

    // call recursive function
    compute_node_table(x, d, cur_node, last_parent, 0, k, &table,
        &num_allocated, &best);

    /*size_t ci = cur_node * d;
    for (size_t cc = 0; cc < d; ++cc) {
      ci += 1;
      printf("  index = %lu\n", ci);
      for (size_t jj = 0; jj < k; ++jj) {
        printf("    k = %lu: %lf\n", jj, best[0][cc][jj]);
      }
      printf("\n");
    }*/
    
    size_t child_index = min(cur_node * d + d, x.size() - 1);
    size_t start_index = d - 1 - (cur_node * d + d - child_index);
    for (size_t jj = start_index; ; --jj) {
      size_t allocated = num_allocated[cur_k][jj];
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
