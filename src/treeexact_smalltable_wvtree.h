#ifndef __TREEEXACT_SMALLTABLE_WVTREE_H__
#define __TREEEXACT_SMALLTABLE_WVTREE_H__

#include <cstdio>
#include <vector>

namespace treeapprox {

bool treeexact_smalltable_wvtree(const std::vector<double>& x,
                                 size_t d,
                                 size_t k,
                                 std::vector<bool>* support);

};  // namespace treeapprox

#endif
