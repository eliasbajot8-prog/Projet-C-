#pragma once

#include "PdeSolver.h"
#include "windkessel.h"
#include "OdeSolver.h"
#include "P_Q_System.h"
#include "utils.h"

class Coupling
{
public:
    Coupling(const Parameters &params);

    // fait UN PAS de temps couplé
    void step(double t);

    // accès aux objets internes
    PdeSolver &getPde() { return pde; }
    WindkesselModel &getWindkessel() { return wk; }

    // accéder à la pression et débit WK
    double getP_wk() const { return X_wk.P; }
    double getQ_wk() const { return X_wk.Q; }

private:
    Parameters params;

    PdeSolver pde;
    WindkesselModel wk;
    OdeSolver ode;

    P_Q_System X_wk;

    double A0;
    double dt;
};