#ifndef STAN_MATH_OPENCL_PRIM_CUMULATIVE_SUM_HPP
#define STAN_MATH_OPENCL_PRIM_CUMULATIVE_SUM_HPP
#ifdef STAN_OPENCL
#include <stan/math/opencl/prim/size.hpp>
#include <stan/math/opencl/kernels/cumulative_sum.hpp>
#include <stan/math/prim/meta.hpp>
#include <stan/math/prim/err.hpp>

namespace stan {
namespace math {

/**
 * Return the cumulative sum of the specified vector.
 *
 * The cumulative sum of a vector of values \code{x} is the
 *
 * \code x[0], x[1] + x[2], ..., x[1] + , ..., + x[x.size()-1] @endcode
 *
 * @tparam T type of elements in the vector
 * @param x Vector of values.
 * @return Cumulative sum of values.
 */
template <typename T_vec,
          require_all_kernel_expressions_and_none_scalar_t<T_vec>* = nullptr>
inline auto cumulative_sum(T_vec&& v) {
  check_vector("cumulative_sum(OpenCL)", "v", v);
  const int local_size
      = opencl_kernels::cumulative_sum1_double_sum.get_option("LOCAL_SIZE_");
  int work_groups = std::min(
      (v.size() + local_size - 1) / local_size,
      static_cast<int>(
          opencl_context.device()[0].getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>())
          * 16);
  const int local_size2
      = opencl_kernels::cumulative_sum2_double_sum.get_option("LOCAL_SIZE_");
  matrix_cl<double> tmp_threads(local_size * work_groups, 1);
  matrix_cl<double> tmp_wgs(work_groups, 1);
  matrix_cl<double> res(v.rows(), v.cols());
  if (!is_matrix_cl<T_vec>::value) {
    res = v;
  }
  const matrix_cl<double>& in
      = static_select<is_matrix_cl<T_vec>::value>(v, res);

  try {
    opencl_kernels::cumulative_sum1_double_sum(
        cl::NDRange(local_size * work_groups), cl::NDRange(local_size), tmp_wgs,
        tmp_threads, in, v.size());
    opencl_kernels::cumulative_sum2_double_sum(cl::NDRange(local_size2),
                                               cl::NDRange(local_size2),
                                               tmp_wgs, work_groups);
    opencl_kernels::cumulative_sum3_double_sum(
        cl::NDRange(local_size * work_groups), cl::NDRange(local_size), res, in,
        tmp_threads, tmp_wgs, v.size());
  } catch (const cl::Error& e) {
    check_opencl_error("cumulative_sum", e);
  }
  return res;
}

}  // namespace math
}  // namespace stan

#endif
#endif
