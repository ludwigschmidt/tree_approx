#include "treeapprox_binsearch.h"

#include <cstdio>
#include <vector>

#include "boost/assign/list_of.hpp"
#include "gtest/gtest.h"

using boost::assign::list_of;
using std::vector;
using treeapprox::binsearch_options;
using treeapprox::treeapprox_binsearch;

template <typename T, size_t N>
T* begin(T(&arr)[N]) { return &arr[0]; }
template <typename T, size_t N>
T* end(T(&arr)[N]) { return &arr[0]+N; }

void WriteToStderr(const char* s) {
  fprintf(stderr, s);
  fflush(stderr);
}

void CheckResult(const vector<bool>& expected_result,
                 const vector<bool>& result) {
  ASSERT_EQ(expected_result.size(), result.size());
  for (size_t ii = 0; ii < expected_result.size(); ++ii) {
    EXPECT_EQ(expected_result[ii], result[ii])
        << "Support mismatch at index " << ii;
  }
}

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
  const double x2[] = {1, 0, 0, 1, 1, 0, 0};
  const bool res2[] = {1, 1, 0, 1, 1, 0, 0};
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

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

