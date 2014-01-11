#include <algorithm>
#include <cmath>
#include <cstdio>

#include "treeapprox_binsearch.h"

using std::ceil;
using std::max;

namespace treeapprox {

const int kOutputBufferSize = 10000;
char output_buffer[kOutputBufferSize];

size_t compute_tree_d(const std::vector<double>& x,
                      size_t d,
                      double lambda,
                      std::vector<bool>* support,
                      std::vector<double>* subtree_weights,
                      std::vector<size_t>* bfs_queue) {
  std::vector<double>& w = *subtree_weights;
  std::vector<bool>& supp = *support;
  std::vector<size_t>& q = *bfs_queue;

  // compute subtree weights
  size_t last_parent = static_cast<size_t>(floor(
      static_cast<double>(x.size() - 2) / d));
  for (size_t ii = x.size() - 1; ii > last_parent; --ii) {
    w[ii] = x[ii] - lambda;
  }

  size_t child_index;
  for (size_t ii = last_parent; ; --ii) {
    w[ii] = x[ii] - lambda;

    child_index = ii * d;
    for (size_t jj = 1; jj <= d; ++jj) {
      child_index += 1;
      if (child_index < x.size()) {
        if (w[child_index] > 0.0) {
          w[ii] += w[child_index];
        }
      } else {
        break;
      }
    }
    if (ii == 0) {
      break;
    }
  }

  // compute supports with a BFS starting at the root
  std::fill(supp.begin(), supp.end(), false);
  size_t support_size = 0;
  size_t q_next = -1;
  size_t q_end = 0;
  if (w[0] > 0.0) {
    q_next += 1;
    q[q_next] = 0;
  }

  size_t cur;
  while (q_next <= q_end) {
    cur = q[q_next];
    q_next += 1;

    support_size += 1;
    supp[cur] = true;

    child_index = cur * d;
    for (size_t ii = 1; ii <= d; ++ii) {
      child_index += 1;  
      if (child_index < x.size()) {
        if (w[child_index] > 0.0) {
          q_end += 1;
          q[q_end] = child_index;
        }
      } else {
        break;
      }
    }
  }

  return support_size;
}


size_t compute_tree_2(const std::vector<double>& x,
                      double lambda,
                      std::vector<bool>* support,
                      std::vector<double>* subtree_weights,
                      std::vector<size_t>* bfs_queue) {
  std::vector<double>& w = *subtree_weights;
  std::vector<bool>& supp = *support;
  std::vector<size_t>& q = *bfs_queue;

  // compute subtree weights
  size_t last_parent = static_cast<size_t>(floor(
      static_cast<double>(x.size() - 2) / 2));
  for (size_t ii = x.size() - 1; ii > last_parent; --ii) {
    w[ii] = x[ii] - lambda;
  }

  size_t child_index;
  for (size_t ii = last_parent; ; --ii) {
    w[ii] = x[ii] - lambda;

    child_index = 2 * ii + 1;
    if (child_index < x.size()) {
      if (w[child_index] > 0.0) {
        w[ii] += w[child_index];
      }

      child_index += 1;
      if (child_index < x.size() && w[child_index] > 0.0) {
        w[ii] += w[child_index];
      }
    }
    if (ii == 0) {
      break;
    }
  }

  // compute supports with a BFS starting at the root
  std::fill(supp.begin(), supp.end(), false);
  size_t support_size = 0;
  size_t q_next = -1;
  size_t q_end = 0;
  if (w[0] > 0.0) {
    q_next += 1;
    q[q_next] = 0;
  }

  size_t cur;
  while (q_next <= q_end) {
    cur = q[q_next];
    q_next += 1;

    support_size += 1;
    supp[cur] = true;

    child_index = 2 * cur + 1;
    if (child_index < x.size()) {
      if (w[child_index] > 0.0) {
        q_end += 1;
        q[q_end] = child_index;
      }

      child_index += 1;
      if (child_index < x.size() && w[child_index] > 0.0) {
        q_end += 1;
        q[q_end] = child_index;
      }
    }
  }

  return support_size;
}


size_t compute_tree_4(const std::vector<double>& x,
                      double lambda,
                      std::vector<bool>* support,
                      std::vector<double>* subtree_weights,
                      std::vector<size_t>* bfs_queue) {
  std::vector<double>& w = *subtree_weights;
  std::vector<bool>& supp = *support;
  std::vector<size_t>& q = *bfs_queue;

  // compute subtree weights
  size_t last_parent = static_cast<size_t>(floor(
      static_cast<double>(x.size() - 2) / 4));
  for (size_t ii = x.size() - 1; ii > last_parent; --ii) {
    w[ii] = x[ii] - lambda;
  }

  size_t child_index;
  for (size_t ii = last_parent; ; --ii) {
    w[ii] = x[ii] - lambda;

    child_index = 4 * ii + 1;
    if (child_index < x.size()) {
      if (w[child_index] > 0.0) {
        w[ii] += w[child_index];
      }

      child_index += 1;
      if (child_index < x.size()) {
        if (w[child_index] > 0.0) {
          w[ii] += w[child_index];
        }

        child_index += 1;
        if (child_index < x.size()) {
          if (w[child_index] > 0.0) {
            w[ii] += w[child_index];
          }

          child_index += 1;
          if (child_index < x.size() && w[child_index] > 0.0) {
            w[ii] += w[child_index];
          }
        }
      }
    }
    if (ii == 0) {
      break;
    }
  }

  // compute supports with a BFS starting at the root
  std::fill(supp.begin(), supp.end(), false);
  size_t support_size = 0;
  size_t q_next = -1;
  size_t q_end = 0;
  if (w[0] > 0.0) {
    q_next += 1;
    q[q_next] = 0;
  }

  size_t cur;
  while (q_next <= q_end) {
    cur = q[q_next];
    q_next += 1;

    support_size += 1;
    supp[cur] = true;

    child_index = 4 * cur + 1;
    if (child_index < x.size()) {
      if (w[child_index] > 0.0) {
        q_end += 1;
        q[q_end] = child_index;
      }

      child_index += 1;
      if (child_index < x.size()) {
        if (w[child_index] > 0.0) {
          q_end += 1;
          q[q_end] = child_index;
        }

        child_index += 1;
        if (child_index < x.size()) {
          if (w[child_index] > 0.0) {
            q_end += 1;
            q[q_end] = child_index;
          }

          child_index += 1;
          if (child_index < x.size() && w[child_index] > 0.0) {
            q_end += 1;
            q[q_end] = child_index;
          }
        }
      }
    }
  }

  return support_size;
}


size_t compute_tree(const std::vector<double>& x,
                    size_t d,
                    double lambda,
                    std::vector<bool>* support,
                    std::vector<double>* subtree_weights,
                    std::vector<size_t>* bfs_queue) {
  if (d == 2) {
    return compute_tree_2(x, lambda, support, subtree_weights, bfs_queue);
  } else if (d == 4) {
    return compute_tree_4(x, lambda, support, subtree_weights, bfs_queue);
  } else {
    return compute_tree_d(x, d, lambda, support, subtree_weights, bfs_queue);
  }
}


bool treeapprox_binsearch(const std::vector<double>& x,
                          size_t d,
                          size_t k_low,
                          size_t k_high,
                          const binsearch_options& options,
                          std::vector<bool>* support,
                          double* final_lambda_low,
                          double* final_lambda_high,
                          size_t* num_iterations) {
  support->resize(x.size());

  size_t max_num_iterations = 0;
  if (options.max_num_iterations < 0) {
    max_num_iterations = 2 * static_cast<size_t>(ceil(log2(x.size())));
  } else {
    max_num_iterations = options.max_num_iterations;
  }
  // TODO: uncomment
  /*if (options.lambda_low < 0) {
    new_options.lambda_low = 0.0;
  }
  if (options.lambda_high < 0) {
    new_options.lambda_high = 0.0;
    for (size_t ii = 0; ii < x.size(); ++ii) {
      new_options.lambda_high = max(new_options.lambda_high, x[ii]);
    }
  }*/
  if (options.verbose) {
    if (options.output_function == NULL) {
      return false;
    }
  }
  if (k_low > k_high) {
    snprintf(output_buffer, kOutputBufferSize, "Error: k_low > k_high.\n");
    options.output_function(output_buffer);
    return false;
  }
  if (k_low > x.size()) {
    snprintf(output_buffer, kOutputBufferSize, "Error: k_low > n.\n");
    options.output_function(output_buffer);
    return false;
  }
  if (k_high > x.size()) {
    snprintf(output_buffer, kOutputBufferSize, "Error: k_high > n.\n");
    options.output_function(output_buffer);
    return false;
  }


  // TODO: use options
  double lambda_low = 0.0;
  double lambda_high = 0.0;
  for (size_t ii = 0; ii < x.size(); ++ii) {
    lambda_high = max(lambda_high, x[ii]);
  }

  if (options.verbose) {
    snprintf(output_buffer, kOutputBufferSize, "n: %lu  d: %lu  k_low: %lu  "
        "k_high: %lu  l_low: %e  l_high: %e  max_num_iter: %lu\n", x.size(), d,
        k_low, k_high, lambda_low, lambda_high, max_num_iterations);
    options.output_function(output_buffer);
  }


  std::vector<double> subtree_weights(x.size());
  std::vector<size_t> bfs_queue(x.size());

  size_t num_iter = 0;
  double lambda_mid = 0.0;
  size_t cur_k;
  while (num_iter < max_num_iterations) {
    num_iter += 1;
    lambda_mid = (lambda_low + lambda_high) / 2.0;

    cur_k = compute_tree(x, d, lambda_mid, support, &subtree_weights,
        &bfs_queue);

    if (options.verbose) {
      snprintf(output_buffer, kOutputBufferSize, "l_cur: %e  (l_low: %e, "
          "l_high: %e)  k: %lu\n", lambda_mid, lambda_low, lambda_high, cur_k);
      options.output_function(output_buffer);
    }

    if (cur_k <= k_high && cur_k >= k_low) {
      *final_lambda_low = lambda_low;
      *final_lambda_high = lambda_high;
      *num_iterations = num_iter;
      return true;
    }

    if (cur_k > k_high) {
      lambda_low = lambda_mid;
    } else {
      lambda_high = lambda_mid;
    }
  }

  compute_tree(x, d, lambda_high, support, &subtree_weights, &bfs_queue);
  *final_lambda_low = lambda_low;
  *final_lambda_high = lambda_high;
  *num_iterations = num_iter;

  return true;
}

};  // namespace treeapprox
