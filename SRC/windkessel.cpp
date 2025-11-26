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

P_Q_System WindkesselModel::rhs(double t, const P_Q_System &X) const
{
    P_Q_System dX;

    double C = compliance(X.P);

    // dP/dt = (Q - P/Rp) / C
    dX.P = (Qin - X.P / p.getRp()) / C;

    // dQ/dt = (Pin - P - Rc Q) / Lw
    dX.Q = (Pin(t) - X.P - p.getRc() * Qin) / p.getLw();

    return dX;
}