#include "treeexact_smalltable_wvtree.h"

#include <vector>

#include "gtest/gtest.h"

#include "test_helpers.h"

using std::vector;
using treeapprox::treeexact_smalltable_wvtree;

void RunAlgo(const vector<double>& x,
             size_t d,
             size_t k,
             const vector<bool>& expected_result) {
  vector<bool> result;
  ASSERT_TRUE(treeexact_smalltable_wvtree(x, d, k, &result));
  CheckResult(expected_result, result);
}

// d = 2

TEST(TreeexactSmalltableWvtreeTest, SimpleBinaryTest) {
  const double x2[] = {0, 1, 1, 0, 1, 1, 0, 0};
  const bool res2[] = {1, 1, 1, 0, 1, 1, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 5, res);
}

TEST(TreeexactSmalltableWvtreeTest, SimpleEmptyNodeTest) {
  const double x2[] = {1, 1, 0, 0, 1, 1, 0, 0};
  const bool res2[] = {1, 1, 1, 0, 1, 1, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 5, res);
}

TEST(TreeexactSmalltableWvtreeTest, SimpleDecisionTest) {
  const double x2[] = {0, 1, 0, 0, 2, 3, 0, 0};
  const bool res2[] = {1, 1, 1, 0, 0, 1, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 4, res);
}

TEST(TreeexactSmalltableWvtreeTest, SimpleNoDecisionTest) {
  const double x2[] = {0, 1, 0, 0, 0, 0, 2, 3};
  const bool res2[] = {1, 1, 0, 1, 0, 0, 1, 1};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 5, res);
}

TEST(TreeexactSmalltableWvtreeTest, SimpleAlmostFullTest) {
  const double x2[] = {1, 1, 1, 1, 1, 1, 1, 0};
  const bool res2[] = {1, 1, 1, 1, 1, 1, 1, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 7, res);
}

TEST(TreeexactSmalltableWvtreeTest, NotFullTree) {
  // Layout the memory so that the non-existing child has a large value.
  const double x2[] = {1, 1, 0, 1, 0, 0, 1, 100};
  vector<double> x(begin(x2), end(x2));
  x.resize(7);
  const bool res2[] = {1, 1, 0, 1, 0, 0, 1};
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 5, res);
}

TEST(TreeexactSmalltableWvtreeTest, MediumSizeTest) {
  const double x2[] = {0, 50, 97, 50, 43, 42, 45, 75, 73, 48, 50, 78, 57, 89,
                       69, 38, 17, 52, 31, 81, 68,  8, 96, 41, 79, 62, 69, 54,
                       51, 82, 94, 38};
  const bool res2[] = {1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0,
                       1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
                       0, 1, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 10, res);
}


// d = 3

TEST(TreeexactSmalltableWvtreeTest, SimpleTest3) {
  const double x2[] = {0, 1, 0, 0, 1, 1, 0, 0, 0};
  const bool res2[] = {1, 1, 0, 0, 1, 1, 0, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 3, 4, res);
}

TEST(TreeexactSmalltableWvtreeTest, SimpleDecisionTest3) {
  const double x2[] = {0, 1, 0, 0, 0, 0, 0, 1, 1};
  const bool res2[] = {1, 0, 1, 0, 0, 0, 0, 1, 1};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 3, 4, res);
}

TEST(TreeexactSmalltableWvtreeTest, FourLevelTest3) {
  const double x2[] = {0, 1, 0, 0, 1, 1, 0, 0, 0,
                       0, 0, 0, 1, 0, 1, 0, 0, 0,
                       0, 0, 0, 0, 0, 1, 0, 0, 9};
  const bool res2[] = {1, 1, 1, 0, 1, 1, 0, 0, 1,
                       0, 0, 0, 1, 0, 1, 0, 0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0, 1};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 3, 9, res);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
