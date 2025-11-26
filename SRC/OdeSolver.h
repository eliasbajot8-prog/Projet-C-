#pragma once

#include "P_Q_System.h"
#include "windkessel.h"

// -----------------------------------------------------------
// Classe générique pour résoudre une EDO vectorielle d’ordre 1
// sous la forme : dX/dt = f(t, X)
// Ici, X est un P_Q_System et f est windkessel.rhs
// -----------------------------------------------------------

class OdeSolver
{
public:
    // Constructeur : fournit un dt (pas de temps)
    OdeSolver(double dt);

    // Un pas de RK4 : retourne X(t+dt)
    P_Q_System RK4_step(double t, const P_Q_System &X, const WindkesselModel &model);

private:
    double dt; // pas de temps
};