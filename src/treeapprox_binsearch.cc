#include "treeapprox_binsearch.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

using std::ceil;
using std::max;

namespace treeapprox {

const int kOutputBufferSize = 10000;
char output_buffer[kOutputBufferSize];

size_t compute_tree_d(const std::vector<double>& x,
                      size_t d,
                      double lambda,
                      size_t last_parent,
                      std::vector<bool>* support,
                      std::vector<double>* subtree_weights,
                      std::vector<size_t>* bfs_queue) {
  std::vector<double>& w = *subtree_weights;
  std::vector<bool>& supp = *support;
  std::vector<size_t>& q = *bfs_queue;

  // compute subtree weights
  for (size_t ii = x.size() - 1; ii > last_parent; --ii) {
    w[ii] = x[ii] - lambda;
  }

  size_t child_index;
  
  // last parent might not be full
  w[last_parent] = x[last_parent] - lambda;
  child_index = last_parent * d;
  for (size_t jj = 1; jj <= d; ++jj) {
    child_index += 1;
    if (child_index >= x.size()) {
      break;
    }
    if (w[child_index] > 0.0) {
      w[last_parent] += w[child_index];
    }
  }

  // other nodes are full
  if (last_parent > 0) {
    for (size_t ii = last_parent - 1; ; --ii) {
      w[ii] = x[ii] - lambda;

      child_index = ii * d;
      for (size_t jj = 1; jj <= d; ++jj) {
        child_index += 1;
        if (w[child_index] > 0.0) {
          w[ii] += w[child_index];
        }
      }
      if (ii == 0) {
        break;
      }
    }
  }

  // compute supports with a BFS starting at the root
  std::fill(supp.begin(), supp.end(), false);
  size_t support_size = 0;
  size_t q_next = 0;
  size_t q_end = 0;
  if (w[0] > 0.0) {
    q[q_end] = 0;
    q_end += 1;
  }

  size_t cur;
  while (q_next < q_end) {
    cur = q[q_next];
    q_next += 1;

    support_size += 1;
    supp[cur] = true;

    if (cur > last_parent) {
      continue;
    }

    child_index = cur * d;

    if (cur == last_parent) {
      for (size_t ii = 1; ii <= d; ++ii) {
        child_index += 1;  
        if (child_index >= x.size()) {
          break;
        }
        if (w[child_index] > 0.0) {
          q[q_end] = child_index;
          q_end += 1;
        }
      }
    } else {
      for (size_t ii = 1; ii <= d; ++ii) {
        child_index += 1;  
        if (w[child_index] > 0.0) {
          q[q_end] = child_index;
          q_end += 1;
        }
      }
    }
  }

  return support_size;
}


size_t compute_tree_wavelet_d(const std::vector<double>& x,
                              size_t d,
                              double lambda,
                              size_t last_parent,
                              std::vector<bool>* support,
                              std::vector<double>* subtree_weights,
                              std::vector<size_t>* bfs_queue) {
  std::vector<double>& w = *subtree_weights;
  std::vector<bool>& supp = *support;
  std::vector<size_t>& q = *bfs_queue;

  // compute subtree weights
  for (size_t ii = x.size() - 1; ii > last_parent; --ii) {
    w[ii] = x[ii] - lambda;
  }

  size_t child_index;
  size_t num_children;
  // last parent might not be full
  w[last_parent] = x[last_parent] - lambda;
  if (last_parent != 0) {
    child_index = last_parent * d;
    num_children = d;
  } else {
    child_index = last_parent * d + 1;
    num_children = d - 1;
  }
  for (size_t jj = 1; jj <= num_children; ++jj) {
    if (child_index >= x.size()) {
      break;
    }
    if (w[child_index] > 0.0) {
      w[last_parent] += w[child_index];
    }
    child_index += 1;
  }

  // other nodes are full
  if (last_parent > 0) {
    for (size_t ii = last_parent - 1; ii > 0; --ii) {
      w[ii] = x[ii] - lambda;

      child_index = ii * d;
      for (size_t jj = 1; jj <= d; ++jj) {
        if (w[child_index] > 0.0) {
          w[ii] += w[child_index];
        }
        child_index += 1;
      }
    }
  }

  // handle root separately (has only d-1 children)
  w[0] = x[0] - lambda;
  for (size_t ii = 1; ii < d; ++ii) {
    if (w[ii] > 0.0) {
      w[0] += w[ii];
    }
  }

  // compute supports with a BFS starting at the root
  std::fill(supp.begin(), supp.end(), false);
  size_t support_size = 0;
  size_t q_next = 0;
  size_t q_end = 0;
  if (w[0] > 0.0) {
    q[q_end] = 0;
    q_end += 1;
  }

  size_t cur;
  while (q_next < q_end) {
    cur = q[q_next];
    q_next += 1;

    support_size += 1;
    supp[cur] = true;

    if (cur > last_parent) {
      continue;
    }

    if (cur == 0) {
      num_children = d - 1;
      child_index = cur * d + 1;
    } else {
      num_children = d;
      child_index = cur * d;
    }

    if (cur == last_parent) {
      for (size_t ii = 1; ii <= num_children; ++ii) {
        if (w[child_index] > 0.0) {
          q[q_end] = child_index;
          q_end += 1;
        }
        child_index += 1;
        if (child_index >= x.size()) {
          break;
        }
      }
    } else {
      for (size_t ii = 1; ii <= num_children; ++ii) {
        if (w[child_index] > 0.0) {
          q[q_end] = child_index;
          q_end += 1;
        }
        child_index += 1;  
      }
    }
  }

  return support_size;
}


size_t compute_tree_2(const std::vector<double>& x,
                      double lambda,
                      size_t last_parent,
                      std::vector<bool>* support,
                      std::vector<double>* subtree_weights,
                      std::vector<size_t>* bfs_queue) {
  std::vector<double>& w = *subtree_weights;
  std::vector<bool>& supp = *support;
  std::vector<size_t>& q = *bfs_queue;

  // compute subtree weights
  for (size_t ii = x.size() - 1; ii > last_parent; --ii) {
    w[ii] = x[ii] - lambda;
  }

  size_t child_index;
  // last parent might not be full
  w[last_parent] = x[last_parent] - lambda;
  child_index = last_parent * 2 + 1;
  if (w[child_index] > 0.0) {
    w[last_parent] += w[child_index];
  }
  child_index += 1;
  if (child_index < x.size() && w[child_index] > 0.0) {
    w[last_parent] += w[child_index];
  }

  if (last_parent > 0) {
      for (size_t ii = last_parent - 1; ; --ii) {
      w[ii] = x[ii] - lambda;

      child_index = 2 * ii + 1;
      if (w[child_index] > 0.0) {
        w[ii] += w[child_index];
      }

      child_index += 1;
      if (w[child_index] > 0.0) {
        w[ii] += w[child_index];
      }

      if (ii == 0) {
        break;
      }
    }
  }

  // compute supports with a BFS starting at the root
  std::fill(supp.begin(), supp.end(), false);
  size_t support_size = 0;
  size_t q_next = 0;
  size_t q_end = 0;
  if (w[0] > 0.0) {
    q[q_end] = 0;
    q_end += 1;
  }

  size_t cur;
  while (q_next < q_end) {
    cur = q[q_next];
    q_next += 1;

    support_size += 1;
    supp[cur] = true;

    if (cur > last_parent) {
      continue;
    }

    child_index = 2 * cur + 1;
    if (w[child_index] > 0.0) {
      q[q_end] = child_index;
      q_end += 1;
    }

    child_index += 1;
    if (cur != last_parent || child_index < x.size()) {
      if (w[child_index] > 0.0) {
        q[q_end] = child_index;
        q_end += 1;
      }
    }
  }

  return support_size;
}


size_t compute_tree_wavelet_2(const std::vector<double>& x,
                              double lambda,
                              size_t last_parent,
                              std::vector<bool>* support,
                              std::vector<double>* subtree_weights,
                              std::vector<size_t>* bfs_queue) {
  std::vector<double>& w = *subtree_weights;
  std::vector<bool>& supp = *support;
  std::vector<size_t>& q = *bfs_queue;

  // compute subtree weights
  for (size_t ii = x.size() - 1; ii > last_parent; --ii) {
    w[ii] = x[ii] - lambda;
  }

  size_t child_index;
  // last parent might not be full
  w[last_parent] = x[last_parent] - lambda;
  if (last_parent == 0) {
    if (w[1] > 0) {
      w[0] += w[1];
    }
  } else {
    child_index = last_parent * 2;
    if (w[child_index] > 0.0) {
      w[last_parent] += w[child_index];
    }
    child_index += 1;
    if (child_index < x.size() && w[child_index] > 0.0) {
      w[last_parent] += w[child_index];
    }

    for (size_t ii = last_parent - 1; ii > 0; --ii) {
      w[ii] = x[ii] - lambda;

      child_index = 2 * ii;
      if (w[child_index] > 0.0) {
        w[ii] += w[child_index];
      }

      child_index += 1;
      if (w[child_index] > 0.0) {
        w[ii] += w[child_index];
      }
    }

    // handle root separately
    w[0] = x[0] - lambda;
    if (w[1] > 0) {
      w[0] += w[1];
    }
  }

  // compute supports with a BFS starting at the root
  std::fill(supp.begin(), supp.end(), false);
  size_t support_size = 0;
  size_t q_next = 0;
  size_t q_end = 0;
  if (w[0] > 0.0) {
    support_size = 1;
    supp[0] = true;
    
    if (1 < x.size() && w[1] > 0.0) {
      q[q_end] = 1;
      q_end += 1;
    }
  } else {
    return 0;
  }

  size_t cur;
  while (q_next < q_end) {
    cur = q[q_next];
    q_next += 1;

    support_size += 1;
    supp[cur] = true;

    if (cur > last_parent) {
      continue;
    }

    child_index = 2 * cur;
    if (w[child_index] > 0.0) {
      q[q_end] = child_index;
      q_end += 1;
    }

    child_index += 1;
    if (cur != last_parent || child_index < x.size()) {
      if (w[child_index] > 0.0) {
        q[q_end] = child_index;
        q_end += 1;
      }
    }
  }

  return support_size;
}


size_t compute_tree_4(const std::vector<double>& x,
                      double lambda,
                      size_t last_parent,
                      std::vector<bool>* support,
                      std::vector<double>* subtree_weights,
                      std::vector<size_t>* bfs_queue) {
  std::vector<double>& w = *subtree_weights;
  std::vector<bool>& supp = *support;
  std::vector<size_t>& q = *bfs_queue;

  // compute subtree weights
  for (size_t ii = x.size() - 1; ii > last_parent; --ii) {
    w[ii] = x[ii] - lambda;
  }

  size_t child_index;

  w[last_parent] = x[last_parent] - lambda;
  child_index = last_parent * 4 + 1;
  if (w[child_index] > 0.0) {
    w[last_parent] += w[child_index];
  }
  child_index += 1;
  if (child_index < x.size()) {
    if (w[child_index] > 0.0) {
      w[last_parent] += w[child_index];
    }

    child_index += 1;
    if (child_index < x.size()) {
      if (w[child_index] > 0.0) {
        w[last_parent] += w[child_index];
      }
      
      child_index += 1;
      if (child_index < x.size()) {
        if (w[child_index] > 0.0) {
          w[last_parent] += w[child_index];
        }
      }
    }
  }

  if (last_parent > 0) {
    for (size_t ii = last_parent - 1; ; --ii) {
      w[ii] = x[ii] - lambda;

      child_index = 4 * ii + 1;
      if (w[child_index] > 0.0) {
        w[ii] += w[child_index];
      }

      child_index += 1;
      if (w[child_index] > 0.0) {
        w[ii] += w[child_index];
      }

      child_index += 1;
      if (w[child_index] > 0.0) {
        w[ii] += w[child_index];
      }

      child_index += 1;
      if (w[child_index] > 0.0) {
        w[ii] += w[child_index];
      }

      if (ii == 0) {
        break;
      }
    }
  }

  // compute supports with a BFS starting at the root
  std::fill(supp.begin(), supp.end(), false);
  size_t support_size = 0;
  size_t q_next = 0;
  size_t q_end = 0;
  if (w[0] > 0.0) {
    q[q_end] = 0;
    q_end += 1;
  }

  size_t cur;
  while (q_next < q_end) {
    cur = q[q_next];
    q_next += 1;

    support_size += 1;
    supp[cur] = true;

    if (cur > last_parent) {
      continue;
    }

    child_index = 4 * cur + 1;
    if (w[child_index] > 0.0) {
      q[q_end] = child_index;
      q_end += 1;
    }
    child_index += 1;

    if (cur == last_parent) {
      if (child_index < x.size()) {
        if (w[child_index] > 0.0) {
          q[q_end] = child_index;
          q_end += 1;
        }

        child_index += 1;
        if (child_index < x.size()) {
          if (w[child_index] > 0.0) {
            q[q_end] = child_index;
            q_end += 1;
          }

          child_index += 1;
          if (child_index < x.size()) {
            if (w[child_index] > 0.0) {
              q[q_end] = child_index;
              q_end += 1;
            }
          }
        }
      }
    } else {
      if (w[child_index] > 0.0) {
        q[q_end] = child_index;
        q_end += 1;
      }

      child_index += 1;
      if (w[child_index] > 0.0) {
        q[q_end] = child_index;
        q_end += 1;
      }

      child_index += 1;
      if ( w[child_index] > 0.0) {
        q[q_end] = child_index;
        q_end += 1;
      }
    }
  }

  return support_size;
}


size_t compute_tree_wavelet_4(const std::vector<double>& x,
                              double lambda,
                              size_t last_parent,
                              std::vector<bool>* support,
                              std::vector<double>* subtree_weights,
                              std::vector<size_t>* bfs_queue) {
  std::vector<double>& w = *subtree_weights;
  std::vector<bool>& supp = *support;
  std::vector<size_t>& q = *bfs_queue;

  // compute subtree weights
  for (size_t ii = x.size() - 1; ii > last_parent; --ii) {
    w[ii] = x[ii] - lambda;
  }

  size_t child_index;

  w[last_parent] = x[last_parent] - lambda;
  if (last_parent == 0) {
    child_index = last_parent * 4 + 1;
    if (w[child_index] > 0.0) {
      w[last_parent] += w[child_index];
    }
    child_index += 1;
    if (child_index < x.size()) {
      if (w[child_index] > 0.0) {
        w[last_parent] += w[child_index];
      }

      child_index += 1;
      if (child_index < x.size()) {
        if (w[child_index] > 0.0) {
          w[last_parent] += w[child_index];
        }
      }
    }
  } else {
    child_index = last_parent * 4;
    if (w[child_index] > 0.0) {
      w[last_parent] += w[child_index];
    }
    child_index += 1;
    if (child_index < x.size()) {
      if (w[child_index] > 0.0) {
        w[last_parent] += w[child_index];
      }

      child_index += 1;
      if (child_index < x.size()) {
        if (w[child_index] > 0.0) {
          w[last_parent] += w[child_index];
        }
        
        child_index += 1;
        if (child_index < x.size()) {
          if (w[child_index] > 0.0) {
            w[last_parent] += w[child_index];
          }
        }
      }
    }
    for (size_t ii = last_parent - 1; ii > 0; --ii) {
      w[ii] = x[ii] - lambda;

      child_index = 4 * ii;
      if (w[child_index] > 0.0) {
        w[ii] += w[child_index];
      }

      child_index += 1;
      if (w[child_index] > 0.0) {
        w[ii] += w[child_index];
      }

      child_index += 1;
      if (w[child_index] > 0.0) {
        w[ii] += w[child_index];
      }

      child_index += 1;
      if (w[child_index] > 0.0) {
        w[ii] += w[child_index];
      }

    }

    // handle root separately
    w[0] = x[0] - lambda;
    if (w[1] > 0.0) {
      w[0] += w[1];
    }
    if (w[2] > 0.0) {
      w[0] += w[2];
    }
    if (w[3] > 0.0) {
      w[0] += w[3];
    }
  }

  // compute supports with a BFS starting at the root
  std::fill(supp.begin(), supp.end(), false);
  size_t support_size = 0;
  size_t q_next = 0;
  size_t q_end = 0;
  if (w[0] > 0.0) {
    support_size = 1;
    supp[0] = true;

    if (1 < x.size()) {
      if (w[1] > 0.0) {
        q[q_end] = 1;
        q_end += 1;
      }

      if (2 < x.size()) {
        if (w[2] > 0.0) {
          q[q_end] = 2;
          q_end += 1;
        }

        if (3 < x.size()) {
          if (w[3] > 0.0) {
            q[q_end] = 3;
            q_end += 1;
          }
        }
      }
    }
  }

  size_t cur;
  while (q_next < q_end) {
    cur = q[q_next];
    q_next += 1;

    support_size += 1;
    supp[cur] = true;

    if (cur > last_parent) {
      continue;
    }

    child_index = 4 * cur;
    if (w[child_index] > 0.0) {
      q[q_end] = child_index;
      q_end += 1;
    }
    child_index += 1;

    if (cur == last_parent) {
      if (child_index < x.size()) {
        if (w[child_index] > 0.0) {
          q[q_end] = child_index;
          q_end += 1;
        }

        child_index += 1;
        if (child_index < x.size()) {
          if (w[child_index] > 0.0) {
            q[q_end] = child_index;
            q_end += 1;
          }

          child_index += 1;
          if (child_index < x.size()) {
            if (w[child_index] > 0.0) {
              q[q_end] = child_index;
              q_end += 1;
            }
          }
        }
      }
    } else {
      if (w[child_index] > 0.0) {
        q[q_end] = child_index;
        q_end += 1;
      }

      child_index += 1;
      if (w[child_index] > 0.0) {
        q[q_end] = child_index;
        q_end += 1;
      }

      child_index += 1;
      if ( w[child_index] > 0.0) {
        q[q_end] = child_index;
        q_end += 1;
      }
    }
  }

  return support_size;
}


size_t compute_tree(const std::vector<double>& x,
                    size_t d,
                    double lambda,
                    binsearch_options::TreeLayout layout,
                    size_t last_parent,
                    std::vector<bool>* support,
                    std::vector<double>* subtree_weights,
                    std::vector<size_t>* bfs_queue) {
  if (layout == binsearch_options::kCompleteTree) {
    if (d == 2) {
      return compute_tree_2(x, lambda, last_parent, support, subtree_weights,
          bfs_queue);
    } else if (d == 4) {
      return compute_tree_4(x, lambda, last_parent, support, subtree_weights,
          bfs_queue);
    } else {
      return compute_tree_d(x, d, lambda, last_parent, support, subtree_weights,
          bfs_queue);
    }
  } else {
    if (d == 2) {
      return compute_tree_wavelet_2(x, lambda, last_parent, support,
          subtree_weights, bfs_queue);
    } else if (d == 4) {
      return compute_tree_wavelet_4(x, lambda, last_parent, support,
          subtree_weights, bfs_queue);
    } else {
      return compute_tree_wavelet_d(x, d, lambda, last_parent, support,
          subtree_weights, bfs_queue);
    }
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
  if (x.size() < d) {
    snprintf(output_buffer, kOutputBufferSize, "Currently, the tree must "
        "have at least d nodes.\n");
    options.output_function(output_buffer);
    return false;
  }

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

  size_t last_parent = 0;

  // last parent is the parent of the last leaf
  if (options.layout == binsearch_options::kCompleteTree) {
    last_parent = (x.size() - 2) / d;
  } else if (options.layout == binsearch_options::kWaveletTree) {
    last_parent = (x.size() - 1) / d;
  }

  std::vector<double> subtree_weights(x.size());
  std::vector<size_t> bfs_queue(x.size());

  size_t num_iter = 0;
  double lambda_mid = 0.0;
  size_t cur_k;
  while (num_iter < max_num_iterations) {
    num_iter += 1;
    lambda_mid = (lambda_low + lambda_high) / 2.0;

    cur_k = compute_tree(x, d, lambda_mid, options.layout, last_parent, support,
                         &subtree_weights, &bfs_queue);

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

  compute_tree(x, d, lambda_high, options.layout, last_parent, support,
               &subtree_weights, &bfs_queue);
  *final_lambda_low = lambda_low;
  *final_lambda_high = lambda_high;
  *num_iterations = num_iter;

  return true;
}

};  // namespace treeapprox
