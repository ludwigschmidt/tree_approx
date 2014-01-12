#include <vector>

#include <math.h>
#include <matrix.h>
#include <mex.h>

#include "mex_helper.h"
#include "treeexact_fulltable.h"

using std::vector;
using treeapprox::treeexact_fulltable;

void output_function(const char* s) {
  mexPrintf(s);
  mexEvalString("drawnow;");
}

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
  if (!get_double_row_vector(prhs[0], &x)) {
    mexErrMsgTxt("The signal amplitudes must be a row vector of type double.");
  }
  if (x.size() == 0) {
    mexErrMsgTxt("The input signal must have at least one element.");
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
  bool result = treeexact_fulltable(x, d, k, &support);

  if (!result) {
    mexErrMsgTxt("treeexact_fulltable returned false.");
  }

  // return values

  if (nlhs >= 1) {
    set_double_row_vector(&(plhs[0]), support);
  }
}
