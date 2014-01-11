#ifndef __TREEAPPROX_BIN_SEARCH_H__
#define __TREEAPPROX_BIN_SEARCH_H__

#include <cstdlib>
#include <vector>

namespace treeapprox {

struct binsearch_options {
  int max_num_iterations;
  double lambda_low;
  double lambda_high;
  bool verbose;
  void (*output_function)(const char*);

  binsearch_options() : max_num_iterations(-1), lambda_low(-1), lambda_high(-1),
      verbose(false), output_function(NULL) {}
};

bool treeapprox_binsearch(const std::vector<double>& x,
                          size_t d,
                          size_t k_low,
                          size_t k_high,
                          const binsearch_options& options,
                          std::vector<bool>* support,
                          double* final_lambda_low,
                          double* final_lambda_high,
                          size_t* num_iterations);


};  // namespace treeapprox

#endif
