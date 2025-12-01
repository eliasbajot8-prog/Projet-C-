#include "coupling.h"

Coupling::Coupling(const Parameters &p)
    : params(p),
      pde(p),
      wk(p),
      ode(p.getDt()),
      X_wk(p.getPmin(), 1e-4) // état initial du Windkessel
{
    A0 = p.getA0();
    dt = p.getDt();

    pde.initialize(p);
}

void Coupling::step(double t)
{
    // -----------------------------
    // 1. Vitesse à la sortie du PDE
    // -----------------------------
    const auto &U = pde.getU();
    int N = U.size();

    double U_out = U[N - 1];
    double Q_out = A0 * U_out; // débit entrant dans le windkessel

    // -----------------------------
    // 2. On force l'entrée du WK
    // -----------------------------
    wk.set_Q_in(Q_out);

    // -----------------------------
    // 3. On fait évoluer le Windkessel
    // -----------------------------
    X_wk = ode.RK4_step(t, X_wk, wk);

    double P_wk = X_wk.P;
    double Q_wk = X_wk.Q;
    // -----------------------------
    // 4. Conditions limites pour le PDE
    // -----------------------------
    double P_left = wk.Pin(t);                      // pression d'entrée (cœur)
    double P_right = P_wk + params.getRc() * Q_out; // pression calculée par le WK

    // -----------------------------
    // 5. Avancer la PDE
    // -----------------------------
    pde.step(t, P_left, P_right);
}