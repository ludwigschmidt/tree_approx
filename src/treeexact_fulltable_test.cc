#include "treeexact_fulltable.h"

#include <vector>

#include "gtest/gtest.h"

#include "test_helpers.h"

using std::vector;
using treeapprox::treeexact_fulltable;

void RunAlgo(const vector<double>& x,
             size_t d,
             size_t k,
             const vector<bool>& expected_result) {
  vector<bool> result;
  ASSERT_TRUE(treeexact_fulltable(x, d, k, &result));
  CheckResult(expected_result, result);
}

// d = 2

TEST(TreeexactFulltableTest, SimpleBinaryTest) {
  const double x2[] = {1, 1, 0, 1, 1, 0, 0};
  const bool res2[] = {1, 1, 0, 1, 1, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 4, res);
}

TEST(TreeexactFulltableTest, SimpleEmptyNodeTest) {
  const double x2[] = {1, 0, 0, 1, 1, 0, 0};
  const bool res2[] = {1, 1, 0, 1, 1, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 4, res);
}

TEST(TreeexactFulltableTest, SimpleDecisionTest) {
  const double x2[] = {1, 0, 0, 2, 3, 0, 0};
  const bool res2[] = {1, 1, 0, 0, 1, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 3, res);
}

TEST(TreeexactFulltableTest, SimpleNoDecisionTest) {
  const double x2[] = {1, 0, 0, 0, 0, 2, 3};
  const bool res2[] = {1, 0, 1, 0, 0, 1, 1};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 4, res);
}

TEST(TreeexactFulltableTest, SimpleAlmostFullTest) {
  const double x2[] = {1, 1, 1, 1, 1, 1, 0};
  const bool res2[] = {1, 1, 1, 1, 1, 1, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 6, res);
}

TEST(TreeexactFulltableTest, NotFullTree) {
  // Layout the memory so that the non-existing child has a large value.
  const double x2[] = {1, 0, 1, 0, 0, 1, 100};
  vector<double> x(begin(x2), end(x2));
  x.resize(6);
  const bool res2[] = {1, 0, 1, 0, 0, 1};
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 3, res);
}

TEST(TreeexactFulltableTest, MediumSizeTest) {
  const double x2[] = {50, 97, 50, 43, 42, 45, 75, 73, 48, 50, 78, 57, 89,
                       69, 38, 17, 52, 31, 81, 68,  8, 96, 41, 79, 62, 69, 54,
                       51, 82, 94, 38};
  const bool res2[] = {1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0,
                       1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
                       0, 1, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 9, res);
}


// d = 3

TEST(TreeexactFulltableTest, SimpleTest3) {
  const double x2[] = {1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0};
  const bool res2[] = {1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 3, 4, res);
}

TEST(TreeexactFulltableTest, SimpleDecisionTest3) {
  const double x2[] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0};
  const bool res2[] = {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 3, 4, res);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
