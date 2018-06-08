#ifndef PK_LINODE_SOLVER_HPP
#define PK_LINODE_SOLVER_HPP

namespace refactor {

  using boost::math::tools::promote_args;
  using Eigen::Matrix;
  using Eigen::Dynamic;

  /**
   * Linear ODE solver based on matrix exponential.
   *
   */
  class PKLinODEModelSolver {
  public:
    PKLinODEModelSolver() {}

  /**
   * Solve Linear ODE model, or any model that can provide
   * RHS matrix (thus can be seen as linear ODE model)
   *
   * @tparam T_time  type of time
   * @tparam T_model type of model
   * @tparam Ts_par  type of model parameters.
   * @param pkmodel  linear ODE model
   * @param dt  time span
   */
    template<typename T_time,
             template <class, class... > class T_model,
             class... Ts_par>    
    Eigen::Matrix<typename T_model<Ts_par...>::scalar_type, Eigen::Dynamic, 1> 
    solve(const T_model<Ts_par...> &pkmodel, const T_time& dt) {
      using Eigen::Matrix;
      using Eigen::Dynamic;
      using stan::math::matrix_exp;
      using stan::math::mdivide_left;
      using stan::math::multiply;
      using scalar_type = typename T_model<Ts_par...>::scalar_type;
      using T_rate = typename T_model<Ts_par...>::rate_type;

      auto init = pkmodel.y0()   ;
      auto rate = pkmodel.rate() ;
      auto system = pkmodel.rhs_matrix();

      if (std::any_of(rate.begin(), rate.end(), [](T_rate r){return r != 0;})) {
        int nCmt = system.cols();
        Matrix<scalar_type, Dynamic, 1> rate_vec(rate.size()), x(nCmt), x2(nCmt);
        for (size_t i = 0; i < rate.size(); i++) rate_vec(i) = rate[i];
        x = mdivide_left(system, rate_vec);
        x2 = x + init.transpose();
        Matrix<scalar_type, Dynamic, Dynamic> dt_system = multiply(dt, system);
        Matrix<scalar_type, Dynamic, 1> pred = matrix_exp(dt_system) * x2;
        pred -= x;
        return pred.transpose();
      } else {
        // if (rate_zeros) {
        Matrix<scalar_type, Dynamic, Dynamic> dt_system = multiply(dt, system);
        Matrix<scalar_type, Dynamic, 1> pred = matrix_exp(dt_system)
          * init.transpose();
        return pred.transpose();
      }

      // if (dt == 0) { return init;
      // } else {


      //   bool rate_zeros = true;
      //   for (size_t i = 0; i < rate.size(); i++)
      //     if (rate[i] != 0) rate_zeros = false;

      //   // trick to promote dt, and dt_system
      //   scalar_type dt_s = dt;

      //   if (rate_zeros) {
      //     Matrix<scalar_type, Dynamic, Dynamic> dt_system = multiply(dt_s, system);
      //     Matrix<scalar_type, Dynamic, 1> pred = matrix_exp(dt_system)
      //       * init.transpose();
      //     return pred.transpose();
      //   } else {
      //     int nCmt = system.cols();
      //     Matrix<scalar_type, Dynamic, 1> rate_vec(rate.size()), x(nCmt), x2(nCmt);
      //     for (size_t i = 0; i < rate.size(); i++) rate_vec(i) = rate[i];
      //     x = mdivide_left(system, rate_vec);
      //     x2 = x + init.transpose();
      //     Matrix<scalar_type, Dynamic, Dynamic> dt_system = multiply(dt_s, system);
      //     Matrix<scalar_type, Dynamic, 1> pred = matrix_exp(dt_system) * x2;
      //     pred -= x;
      //     return pred.transpose();
      //   }
      // }
    }
  };
}

#endif
