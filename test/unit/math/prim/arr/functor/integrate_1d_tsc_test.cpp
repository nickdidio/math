#include <stan/math.hpp>
#include <gtest/gtest.h>
#include <test/unit/util.hpp>
#include <iostream>
#include <sstream>
#include <vector>

struct f1 {
  template <typename T1, typename T2, typename T3, typename T4>
  inline
  typename stan::return_type<T1, T2, T3, T4>::type
  operator()(const T1& x, const T2& y, const T3& x_r, const T4& x_i,
             std::ostream& msgs) const {
    return exp(x) + y;
  }
};

struct f2 {
  template <typename T1, typename T2, typename T3, typename T4>
  inline
  typename stan::return_type<T1, T2, T3, T4>::type
  operator()(const T1& x, const std::vector<T2>& y, const T3& x_r,
             const T4& x_i, std::ostream& msgs) const {
    return exp(x) + pow(y[0], 2) + pow(y[1], 3);
  }
};

struct f3 {
  template <typename T1, typename T2, typename T3, typename T4>
  inline
  typename stan::return_type<T1, T2, T3, T4>::type
  operator()(const T1& x, const Eigen::Matrix<T2, Eigen::Dynamic, 1>& y,
             const T3& x_r, const T4& x_i, std::ostream& msgs) const {
    return exp(x) + pow(y(0), 2) + pow(y(1), 4) + 3*y(2);
  }
};

struct f4 {
  template <typename T1, typename T2, typename T3, typename T4>
  inline
  typename stan::return_type<T1, T2, T3, T4>::type
  operator()(const T1& x, const std::vector<T2>& y, const T3& x_r,
             const T4& x_i, std::ostream& msgs) const {
    return exp(x) + pow(y.at(0), 2) + pow(y.at(1), 5) + 3*y.at(2);
  }
};

std::ostringstream msgs;

TEST(StanMath_integrate_1d_tsc, test1) {
  using stan::math::integrate_1d_tsc;
  double x_r;
  int x_i;

  f1 if1;

  EXPECT_FLOAT_EQ(integrate_1d_tsc(if1, .2, .7, .5, x_r, x_i, msgs), 1.04235);

  f2 if2;

  EXPECT_FLOAT_EQ(integrate_1d_tsc(if2, -.2, .7,
                                     std::vector<double>(2, .4), x_r, x_i, msgs),
                  1.396622);
  f3 if3;
  Eigen::VectorXd a(3);
  a(0) = 4.0;
  a(1) = 6.0;
  a(2) = 5.1;

  EXPECT_FLOAT_EQ(integrate_1d_tsc(if3, -.2, 2.9, a, x_r, x_i, msgs),
                  4131.985);

  f4 if4;
  std::vector<double> b(3);
  b.at(0) = 4.0;
  b.at(1) = 6.0;
  b.at(2) = 5.1;

  EXPECT_FLOAT_EQ(integrate_1d_tsc(if4, -.2, 2.9, b, x_r, x_i, msgs),
                  24219.99);
}
