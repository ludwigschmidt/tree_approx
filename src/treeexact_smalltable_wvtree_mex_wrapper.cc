#include <vector>

#include <math.h>
#include <matrix.h>
#include <mex.h>

#include "mex_helper.h"
#include "treeexact_smalltable_wvtree.h"
#include "wavedec2_rearrange.h"

using std::vector;
using treeapprox::treeexact_smalltable_wvtree;
using treeapprox::wavedec2_rearrange;

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

  size_t d = 0;
  if (!get_double_as_size_t(prhs[1], &d)) {
    mexErrMsgTxt("Tree degree has to be a positive integer.");
  }

  if (d != 4) {
    mexErrMsgTxt("Currentl only trees with degree 4 are supported.");
  }

  vector<double> x;
  size_t rows;
  size_t columns;
  size_t n;
  if (!get_matrix_dimensions(prhs[0], &rows, &columns)) {
    mexErrMsgTxt("Could not read the dimensions of the input signal.");
  }

  if (columns != 1) {
    mexErrMsgTxt("The input signal must be a column vector.");
  }
  if (rows == 0 || columns == 0) {
    mexErrMsgTxt("The input signal must have at least one element.");
  }
  n = rows;

  vector<size_t> forward_indices;
  vector<size_t> backward_indices;
  if (!wavedec2_rearrange(n, &forward_indices, &backward_indices)) {
    mexErrMsgTxt("Could not compute the index rearrangement.");
  }

  if (!get_double_column_vector_with_indexing(prhs[0], &x, forward_indices)) {
    mexErrMsgTxt("Could not read the signal as a vector of type double.");
  }

  size_t k = 0;
  if (!get_double_as_size_t(prhs[2], &k)) {
    mexErrMsgTxt("k to be a positive integer.");
  }

  // function call

  vector<bool> support;
  bool result = treeexact_smalltable_wvtree(x, d, k, &support);

  if (!result) {
    mexErrMsgTxt("treeexact_smalltable_wvtree returned false.");
  }

  // return values

  if (nlhs >= 1) {
    set_double_column_vector_with_indexing(&(plhs[0]), support,
        backward_indices);
  }
}
