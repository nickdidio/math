#ifndef STAN_MATH_PRIM_FUN_CORR_CONSTRAIN_HPP
#define STAN_MATH_PRIM_FUN_CORR_CONSTRAIN_HPP

#include <stan/math/prim/meta.hpp>
#include <stan/math/prim/fun/log1m.hpp>
#include <stan/math/prim/fun/square.hpp>
#include <stan/math/prim/fun/tanh.hpp>
#include <cmath>

namespace stan {
namespace math {

/**
 * Return the result of transforming the specified object to have
 * a valid correlation value between -1 and 1 (inclusive).
 *
 * <p>The transform used is the hyperbolic tangent function,
 *
 * <p>\f$f(x) = \tanh x = \frac{\exp(2x) - 1}{\exp(2x) + 1}\f$.
 *
 * @tparam T type of value
 * @param[in] x value
 * @return tanh transform of value
 */
template <typename T>
inline auto corr_constrain(T&& x) {
  using std::tanh;
  return tanh(std::forward<T>(x));
}

/**
 * Return the result of transforming the specified scalar to have
 * a valid correlation value between -1 and 1 (inclusive).
 *
 * <p>The transform used is as specified for
 * <code>corr_constrain(T)</code>.  The log absolute Jacobian
 * determinant is
 *
 * <p>\f$\log | \frac{d}{dx} \tanh x  | = \log (1 - \tanh^2 x)\f$.
 *
 * @tparam T Type of scalar.
 * @tparam T2 Type of log prob
 * @param[in] x value
 * @param[in,out] lp log density accumulator
 */
template <typename T, typename T2, typename = require_all_stan_scalar_t<T, T2>>
inline auto corr_constrain(T x, T2& lp) {
  using std::tanh;
  auto tanh_x = tanh(x);
  lp += sum(log1m(square(tanh_x)));
  return tanh_x;
}

/**
 * Return the result of transforming the specified eigen matrix to have
 * a valid correlation value between -1 and 1 (inclusive).
 *
 * <p>The transform used is as specified for
 * <code>corr_constrain(T)</code>.  The log absolute Jacobian
 * determinant is
 *
 * <p>\f$\log | \frac{d}{dx} \tanh x  | = \log (1 - \tanh^2 x)\f$.
 *
 * @tparam T Type of Eigen.
 * @tparam T2 Type of log density accumulator.
 * @param[in] x value.
 * @param[in,out] lp log density accumulator.
 */
template <typename T, typename T2, typename = require_stan_scalar_t<T2>,
          typename = require_eigen_t<T>>
inline auto corr_constrain(T&& x, T2& lp) {
  using std::tanh;
  typename T::PlainObject tanh_x = tanh(std::forward<T>(x));
  lp += sum(log1m(square(tanh_x)));
  return tanh_x;
}

}  // namespace math
}  // namespace stan
#endif
