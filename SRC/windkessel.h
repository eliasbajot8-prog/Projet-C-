#pragma once

#include "utils.h"      // pour class Parameters
#include "P_Q_System.h" // pour P_Q_System

class WindkesselModel
{
public:
    // Constructeur : copie les paramètres
    WindkesselModel(const Parameters &params);

    // Pression ventriculaire d'entrée Pin(t)
    double Pin(double t) const;

    // Compliance non linéaire C(P)
    double compliance(double P) const;

    // RHS du système d'EDO : retourne (dP/dt, dQ/dt)
    P_Q_System rhs(double t, const P_Q_System &X) const;

    // Débit d'entrée dans le Windkessel
    double Qin;
    void set_Q_in(double q) { Qin = q; }

private:
    // L'objet paramètre est stocké (copie)
    Parameters p;
};