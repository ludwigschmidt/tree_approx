#ifndef __REARRANGE_WAVEDEC2_H__
#define __REARRANGE_WAVEDEC2_H__

#include <algorithm>
#include <cstdio>
#include <vector>

namespace treeapprox {

// returns two arrays for rearranging data from Matlab.
// when the data is rearranged via data[ii] = data_matlab[forward[ii]],
// the following properties hold:
// - the root is at index 0
// - the children of node x are at indices 4 * x, ..., 4 * x + 3  (for x != 0)

bool rearrange_wavedec2(size_t n,
                        std::vector<size_t>* forward,
                        std::vector<size_t>* backward) {
  // check if n is a power of 4
  size_t tmp = 1;
  size_t num_levels = 1;
  while (tmp < n) {
    tmp *= 4;
    num_levels += 1;
  }
  if (tmp != n) {
    return false;
  }

  if (forward != NULL) {
    forward->resize(n);
  }
  if (backward != NULL) {
    backward->resize(n);
  }

  size_t num_identity = std::min(n, 16ul);
  for (size_t ii = 0; ii < num_identity; ++ii) {
    if (forward != NULL) {
      (*forward)[ii] = ii;
    }
    if (backward != NULL) {
      (*backward)[ii] = ii;
    }
  }

  // we process the four children of node ii
  size_t ii = 4;
  size_t anchor = ii;
  size_t next_anchor = anchor * 4;
  size_t block_edge = 2;
  size_t ichild = 0;
  for (size_t ilevel = 2; ilevel < num_levels - 1; ++ilevel) {
    for (size_t iblock = 0; iblock < 3; ++iblock) {
      anchor = ii;
      next_anchor = anchor * 4;
      for (size_t x_offset = 0; x_offset < block_edge; ++x_offset) {
        for (size_t y_offset = 0; y_offset < block_edge; ++y_offset) {
          ichild = next_anchor + x_offset * 2 * (2 * block_edge) + 2 * y_offset;
          if (forward != NULL) {
            (*forward)[4 * ii] = ichild;
            ichild += 1;
            (*forward)[4 * ii + 1] = ichild;
            ichild = ichild - 1 + (2 * block_edge);
            (*forward)[4 * ii + 2] = ichild;
            ichild += 1;
            (*forward)[4 * ii + 3] = ichild;
          }

          ichild = next_anchor + x_offset * 2 * (2 * block_edge) + 2 * y_offset;
          if (backward != NULL) {
            (*backward)[ichild] = 4 * ii;
            ichild += 1;
            (*backward)[ichild] = 4 * ii + 1;
            ichild = ichild - 1 + (2 * block_edge);
            (*backward)[ichild] = 4 * ii + 2;
            ichild += 1;
            (*backward)[ichild] = 4 * ii + 3;
          }

          ii += 1;
        }
      }
    }
    block_edge *= 2;
  }

  return true;
}

};  // namespace treeapprox

#endif
