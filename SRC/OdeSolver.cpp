#include "OdeSolver.h"

// -----------------------------------------------------------
//            CONSTRUCTEUR
// -----------------------------------------------------------
OdeSolver::OdeSolver(double dt_)
    : dt(dt_)
{
}

// -----------------------------------------------------------
//       UN PAS DE RUNGE-KUTTA 4 (RK4)
// -----------------------------------------------------------
P_Q_System OdeSolver::RK4_step(double t,
                               const P_Q_System &X,
                               const WindkesselModel &model)
{
    P_Q_System k1 = model.rhs(t, X);
    P_Q_System k2 = model.rhs(t + dt * 0.5, X + k1 * (dt * 0.5));
    P_Q_System k3 = model.rhs(t + dt * 0.5, X + k2 * (dt * 0.5));
    P_Q_System k4 = model.rhs(t + dt, X + k3 * dt);

    // Combinaison finale
    P_Q_System Xnext =
        X + (k1 + k2 * 2.0 + k3 * 2.0 + k4) * (dt / 6.0);

    return Xnext;
}