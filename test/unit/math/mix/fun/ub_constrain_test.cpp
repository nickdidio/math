#include <test/unit/math/test_ad.hpp>
#include <limits>

namespace ub_constrain_test {
template <typename T1, typename T2>
void expect_ub_constrain(const T1& x, const T2& ub) {
  auto f1 = [](const auto& x, const auto& ub) {
    return stan::math::ub_constrain(x, ub);
  };
  auto f2 = [](const auto& x, const auto& ub) {
    stan::return_type_t<decltype(x), decltype(ub)> lp = 0;
    return stan::math::ub_constrain(x, ub, lp);
  };
  auto f3 = [](const auto& x, const auto& ub) {
    stan::return_type_t<decltype(x), decltype(ub)> lp = 0;
    stan::math::ub_constrain(x, ub, lp);
    return lp;
  };

  auto f4 = [](const auto& x, const auto& ub) {
    stan::return_type_t<decltype(x), decltype(ub)> lp = 0;
    auto xx = stan::math::ub_constrain(x, ub, lp);
    return stan::math::add(lp, stan::math::sum(xx));
  };
  stan::test::expect_ad(f1, x, ub);
  stan::test::expect_ad(f2, x, ub);
  stan::test::expect_ad(f3, x, ub);
  stan::test::expect_ad(f4, x, ub);
}
}  // namespace ub_constrain_test

TEST(mathMixScalFun, ub_constrain) {
  ub_constrain_test::expect_ub_constrain(-1, 2);
  ub_constrain_test::expect_ub_constrain(2, 4);
}

TEST(mathMixScalFun, ub_constrain_inf) {
  ub_constrain_test::expect_ub_constrain(-1, stan::math::INFTY);
  ub_constrain_test::expect_ub_constrain(2, stan::math::INFTY);
}

TEST(mathMixMatFun, ub_mat_constrain) {
  using stan::scalar_type_t;
  using stan::math::promote_scalar_t;
  using stan::math::ub_constrain;

  Eigen::MatrixXd A(2, 3);
  A << -1.1, 0.0, 1.0, 2.0, 0.0, 0.0005;
  Eigen::MatrixXd ubm(2, 3);
  ubm << 1.0, 2.0, 2.0, 33.7, 0.0, 0.00005;

  double ubd1 = 10.1;
  ub_constrain_test::expect_ub_constrain(A, ubm);
//  ub_constrain_test::expect_ub_constrain(A, ubd1);
}

TEST(mathMixMatFun, ub_mat_constrain_inf) {

  Eigen::MatrixXd A(2, 2);
  A << -1.1, 0.0, 1.0, 2.0;
  Eigen::MatrixXd ubm(2, 2);
  ubm << 1.0, 2.0, stan::math::INFTY, 33.7;

  double ubd1 = stan::math::INFTY;

  ub_constrain_test::expect_ub_constrain(A, ubm);
  ub_constrain_test::expect_ub_constrain(A, ubd1);
}
