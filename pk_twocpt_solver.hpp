#ifndef PK_TWOCPT_SOLVER_HPP
#define PK_TWOCPT_SOLVER_HPP

namespace refactor {

  using boost::math::tools::promote_args;

  class PKTwoCptModelSolver {
  public:
    PKTwoCptModelSolver() {}

    template<typename T_time, template <class, class... > class T_model, class... Ts_par>    
    Eigen::Matrix<typename T_model<Ts_par...>::scalar_type, Eigen::Dynamic, 1> 
    solve(const T_model<Ts_par...> &pkmodel, const T_time& dt) {
      using Eigen::Matrix;
      using Eigen::Dynamic;
      using std::vector;
      using scalar_type = typename T_model<Ts_par...>::scalar_type;

      auto init = pkmodel.y0()   ;
      auto rate = pkmodel.rate() ;
      auto ka   = pkmodel.ka()   ;
      auto k10  = pkmodel.k10()  ;
      auto k12  = pkmodel.k12()  ;
      auto k21  = pkmodel.k21()  ;
      auto alpha= pkmodel.alpha();

      std::vector<scalar_type> a(3, 0);
      Matrix<scalar_type, 1, Dynamic> pred = Matrix<scalar_type, 1, Dynamic>::Zero(3);

      if ((init[0] != 0) || (rate[0] != 0))  {
        pred(0, 0) = init[0] * exp(-ka * dt) + rate[0] * (1 - exp(-ka * dt)) / ka;
        a[0] = ka * (k21 - alpha[0]) / ((ka - alpha[0]) * (alpha[1] - alpha[0]));
        a[1] = ka * (k21 - alpha[1]) / ((ka - alpha[1]) * (alpha[0] - alpha[1]));
        a[2] = -(a[0] + a[1]);
        pred(0, 1) += torsten::PolyExp(dt, init[0], 0, 0, 0, false, a, alpha, 3)
          + torsten::PolyExp(dt, 0, rate[0], dt, 0, false, a, alpha, 3);
        a[0] = ka * k12 / ((ka - alpha[0]) * (alpha[1] - alpha[0]));
        a[1] = ka * k12 / ((ka - alpha[1]) * (alpha[0] - alpha[1]));
        a[2] = -(a[0] + a[1]);
        pred(0, 2) += torsten::PolyExp(dt, init[0], 0, 0, 0, false, a, alpha, 3)
          + torsten::PolyExp(dt, 0, rate[0], dt, 0, false, a, alpha, 3);
      }

      if ((init[1] != 0) || (rate[1] != 0)) {
        a[0] = (k21 - alpha[0]) / (alpha[1] - alpha[0]);
        a[1] = (k21 - alpha[1]) / (alpha[0] - alpha[1]);
        pred(0, 1) += torsten::PolyExp(dt, init[1], 0, 0, 0, false, a, alpha, 2)
          + torsten::PolyExp(dt, 0, rate[1], dt, 0, false, a, alpha, 2);
        a[0] = k12 / (alpha[1] - alpha[0]);
        a[1] = -a[0];
        pred(0, 2) += torsten::PolyExp(dt, init[1], 0, 0, 0, false, a, alpha, 2)
          + torsten::PolyExp(dt, 0, rate[1], dt, 0, false, a, alpha, 2);
      }

      if ((init[2] != 0) || (rate[2] != 0)) {
        a[0] = k21 / (alpha[1] - alpha[0]);
        a[1] = -a[0];
        pred(0, 1) += torsten::PolyExp(dt, init[2], 0, 0, 0, false, a, alpha, 2)
          + torsten::PolyExp(dt, 0, rate[2], dt, 0, false, a, alpha, 2);
        a[0] = (k10 + k12 - alpha[0]) / (alpha[1] - alpha[0]);
        a[1] = (k10 + k12 - alpha[1]) / (alpha[0] - alpha[1]);
        pred(0, 2) += torsten::PolyExp(dt, init[2], 0, 0, 0, false, a, alpha, 2)
          + torsten::PolyExp(dt, 0, rate[2], dt, 0, false, a, alpha, 2);
      }

      return pred;
    }
  };

}

#endif
