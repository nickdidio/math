#ifndef STAN_MATH_PRIM_SCAL_FUN_AS_ARRAY_OR_SCALAR_HPP
#define STAN_MATH_PRIM_SCAL_FUN_AS_ARRAY_OR_SCALAR_HPP

#include <stan/math/prim/meta.hpp>

namespace stan {
namespace math {

/** \ingroup type_trait
 * Returns specified input value.
 *
 * @tparam T Type of element.
 * @param v Specified value.
 * @return Same value.
 */
template <typename T, typename = require_stan_scalar_t<T>>
inline const T& as_array_or_scalar(const T& v) {
  return v;
}

}  // namespace math
}  // namespace stan

#endif
