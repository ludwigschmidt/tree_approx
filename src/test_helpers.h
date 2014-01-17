#ifndef __TEST_HELPERS_H___
#define __TEST_HELPERS_H__

#include <cstdio>
#include <vector>

#include "gtest/gtest.h"

template <typename T, size_t N>
T* begin(T(&arr)[N]) { return &arr[0]; }
template <typename T, size_t N>
T* end(T(&arr)[N]) { return &arr[0]+N; }

void WriteToStderr(const char* s) {
  fprintf(stderr, s);
  fflush(stderr);
}

void CheckResult(const std::vector<bool>& expected_result,
                 const std::vector<bool>& result) {
  ASSERT_EQ(expected_result.size(), result.size());
  for (size_t ii = 0; ii < expected_result.size(); ++ii) {
    EXPECT_EQ(expected_result[ii], result[ii])
        << "Support mismatch at index " << ii;
  }
}

#endif
