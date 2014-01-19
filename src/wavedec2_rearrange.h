#ifndef __WAVEDEC2_REARRANGE_H__
#define __WAVEDEC2_REARRANGE_H__

#include <algorithm>
#include <cstdio>
#include <vector>

namespace treeapprox {

// returns two arrays for rearranging data from Matlab.
// when the data is rearranged via data[ii] = data_matlab[forward[ii]],
// the following properties hold:
// - the root is at index 0
// - the children of node x are at indices 4 * x, ..., 4 * x + 3  (for x != 0)

bool wavedec2_rearrange(size_t n,
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

  std::vector<size_t>& forward_ref = *forward;
  std::vector<size_t>& backward_ref = *backward;
  forward_ref.resize(n);
  backward_ref.resize(n);

  size_t num_identity = std::min(n, 16ul);
  for (size_t ii = 0; ii < num_identity; ++ii) {
    forward_ref[ii] = ii;
    backward_ref[ii] = ii;
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
          forward_ref[4 * ii] = ichild;
          backward_ref[ichild] = 4 * ii;

          ichild += 1;
          forward_ref[4 * ii + 1] = ichild;
          backward_ref[ichild] = 4 * ii + 1;

          ichild = ichild - 1 + (2 * block_edge);
          forward_ref[4 * ii + 2] = ichild;
          backward_ref[ichild] = 4 * ii + 2;

          ichild += 1;
          forward_ref[4 * ii + 3] = ichild;
          backward_ref[ichild] = 4 * ii + 3;

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
