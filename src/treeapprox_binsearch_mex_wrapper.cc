#include <vector>
#include <string>
#include <set>

#include <math.h>
#include <matrix.h>
#include <mex.h>

#include "mex_helper.h"
#include "treeapprox_binsearch.h"

using std::set;
using std::string;
using std::vector;
using treeapprox::binsearch_options;
using treeapprox::treeapprox_binsearch;

void output_function(const char* s) {
  mexPrintf(s);
  mexEvalString("drawnow;");
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
  if (nrhs < 4) {
    mexErrMsgTxt("At least four input argument required (signal, tree degree,"
        " k_low, k_high).");
  }
  if (nrhs > 5) {
    mexErrMsgTxt("Too many input arguments, at most five: signal, tree degree,"
        " k_low, k_high, and the options struct.");
  }
  if (nlhs > 4) {
    mexErrMsgTxt("Too many output arguments.");
  }

  // required parameters

  vector<double> x;
  bool is_column_vector;
  if (!get_double_vector(prhs[0], &x, &is_column_vector)) {
    mexErrMsgTxt("Could not read the signal as a vector of type double.");
  }
  if (x.size() == 0) {
    mexErrMsgTxt("The input signal must have at least one element.");
  }

  size_t d = 0;
  if (!get_double_as_size_t(prhs[1], &d)) {
    mexErrMsgTxt("Tree degree has to be a positive integer.");
  }

  size_t k_low = 0;
  if (!get_double_as_size_t(prhs[2], &k_low)) {
    mexErrMsgTxt("k_low has to be a positive integer.");
  }

  size_t k_high = 0;
  if (!get_double_as_size_t(prhs[3], &k_high)) {
    mexErrMsgTxt("k_high has to be a positive integer.");
  }


  // optional parameters

  binsearch_options opts;
  opts.max_num_iterations = -1;
  opts.lambda_low = -1;
  opts.lambda_high = -1;
  opts.verbose = false;
  opts.output_function = output_function;
  opts.layout = binsearch_options::kCompleteTree;
  std::string layout_string;

  if (nrhs == 5) {
    // set of accepted options
    set<string> known_options;
    known_options.insert("verbose");
    known_options.insert("max_num_iterations");
    known_options.insert("tree_layout");

    // retrieve option struct fields
    vector<string> options;
    if (!get_fields(prhs[4], &options)) {
      mexErrMsgTxt("Cannot get fields from options argument.");
    }

    for (size_t ii = 0; ii < options.size(); ++ii) {
      if (known_options.find(options[ii]) == known_options.end()) {
        const size_t tmp_size = 1000;
        char tmp[tmp_size];
        snprintf(tmp, tmp_size, "Unknown option \"%s\"\n", options[ii].c_str());
        mexErrMsgTxt(tmp);
      }
    }

    if (has_field(prhs[4], "verbose")
        && !get_bool_field(prhs[4], "verbose", &opts.verbose)) {
      mexErrMsgTxt("The verbose flag must be a boolean scalar.");
    }

    if (has_field(prhs[4], "max_num_iterations")
        && !get_double_field_as_int(prhs[4], "max_num_iterations",
        &opts.max_num_iterations)) {
      mexErrMsgTxt("The max_num_iterations field must be a double scalar.");
    }

    if (has_field(prhs[4], "tree_layout")
        && !get_string_field(prhs[4], "tree_layout", &layout_string)) {
      mexErrMsgTxt("The tree_layout field must be a string.");
    }

    if (layout_string == "wavelet_tree") {
      opts.layout = binsearch_options::kWaveletTree;
    } else if (layout_string != "complete_tree") {
      mexErrMsgTxt("The layout must be either \"wavelet_tree\" or \"complete_"
          "tree\".");
    }
  }

  vector<bool> support;
  double final_lambda_low;
  double final_lambda_high;
  size_t num_iter;

  bool result = treeapprox_binsearch(x, d, k_low, k_high, opts, &support,
      &final_lambda_low, &final_lambda_high, &num_iter);

  if (!result) {
    mexErrMsgTxt("treeapprox_binsearch returned false.");
  }

  if (nlhs >= 1) {
    set_double_vector(&(plhs[0]), support, is_column_vector);
  }
  if (nlhs >= 2) {
    set_double(&(plhs[1]), final_lambda_low);
  }
  if (nlhs >= 3) {
    set_double(&(plhs[2]), final_lambda_high);
  }
  if (nlhs >= 4) {
    set_double(&(plhs[3]), num_iter);
  }
}
