#pragma once

#include <string>
#include <vector>

class Parameters
{
public:
    // Constructeur par défaut
    Parameters() = default;

    // Chargement depuis un fichier .in
    void load_from_file(const std::string &filename);

    // === GETTERS (accès en lecture uniquement) ===
    double getRho() const { return rho; }
    double getCelerity() const { return c; }
    double getA0() const { return A0; }
    double getLx() const { return Lx; }

    double getRc() const { return Rc; }
    double getRp() const { return Rp; }
    double getC0() const { return C0; }
    double getAlpha() const { return alpha; }
    double getLw() const { return Lw; }

    double getT() const { return T; }
    double getTs() const { return Ts; }
    double getTau() const { return tau; }
    double getPmin() const { return Pmin; }
    double getPmax() const { return Pmax; }

    int getNx() const { return Nx; }
    double getDt() const { return dt; }
    double getTfinal() const { return t_final; }
    int getOutputFrequency() const { return output_frequency; }

private: // (Valeurs par défaut)
    // Physique
    double rho = 1060.0;
    double c = 5.0;
    double A0 = 3.14e-4;
    double Lx = 0.3;

    // Windkessel (4 éléments)
    double Rc = 0.2;
    double Rp = 1.0;
    double C0 = 2.0;
    double alpha = 0.05;
    double Lw = 0.005;

    // Cardiaque
    double T = 1.0;
    double Ts = 0.35;
    double tau = 0.15;
    double Pmin = 10.0;
    double Pmax = 120.0;

    // Numérique
    int Nx = 500;
    double dt = 1e-4;
    double t_final = 1.5;
    int output_frequency = 10;
};

// =====================================================
// AUTRES UTILITAIRES (CSV, etc.)
// =====================================================

void write_vector_csv(const std::string &filename, const std::vector<double> &vec);

void write_two_vectors_csv(const std::string &filename, const std::vector<double> &x, const std::vector<double> &y);

double compute_error_l2(const std::vector<double> &P_num,
                        const std::vector<double> &x,
                        double t,
                        double c,
                        double Lx,
                        double P_base);