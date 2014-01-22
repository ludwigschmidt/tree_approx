#include <vector>

#include <math.h>
#include <matrix.h>
#include <mex.h>

#include "mex_helper.h"
#include "rearrange_wavedec2.h"

using std::vector;
using treeapprox::rearrange_wavedec2;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
  if (nrhs != 2) {
    mexErrMsgTxt("Exactly two input argument required (signal, direction).");
  }
  if (nlhs > 1) {
    mexErrMsgTxt("Too many output arguments.");
  }

  // required parameters

  size_t rows;
  size_t columns;
  if (!get_matrix_dimensions(prhs[0], &rows, &columns)) {
    mexErrMsgTxt("Could not get the dimensions of the input.");
  }

  if (rows != 1 && columns != 1) {
    mexErrMsgTxt("First parameter must be a vector.");
  }

  if (!mxIsClass(prhs[0], "double")) {
    mexErrMsgTxt("First parameter must be a double vector.");
  }

  double* data = static_cast<double*>(mxGetData(prhs[0]));

  bool forward = true;
  if (!mxIsClass(prhs[1], "char")) {
    mexErrMsgTxt("Second parameter must be a string.");
  }
  char* direction = mxArrayToString(prhs[1]);
  if (strcmp(direction, "backward") == 0) {
    forward = false;
  } else if (strcmp(direction, "forward") != 0) {
    mexErrMsgTxt("Second parameter must be \"forward\" or \"backward\".");
  }
  mxFree(direction);

  // function call
  
  size_t n = rows * columns;
  vector<size_t> indices;
  vector<size_t>* forward_indices;
  vector<size_t>* backward_indices;
  if (forward) {
    forward_indices = &indices;
    backward_indices = NULL;
  } else {
    forward_indices = NULL;
    backward_indices = &indices;
  }

  if (!rearrange_wavedec2(n, forward_indices, backward_indices)) {
    mexErrMsgTxt("Could not compute the index rearrangement.");
  }

  // return values

  if (nlhs >= 1) {
    int numdims = 2;
    mwSize dims[2];
    dims[0] = rows;
    dims[1] = columns;
    plhs[0] = mxCreateNumericArray(numdims, dims, mxDOUBLE_CLASS, mxREAL);
    double* output_data_linear = static_cast<double*>(mxGetData(plhs[0]));
    for (size_t ii = 0; ii < n; ++ii) {
      output_data_linear[ii] = data[indices[ii]];
    }
  }
}
