#pragma once

#include <vector>
#include "utils.h" // pour Parameters

class PdeSolver
{
public:
    // Constructeur : initialise la grille et les champs
    PdeSolver(const Parameters &params);

    // Met la condition initiale (repos)
    void initialize(const Parameters &params);

    // Un pas de temps de Lax–Wendroff
    // P_left  = P(0,t)   (côté coeur)
    // P_right = P(L,t)   (côté Windkessel ou autre BC)
    void step(double t, double P_left, double P_right);

    // Accès en lecture aux champs
    const std::vector<double> &getP() const { return P; }
    const std::vector<double> &getU() const { return U; }
    const std::vector<double> &getX() const { return x; }

    int getNx() const { return Nx; }
    double getDx() const { return dx; }
    double getDt() const { return dt; }

private:
    int Nx;
    double Lx;
    double dx;
    double dt;
    double rho;
    double c;

    std::vector<double> x;            // grille spatiale
    std::vector<double> P, U;         // champs courants
    std::vector<double> P_new, U_new; // pour le pas suivant
};