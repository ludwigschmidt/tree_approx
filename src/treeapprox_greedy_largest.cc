#include "treeapprox_greedy_largest.h"

#include <algorithm>
#include <map>

using std::pair;
using std::vector;

namespace treeapprox {

bool treeapprox_greedy_largest(const std::vector<double>& x,
                               size_t d,
                               size_t k,
                               std::vector<bool>* support) {
  std::vector<bool>& supp = *support;
  supp.resize(x.size());
  std::fill(supp.begin(), supp.end(), false);

  if (x.size() == 0 || k == 0) {
    return true;
  }

  supp[0] = true;
  size_t supp_size = 1;

  vector<pair<double, size_t> > vals;
  vals.resize(x.size());
  for (size_t ii = 0; ii < x.size(); ++ii) {
    vals[ii].first = x[ii];
    vals[ii].second = ii;
  }

  // We only need to sort the k largest elements.
  std::nth_element(vals.begin(), vals.begin() + (x.size() - k), vals.end());
  std::sort(vals.begin() + (x.size() - k), vals.end());

  for (vector<pair<double, size_t> >::reverse_iterator iter = vals.rbegin();
      iter != vals.rend(); ++iter) {
    size_t curi = iter->second;
    if (supp[curi]) {
      continue;
    }

    size_t to_add = 0;
    size_t tmp_node = curi;
    while (!supp[tmp_node]) {
      tmp_node /= d;
      ++to_add;
    }

    if (to_add + supp_size > k) {
      break;
    }

    supp_size += to_add;
    tmp_node = curi;
    while (!supp[tmp_node]) {
      supp[tmp_node] = true;
      tmp_node /= d;
    }
  }

  return true;
}

}  // namespace treeapprox
