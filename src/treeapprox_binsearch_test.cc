#include "treeapprox_binsearch.h"

#include <vector>

#include "gtest/gtest.h"

#include "test_helpers.h"

using std::vector;
using treeapprox::binsearch_options;
using treeapprox::treeapprox_binsearch;

void RunAlgo(const vector<double>& x,
             size_t d,
             size_t k_low,
             size_t k_high,
             const vector<bool>& expected_result) {
  binsearch_options opts;
  opts.verbose = true;
  opts.output_function = WriteToStderr;

  vector<bool> result;
  double llow;
  double lhigh;
  size_t numiter;
  
  ASSERT_TRUE(treeapprox_binsearch(x, d, k_low, k_high, opts, &result, &llow,
                                   &lhigh, &numiter));
  CheckResult(expected_result, result);
}

// d = 2

TEST(TreeapproxBinsearchTest, SimpleBinaryTest) {
  const double x2[] = {1, 1, 0, 1, 1, 0, 0};
  const bool res2[] = {1, 1, 0, 1, 1, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 4, 5, res);
}

TEST(TreeapproxBinsearchTest, SimpleBinaryTest2) {
  const double x2[] = {10, 10, 7, 10, 10, 7, 7};
  const bool res2[] = {1, 1, 0, 1, 1, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 4, 5, res);
}

TEST(TreeapproxBinsearchTest, EmptyParentTest) {
  const double x2[] = {1, 0, 0, 0, 0, 1, 1};
  const bool res2[] = {1, 0, 1, 0, 0, 1, 1};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 4, 5, res);
}

TEST(TreeapproxBinsearchTest, NotConvexTest) {
  const double x2[] = {1, 0, 0, 2, 3, 0, 0};
  const bool res2[] = {0, 0, 0, 0, 0, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 3, 3, res);
}

TEST(TreeapproxBinsearchTest, NotConvexTest2) {
  const double x2[] = {1, 0, 0, 2, 3, 0, 0};
  const bool res2[] = {1, 1, 0, 1, 1, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 3, 4, res);
}

TEST(TreeexactSmalltableTest, NotFullTree) {
  // Layout the memory so that the non-existing child has a large value.
  const double x2[] = {1, 0, 1, 0, 0, 1, 100};
  vector<double> x(begin(x2), end(x2));
  x.resize(6);
  const bool res2[] = {1, 0, 1, 0, 0, 1};
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 2, 3, 4, res);
}

// d = 3

TEST(TreeapproxBinsearchTest, SimpleBinaryTestD3) {
  const double x2[] = {1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0};
  const bool res2[] = {1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 3, 4, 5, res);
}

TEST(TreeapproxBinsearchTest, SimpleBinaryTest2D3) {
  const double x2[] = {10, 10, 7, 7, 10, 10, 7, 7, 7, 7, 7, 7, 7};
  const bool res2[] = {1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 3, 4, 5, res);
}

TEST(TreeapproxBinsearchTest, EmptyParentTestD3) {
  const double x2[] = {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0};
  const bool res2[] = {1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 3, 4, 5, res);
}

TEST(TreeapproxBinsearchTest, NotConvexTestD3) {
  const double x2[] = {1, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0};
  const bool res2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 3, 3, 3, res);
}

TEST(TreeapproxBinsearchTest, NotConvexTest2D3) {
  const double x2[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 3};
  const bool res2[] = {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 3, 3, 4, res);
}

TEST(TreeexactSmalltableTest, NotFullTreeD3) {
  // Layout the memory so that the non-existing child has a large value.
  const double x2[] = {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 100};
  vector<double> x(begin(x2), end(x2));
  x.resize(12);
  const bool res2[] = {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1};
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 3, 3, 4, res);
}

// d = 4

TEST(TreeapproxBinsearchTest, SimpleBinaryTestD4) {
  const double x2[] = {1, 1, 0, 0, 0,
                       1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  const bool res2[] = {1, 1, 0, 0, 0,
                       1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 4, 4, 5, res);
}

TEST(TreeapproxBinsearchTest, SimpleBinaryTest2D4) {
  const double x2[] = {10, 7, 10, 7, 7,
                       7, 7, 7, 7, 10, 10, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7};
  const bool res2[] = {1, 0, 1, 0, 0,
                       0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 4, 4, 5, res);
}

TEST(TreeapproxBinsearchTest, EmptyParentTestD4) {
  const double x2[] = {1, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0};
  const bool res2[] = {1, 0, 0, 1, 0,
                       0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 4, 4, 5, res);
}

TEST(TreeapproxBinsearchTest, NotConvexTestD4) {
  const double x2[] = {1, 0, 0, 0, 0,
                       2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  const bool res2[] = {0, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 4, 3, 3, res);
}

TEST(TreeapproxBinsearchTest, NotConvexTest2D4) {
  const double x2[] = {1, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0};
  const bool res2[] = {1, 0, 1, 0, 0,
                       0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
  vector<double> x(begin(x2), end(x2));
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 4, 3, 4, res);
}

TEST(TreeexactSmalltableTest, NotFullTreeD4) {
  // Layout the memory so that the non-existing child has a large value.
  const double x2[] = {1, 0, 0, 0, 1,
                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 100};
  vector<double> x(begin(x2), end(x2));
  x.resize(20);
  const bool res2[] = {1, 0, 0, 0, 1,
                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  vector<bool> res(begin(res2), end(res2));
  RunAlgo(x, 4, 3, 4, res);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

