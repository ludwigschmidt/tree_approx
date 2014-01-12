#include <vector>
#include <cstdio>

#include "treeexact_fulltable.h"

using treeapprox::treeexact_fulltable;

std::vector<double> x;
std::vector<bool> support;
size_t n, d, k;

int main() {
  scanf("%lu %lu %lu", &n, &d, &k);
  x.resize(n);

  for (size_t ii = 0; ii < n; ++ii) {
    scanf("%lg", &(x[ii]));
  }

  bool result = treeexact_fulltable(x, d, k, &support);

  if (!result) {
    fprintf(stderr, "treeexact_fulltable returned false.\n");
    return 1;
  }

  printf("%d", static_cast<int>(support[0]));
  for (size_t ii = 1; ii < n; ++ii) {
    printf(" %d", static_cast<int>(support[ii]));
  }
  printf("\n");

  return 0;
}
