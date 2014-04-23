#include <vector>
#include <cstdio>

#include "treeapprox_binsearch.h"

using treeapprox::binsearch_options;
using treeapprox::treeapprox_binsearch;

std::vector<double> x;
std::vector<bool> support;
size_t n, d, k_low, k_high;

void output_function(const char* s) {
  fprintf(stderr, "%s", s);
  fflush(stderr);
}

int main() {
  scanf("%lu %lu %lu %lu", &n, &d, &k_low, &k_high);
  x.resize(n);

  for (size_t ii = 0; ii < n; ++ii) {
    scanf("%lg", &(x[ii]));
  }

  binsearch_options opts;
  opts.verbose = true;
  opts.output_function = output_function;
  opts.layout = treeapprox::binsearch_options::kWaveletTree;

  double lambda_low, lambda_high;
  size_t num_iter;
  bool result = treeapprox_binsearch(x, d, k_low, k_high, opts, &support,
      &lambda_low, &lambda_high, &num_iter);

  if (!result) {
    fprintf(stderr, "treeapprox_binsearch returned false.\n");
    return 1;
  }

  printf("%d", static_cast<int>(support[0]));
  for (size_t ii = 1; ii < n; ++ii) {
    printf(" %d", static_cast<int>(support[ii]));
  }
  printf("\n");

  return 0;
}
