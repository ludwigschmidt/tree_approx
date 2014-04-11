#ifndef __TREEAPPROX_GREEDY_LARGEST_H__
#define __TREEAPPROX_GREEDY_LARGEST_H__

#include <cstdlib>
#include <vector>

namespace treeapprox {

bool treeapprox_greedy_largest(const std::vector<double>& x,
                               size_t d,
                               size_t k,
                               std::vector<bool>* support);

};  // namespace treeapprox

#endif
