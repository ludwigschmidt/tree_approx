#include "wavedec2_rearrange.h"

#include <vector>

#include "gtest/gtest.h"

#include "test_helpers.h"

using std::vector;
using treeapprox::wavedec2_rearrange;

void CheckIsInverse(const vector<size_t>& forward,
                    const vector<size_t>& backward) {
  ASSERT_EQ(forward.size(), backward.size());
  for (size_t ii = 0; ii < forward.size(); ++ii) {
    ASSERT_GE(forward[ii], 0);
    ASSERT_LT(forward[ii], forward.size());
    ASSERT_GE(backward[forward[ii]], 0);
    ASSERT_LT(backward[forward[ii]], forward.size());
    EXPECT_EQ(ii, backward[forward[ii]]);
  }
}

void Check256(const vector<size_t>& forward) {
  EXPECT_EQ(64, forward[64]);
  EXPECT_EQ(65, forward[65]);
  EXPECT_EQ(72, forward[66]);
  EXPECT_EQ(73, forward[67]);
  EXPECT_EQ(66, forward[68]);
  EXPECT_EQ(67, forward[69]);
  EXPECT_EQ(74, forward[70]);
  EXPECT_EQ(75, forward[71]);
  EXPECT_EQ(80, forward[80]);
  EXPECT_EQ(81, forward[81]);
  EXPECT_EQ(88, forward[82]);
  EXPECT_EQ(89, forward[83]);
}

void RunAlgo(size_t n, const vector<size_t> expected_forward) {
  vector<size_t> forward;
  vector<size_t> backward;
  ASSERT_TRUE(wavedec2_rearrange(n, &forward, &backward));
  CheckResult(expected_forward, forward);
  CheckIsInverse(forward, backward);
}

void RunAlgo(size_t n,
             const vector<size_t> expected_forward_prefix,
             vector<size_t>* forward_result) {
  vector<size_t> backward;
  ASSERT_TRUE(wavedec2_rearrange(n, forward_result, &backward));
  CheckResultPrefix(expected_forward_prefix, *forward_result);
  CheckIsInverse(*forward_result, backward);
}

TEST(Wavedec2RearrangeTest, Test1) {
  const size_t f2[] = {0};
  vector<size_t> f(begin(f2), end(f2));
  RunAlgo(1, f);
}

TEST(Wavedec2RearrangeTest, Test4) {
  const size_t f2[] = {0, 1, 2, 3};
  vector<size_t> f(begin(f2), end(f2));
  RunAlgo(4, f);
}

TEST(Wavedec2RearrangeTest, Test16) {
  const size_t f2[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  vector<size_t> f(begin(f2), end(f2));
  RunAlgo(16, f);
}

const size_t prefix64[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 20, 21, 18, 19, 22, 23, 24, 25, 28, 29, 26, 27, 30, 31,
    32, 33, 36, 37, 34, 35, 38, 39, 40, 41, 44, 45, 42, 43, 46, 47,
    48, 49, 52, 53, 50, 51, 54, 55, 56, 57, 60, 61, 58, 59, 62, 63};

TEST(Wavedec2RearrangeTest, Test64) {
  vector<size_t> f(begin(prefix64), end(prefix64));
  RunAlgo(64, f);
}

TEST(Wavedec2RearrangeTest, Test256) {
  vector<size_t> f(begin(prefix64), end(prefix64));
  vector<size_t> forward;
  RunAlgo(256, f, &forward);
  Check256(forward);
}

TEST(Wavedec2RearrangeTest, Test4096) {
  vector<size_t> f(begin(prefix64), end(prefix64));
  vector<size_t> forward;
  RunAlgo(4096, f, &forward);
  Check256(forward);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
