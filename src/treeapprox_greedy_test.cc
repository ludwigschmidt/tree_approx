#include "treeapprox_greedy.h"

#include <vector>

#include "gtest/gtest.h"

#include "test_helpers.h"

using std::vector;
using treeapprox::treeapprox_greedy;

void RunAlgo(const vector<double>& x,
             size_t d,
             size_t k,
             const vector<bool>& expected_result) {
  vector<bool> result;
  ASSERT_TRUE(treeapprox_greedy(x, d, k, &result));
  CheckResult(expected_result, result);
}

// d = 2

TEST(TreeapproxGreedyTest, SimpleBinaryTest) {
  const double x2[] = {0, 1, 1, 0, 1, 1, 0, 0};
  const bool res2[] = {1, 1, 1, 0, 1, 1, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 5, res);
}

TEST(TreeapproxGreedyTest, SimpleEmptyNodeTest) {
  const double x2[] = {1, 1, 0, 0.1, 1, 1, 0.1, 0.1};
  const bool res2[] = {1, 1, 0, 1, 0, 0, 1, 1};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 5, res);
}

TEST(TreeapproxGreedyTest, SimpleDecisionTest) {
  const double x2[] = {0, 1, 2.5, 2.5, 2, 3, 0, 0};
  const bool res2[] = {1, 1, 1, 1, 0, 1, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 5, res);
}

TEST(TreeapproxGreedyTest, SimpleNoDecisionTest) {
  const double x2[] = {0, 1, 0, 0, 0, 0, 2, 3};
  const bool res2[] = {1, 1, 0, 1, 0, 0, 1, 1};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 5, res);
}

TEST(TreeapproxGreedyTest, SimpleAlmostFullTest) {
  const double x2[] = {1, 1, 1, 1, 1, 1, 1, 0};
  const bool res2[] = {1, 1, 1, 1, 1, 1, 1, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 7, res);
}

TEST(TreeapproxGreedyTest, NotFullTree) {
  // Layout the memory so that the non-existing child has a large value.
  const double x2[] = {1, 1, 0.1, 1, 0, 0, 1, 100};
  vector<double> x(begin(x2), end(x2));
  x.resize(7);
  const bool res2[] = {1, 1, 1, 1, 0, 0, 1};
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 5, res);
}

TEST(TreeapproxGreedyTest, MediumSizeTest) {
  const double x2[] = {0, 50, 97, 50, 43, 42, 45, 75, 73, 48, 50, 78, 57, 89,
                       69, 38, 17, 52, 31, 81, 68,  8, 96, 41, 79, 62, 69, 54,
                       51, 82, 94, 38};
  const bool res2[] = {1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1,
                       1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                       1, 1, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 10, res);
}


// d = 3

TEST(TreeapproxGreedyTest, SimpleTest3) {
  const double x2[] = {0, 1, 0, 0, 1, 1, 0, 0, 0};
  const bool res2[] = {1, 1, 0, 0, 1, 1, 0, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 3, 4, res);
}

TEST(TreeapproxGreedyTest, SimpleDecisionTest3) {
  const double x2[] = {0, 0, 1, 0, 0, 0, 0, 1, 1};
  const bool res2[] = {1, 0, 1, 0, 0, 0, 0, 1, 1};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 3, 4, res);
}

TEST(TreeapproxGreedyTest, FourLevelTest3) {
  const double x2[] = {0, 1, 0, 0, 1, 1, 0, 0, 0,
                       0, 0, 0, 1, 0, 1, 0, 0, 1,
                       0, 0, 0, 0, 0, 1, 0, 0, 9};
  const bool res2[] = {1, 1, 0, 0, 1, 1, 0, 0, 0,
                       0, 0, 0, 1, 0, 1, 0, 0, 1,
                       0, 0, 0, 0, 0, 0, 0, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 3, 7, res);
}


// d = 4

TEST(TreeapproxGreedyTest, SimpleTest4) {
  const double x2[] = {1, 0, 1, 0,
                       0, 1, 0, 1, 0.1, 0.1, 0.1, 0.1, 0, 0, 0, 1};
  const bool res2[] = {1, 0, 1, 0,
                       0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 4, 6, res);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
