#include <vector>

#include <math.h>
#include <matrix.h>
#include <mex.h>

#include "mex_helper.h"
#include "treeapprox_greedy_largest.h"

using std::vector;
using treeapprox::treeapprox_greedy_largest;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
  if (nrhs != 3) {
    mexErrMsgTxt("Exactly three input argument required (signal, tree degree,"
        " k).");
  }
  if (nlhs > 1) {
    mexErrMsgTxt("Too many output arguments.");
  }

  // required parameters

  vector<double> x;
  bool is_column_vector;
  if (!get_double_vector(prhs[0], &x, &is_column_vector)) {
    mexErrMsgTxt("Could not read the signal as a vector of type double.");
  }

  size_t d = 0;
  if (!get_double_as_size_t(prhs[1], &d)) {
    mexErrMsgTxt("Tree degree has to be a positive integer.");
  }

  size_t k = 0;
  if (!get_double_as_size_t(prhs[2], &k)) {
    mexErrMsgTxt("k to be a positive integer.");
  }

  // function call

  vector<bool> support;
  bool result = treeapprox_greedy_largest(x, d, k, &support);

  if (!result) {
    mexErrMsgTxt("treeapprox_greedy_largest returned false.");
  }

  // return values

  if (nlhs >= 1) {
    set_double_vector(&(plhs[0]), support, is_column_vector);
  }
}
