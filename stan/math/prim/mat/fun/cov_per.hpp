#ifndef STAN_MATH_PRIM_MAT_FUN_COV_PER_HPP
#define STAN_MATH_PRIM_MAT_FUN_COV_PER_HPP

#include <stan/math/prim/scal/meta/return_type.hpp>
#include <stan/math/prim/scal/err/check_size_match.hpp>
#include <stan/math/prim/mat/fun/Eigen.hpp>
#include <stan/math/prim/mat/fun/distance.hpp>
#include <stan/math/prim/scal/err/check_not_nan.hpp>
#include <stan/math/prim/scal/err/check_positive.hpp>
#include <stan/math/prim/scal/fun/square.hpp>
#include <stan/math/prim/mat/fun/exp.hpp>
#include <vector>
#include <cmath>

namespace stan {
  namespace math {

    /**
     * Returns a periodic kernel.
     *
     * @tparam T_x type of std::vector of elements
     * @tparam T_sigma type of sigma
     * @tparam T_l type of length scale
     * @tparam T_p type of period
     *
     * @param x std::vector of elements that can be used in square distance.
     *    This function assumes each element of x is the same size.
     * @param sigma standard deviation
     * @param l length scale
     * @param p period
     * @return squared distance
     * @throw std::domain_error if sigma <= 0, l <= 0, p <= 0 or
     *   x is nan or infinite
     */
    template<typename T_x, typename T_sigma, typename T_l, typename T_p>
    inline typename
    Eigen::Matrix<typename stan::return_type<T_x, T_sigma, T_l, T_p>::type,
                  Eigen::Dynamic, Eigen::Dynamic>
    cov_per(const std::vector<T_x>& x,
                 const T_sigma& sigma,
                 const T_l& l,
				 const T_p& p) {
      //using std::exp;
      check_positive("cov_per", "marginal variance", sigma);
      check_positive("cov_per", "length-scale", l);
      check_positive("cov_per", "period", p);
      for (size_t n = 0; n < x.size(); ++n)
        check_not_nan("cov_per", "x", x[n]);

      Eigen::Matrix<typename stan::return_type<T_x, T_sigma, T_l, T_p>::type,
                    Eigen::Dynamic, Eigen::Dynamic>
        cov(x.size(), x.size());

      int x_size = x.size();
      if (x_size == 0)
        return cov;

      T_sigma sigma_sq = square(sigma);
      T_l neg_two_inv_l_sq = -2.0 / square(l);
      T_p pi_div_p = M_PI / p;

      for (int j = 0; j < (x_size - 1); ++j) {
        cov(j, j) = sigma_sq;
        for (int i = j + 1; i < x_size; ++i) {
          cov(i, j) = sigma_sq * exp(square(sin(pi_div_p * distance(x[i], x[j])))
                                     * neg_two_inv_l_sq);
          cov(j, i) = cov(i, j);
        }
      }
      cov(x_size - 1, x_size - 1) = sigma_sq;
      return cov;
    }

    /**
     * Returns a periodic kernel.
     *
     * @tparam T_x1 type of first std::vector of elements
     * @tparam T_x2 type of second std::vector of elements
     * @tparam T_sigma type of sigma
     * @tparam T_l type of length scale
     * @tparam T_p type of period
     *
     * @param x1 std::vector of elements that can be used in square distance
     * @param x2 std::vector of elements that can be used in square distance
     * @param sigma standard deviation
     * @param l length scale
     * @param p period
     * @return squared distance
     * @throw std::domain_error if sigma <= 0, l <= 0, p <= 0 , or
     *   x is nan or infinite
     */
    template<typename T_x1, typename T_x2, typename T_sigma, typename T_l, typename T_p>
    inline typename
    Eigen::Matrix<typename stan::return_type<T_x1, T_x2, T_sigma, T_l, T_p>::type,
                  Eigen::Dynamic, Eigen::Dynamic>
    cov_per(const std::vector<T_x1>& x1,
                 const std::vector<T_x2>& x2,
                 const T_sigma& sigma,
                 const T_l& l,
				 const T_p& p) {
      using std::exp;
      check_positive("cov_per", "marginal variance", sigma);
      check_positive("cov_per", "length-scale", l);
      check_positive("cov_per", "period", p);
      for (size_t n = 0; n < x1.size(); ++n)
        check_not_nan("cov_per", "x1", x1[n]);
      for (size_t n = 0; n < x2.size(); ++n)
        check_not_nan("cov_per", "x2", x2[n]);

      Eigen::Matrix<typename stan::return_type<T_x1, T_x2, T_sigma, T_l, T_p>::type,
                    Eigen::Dynamic, Eigen::Dynamic>
        cov(x1.size(), x2.size());
      if (x1.size() == 0 || x2.size() == 0)
        return cov;

      T_sigma sigma_sq = square(sigma);
      T_l neg_two_inv_l_sq = -2.0 / square(l);
      T_p pi_div_p = M_PI / p;

      for (size_t i = 0; i < x1.size(); ++i) {
        for (size_t j = 0; j < x2.size(); ++j) {
          cov(i, j) = sigma_sq * exp(square(sin(pi_div_p * distance(x1[i], x2[j])))
                                     * neg_two_inv_l_sq);
        }
      }
      return cov;
    }

  }
}

#endif
