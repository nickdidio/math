#ifdef STAN_OPENCL
#include <stan/math/opencl/rev/opencl.hpp>
#include <stan/math.hpp>
#include <gtest/gtest.h>
#include <test/unit/math/opencl/util.hpp>

auto matrix_multiply_functor
    = [](const auto& a, const auto& b) { return stan::math::multiply(a, b); };

TEST(OpenCLMatrixMultiply, prim_rev_values_small) {
  int N = 2;
  int M = 3;
  int K = 4;

  Eigen::MatrixXd a(N, M);
  a << 1, 2, 3, 4, 5, 6;
  Eigen::MatrixXd b(M, K);
  b << 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1;
  stan::math::test::compare_cpu_opencl_prim_rev(matrix_multiply_functor, a, b);
  stan::math::recover_memory();
}

TEST(OpenCLMatrixMultiply, prim_rev_values_N_0) {
  stan::math::recover_memory();
  int N = 0;
  int M = 3;
  int K = 4;

  Eigen::MatrixXd a(N, M);
  Eigen::MatrixXd b(M, K);
  b << 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1;
  stan::math::test::compare_cpu_opencl_prim_rev(matrix_multiply_functor, a, b);
  stan::math::recover_memory();
}

TEST(OpenCLMatrixMultiply, prim_rev_values_M_0) {
  int N = 2;
  int M = 0;
  int K = 4;

  Eigen::MatrixXd a(N, M);
  Eigen::MatrixXd b(M, K);
  stan::math::test::compare_cpu_opencl_prim_rev(matrix_multiply_functor, a, b);
  stan::math::recover_memory();
}

TEST(OpenCLMatrixMultiply, prim_rev_values_K_0) {
  int N = 2;
  int M = 3;
  int K = 0;

  Eigen::MatrixXd a(N, M);
  a << 1, 2, 3, 4, 5, 6;
  Eigen::MatrixXd b(M, K);
  stan::math::test::compare_cpu_opencl_prim_rev(matrix_multiply_functor, a, b);
  stan::math::recover_memory();
}

TEST(OpenCLMatrixMultiply, prim_rev_values_large) {
  int N = 71;
  int M = 83;
  int K = 97;

  Eigen::MatrixXd a = Eigen::MatrixXd::Random(N, M);
  Eigen::MatrixXd b = Eigen::MatrixXd::Random(M, K);
  stan::math::test::compare_cpu_opencl_prim_rev(matrix_multiply_functor, a, b);
  stan::math::recover_memory();
}

#endif
