#include "treeapprox_greedy.h"

#include <algorithm>
#include <map>
#include <queue>

namespace treeapprox {

bool treeapprox_greedy(const std::vector<double>& x,
                       size_t d,
                       size_t k,
                       std::vector<bool>* support) {
  std::vector<bool>& supp = *support;
  supp.resize(x.size());
  std::fill(supp.begin(), supp.end(), false);

  std::priority_queue<std::pair<double, size_t> > q;
  
  if (x.size() == 0 || k == 0) {
    return true;
  }

  supp[0] = true;
  for (size_t ii = 1; ii < d && ii < x.size(); ++ii) {
    q.push(std::make_pair(x[ii], ii));
  }

  if (k > x.size()) {
    k = x.size();
  }
  
  for (size_t num_supp = 1; num_supp < k; ++num_supp) {
    size_t cur_node = q.top().second;
    q.pop();
    supp[cur_node] = true;

    for (size_t ii = cur_node * d;
        ii < cur_node * d + d && ii < x.size();
        ++ii) {
      q.push(std::make_pair(x[ii], ii));
    }
  }

  return true;
}

}  // namespace treeapprox
