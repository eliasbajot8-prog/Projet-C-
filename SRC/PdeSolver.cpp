#include "PdeSolver.h"
#include <iostream>
#include <cmath>

PdeSolver::PdeSolver(const Parameters &params)
{
    Nx = params.getNx();
    Lx = params.getLx();
    dt = params.getDt();
    rho = params.getRho();
    c = params.getCelerity();

    dx = Lx / (Nx - 1);

    // Vérif CFL
    double CFL = c * dt / dx;
    if (CFL > 1.0)
    {
        std::cerr << "[ATTENTION] CFL = " << CFL
                  << " > 1. Schéma potentiellement instable." << std::endl;
    }

    x.resize(Nx);
    P.resize(Nx);
    U.resize(Nx);
    P_new.resize(Nx);
    U_new.resize(Nx);

    // Construire la grille x
    for (int i = 0; i < Nx; ++i)
    {
        x[i] = i * dx;
    }
}

void PdeSolver::initialize(const Parameters &params)
{
    // Pression de repos et vitesse nulle
    double P0 = params.getPmin();

    for (int i = 0; i < Nx; ++i)
    {
        P[i] = P0;
        U[i] = 0.0;
        P_new[i] = P0;
        U_new[i] = 0.0;
    }
}

void PdeSolver::step(double /*t*/, double P_left, double P_right)
{
    // ----------------------------
    // 1) Conditions aux limites
    // ----------------------------
    // Côté gauche (coeur)
    P[0] = P_left;
    U[0] = U[1]; // extrapolation simple

    // Côté droit (sortie)
    P[Nx - 1] = P_right;
    U[Nx - 1] = U[Nx - 2]; // extrapolation simple

    // ----------------------------
    // 2) Points intérieurs : Lax–Wendroff
    // ----------------------------
    double cc = c * c;
    double dtdt = dt * dt;
    double dxdx = dx * dx;

    double coefP = cc * dtdt / (2.0 * dxdx);
    double coefU = cc * dtdt / (2.0 * dxdx);
    double coef1 = rho * cc * dt / (2.0 * dx);
    double coef2 = dt / (2.0 * rho * dx);

    for (int i = 1; i < Nx - 1; ++i)
    {

        // P^{n+1}
        P_new[i] = P[i] - coef1 * (U[i + 1] - U[i - 1]) + coefP * (P[i + 1] - 2.0 * P[i] + P[i - 1]);

        // U^{n+1}
        U_new[i] = U[i] - coef2 * (P[i + 1] - P[i - 1]) + coefU * (U[i + 1] - 2.0 * U[i] + U[i - 1]);
    }

    // BC copiées aussi dans les nouveaux tableaux
    P_new[0] = P[0];
    U_new[0] = U[0];
    P_new[Nx - 1] = P[Nx - 1];
    U_new[Nx - 1] = U[Nx - 1];

    // ----------------------------
    // 3) Remplacer ancien par nouveau
    // ----------------------------
    P.swap(P_new);
    U.swap(U_new);
}