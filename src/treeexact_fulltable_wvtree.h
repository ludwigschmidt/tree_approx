#ifndef __TREEEXACT_FULLTABLE_WVTREE_H__
#define __TREEEXACT_FULLTABLE_WVTREE_H__

#include <cstdio>
#include <vector>

namespace treeapprox {

bool treeexact_fulltable_wvtree(const std::vector<double>& x,
                                size_t d,
                                size_t k,
                                std::vector<bool>* support);

};  // namespace treeapprox

#endif
