#ifndef STAN_MATH_PRIM_FUN_CORR_MATRIX_FREE_HPP
#define STAN_MATH_PRIM_FUN_CORR_MATRIX_FREE_HPP

#include <stan/math/prim/meta.hpp>
#include <stan/math/prim/err.hpp>
#include <stan/math/prim/fun/Eigen.hpp>
#include <stan/math/prim/fun/factor_cov_matrix.hpp>
#include <cmath>

namespace stan {
namespace math {

/**
 * Return the vector of unconstrained partial correlations that
 * define the specified correlation matrix when transformed.
 *
 * <p>The constraining transform is defined as for
 * <code>corr_matrix_constrain(Matrix, size_t)</code>.  The
 * inverse transform in this function is simpler in that it only
 * needs to compute the \f$k \choose 2\f$ partial correlations
 * and then free those.
 *
 * @tparam T type of the matrix (must be derived from \c Eigen::MatrixBase)
 * @param y The correlation matrix to free.
 * @return Vector of unconstrained values that produce the
 * specified correlation matrix when transformed.
 * @throw std::domain_error if the correlation matrix has no
 *    elements or is not a square matrix.
 * @throw std::runtime_error if the correlation matrix cannot be
 *    factorized by factor_cov_matrix() or if the sds returned by
 *    factor_cov_matrix() on log scale are unconstrained.
 */
template <typename T, require_eigen_t<T>* = nullptr>
Eigen::Matrix<value_type_t<T>, Eigen::Dynamic, 1> corr_matrix_free(const T& y) {
  using Eigen::Array;
  using Eigen::Dynamic;

  check_square("corr_matrix_free", "y", y);
  check_nonzero_size("corr_matrix_free", "y", y);

  Eigen::Index k = y.rows();
#ifdef USE_STANC3
  Eigen::Index k_choose_2 = (k * (k - 1)) / 2;
#else
  int k_choose_2 = (k * (k - 1)) / 2;
//  Array<value_type_t<T>, Dynamic, 1> x(k_choose_2);
#endif
  Eigen::Matrix<value_type_t<T>, Dynamic, 1> x(k_choose_2);
  Array<value_type_t<T>, Dynamic, 1> sds(k);
#ifdef USE_STANC3
  bool successful = factor_cov_matrix(y, x.array(), sds);
#else
  bool successful = factor_cov_matrix(y, x, sds);
#endif
  if (!successful) {
    throw_domain_error("corr_matrix_free", "factor_cov_matrix failed on y", y,
                       "");
  }
  check_bounded("corr_matrix_free", "log(sd)", sds, -CONSTRAINT_TOLERANCE,
                CONSTRAINT_TOLERANCE);
#ifdef USE_STANC3
  return x;
#else
  return x.matrix();
#endif
}

}  // namespace math
}  // namespace stan

#endif
