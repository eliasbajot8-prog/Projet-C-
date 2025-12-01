#include "windkessel.h"
#include <cmath>

WindkesselModel::WindkesselModel(const Parameters &params) : p(params)
{
}

double WindkesselModel::Pin(double t) const
{
    // Temps ramené dans le cycle cardiaque
    double tloc = std::fmod(t, p.getT());

    // --- SYSTOLE ---
    if (tloc < p.getTs())
    {
        double x = M_PI * tloc / p.getTs();
        return p.getPmin() + (p.getPmax() - p.getPmin()) * std::sin(x) * std::sin(x);
    }

    // --- DIASTOLE ---
    return p.getPmin() + (p.getPmax() - p.getPmin()) * std::exp(-(tloc - p.getTs()) / p.getTau());
}

double WindkesselModel::compliance(double P) const
{
    return p.getC0() / (1.0 + p.getAlpha() * P);
}

// Dans WindkesselModel::rhs

P_Q_System WindkesselModel::rhs(double t, const P_Q_System &X) const
{
    P_Q_System dX;

    double C = compliance(X.P);

    // Équation de pression (inchangée)
    dX.P = (Qin - X.P / p.getRp()) / C;

    // --- AJOUT DE LA LOGIQUE DE VALVE ---
    double P_ventricule = Pin(t);
    double R_total = p.getRc(); // Résistance de base

    if (P_ventricule < X.P)
    {
        // On multiplie la résistance par un grand facteur (ex: 1000)
        // Cela va "étouffer" le débit négatif très rapidement
        R_total *= 1000.0;
    }

    // Équation de débit avec la résistance conditionnelle
    dX.Q = (P_ventricule - X.P - R_total * X.Q) / p.getLw();

    return dX;
}