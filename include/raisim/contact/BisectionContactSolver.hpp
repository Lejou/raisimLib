//
// Created by jhwangbo on 3/12/17.
//

#ifndef RAI_RAICONTACT2_HPP
#define RAI_RAICONTACT2_HPP

#include "raisim/helper.hpp"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <Eigen/Dense>
#include <raisim/Materials.hpp>
#include "raisim/math.hpp"
#include "raisim/object/Object.hpp"

namespace raisim {

class Object;

namespace contact {

class Single3DContactProblem {
 public:
  Single3DContactProblem(const MaterialPairProperties &mat, double x, double y, double z, double depth_in);
  Single3DContactProblem() = default;
  void checkRank();

  raisim::Vec<3> imp_i, tau_i, position_W;
  raisim::Mat<3, 3> MappInv_i;
  raisim::Mat<3, 3> MappInvWODel_i;
  raisim::Mat<3, 3> Mapp_i;
  raisim::Mat<2, 2> Mapp_iInv22, Mapp_i22;
  raisim::Vec<3> cooTrans_i;
  raisim::Mat<3, 2> MappInv_red;
  std::vector<raisim::Mat<3, 3>> MappInv_j;
  std::vector<Vec<3> *> imp_j;
  double mu, n2_mu, muinv, negMuSquared, coeffRes, bounceThres, bouceVel=0., Mapp_iInv11, impact_vel=0., depth=0.;
  Object *obA = nullptr, *obB = nullptr;
  int rank = 3;
  size_t pointIdA, pointIdB;
  bool atLeastOneWithoutDel = false;
};

typedef std::vector<contact::Single3DContactProblem, AlignedAllocator<contact::Single3DContactProblem, 32>>
    ContactProblems;

class BisectionContactSolver {

 public:

  struct SolverConfiguration {
    double alpha_init = 1.0;
    double alpha_low = 1.0;
    double alpha_decay = 1.0;
    double error_to_terminate = 1e-8;
    double erp = 0.3;
    double erp2 = 0.005;
    int maxIteration = 100;
  };

  explicit BisectionContactSolver(double dt) : dt_(dt), dtinv_(1/dt) {
    error_.resize(100);
  }

  void solve(ContactProblems &contact);

  void updateConfig(SolverConfiguration config) { config_=config; }

  void setTimestep(double dt) {
    dt_ = dt;
    dtinv_ = 1/dt;
  }

  void setOrder(bool order) { order_ = order; }

  int getLoopCounter() const { return loopCounter_; }

  const std::vector<double>& getErrorHistory() const { return error_; }

  SolverConfiguration& getConfig() { return config_; }
  const SolverConfiguration& getConfig() const { return config_; }

 private:
  int loopCounter_ = 0;
  bool order_ = true;
  double dt_ = 0.01;
  double dtinv_ = 100.0;
  SolverConfiguration config_;
  std::vector<double> error_;
};

} // contact
} // raisim


#endif //RAICONTACT2
