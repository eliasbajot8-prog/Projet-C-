#include "utils.h"
#include <fstream>
#include <sstream>
#include <iostream>

void Parameters::load_from_file(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "[ERREUR] Impossible d'ouvrir " << filename << std::endl;
    }

    std::string line;
    while (std::getline(file, line))
    {

        if (line.empty() || line[0] == '#')
            continue;

        std::stringstream ss(line);
        std::string key;
        char eq;
        double value;

        ss >> key >> eq >> value;
        if (ss.fail())
            continue;

        if (key == "rho")
            rho = value;
        else if (key == "c")
            c = value;
        else if (key == "A0")
            A0 = value;
        else if (key == "Lx")
            Lx = value;

        else if (key == "Rc")
            Rc = value;
        else if (key == "Rp")
            Rp = value;
        else if (key == "C0")
            C0 = value;
        else if (key == "alpha")
            alpha = value;
        else if (key == "Lw")
            Lw = value;

        else if (key == "T")
            T = value;
        else if (key == "Ts")
            Ts = value;
        else if (key == "tau")
            tau = value;
        else if (key == "Pmin")
            Pmin = value;
        else if (key == "Pmax")
            Pmax = value;

        else if (key == "Nx")
            Nx = (int)value;
        else if (key == "dt")
            dt = value;
        else if (key == "t_final")
            t_final = value;
        else if (key == "output_frequency")
            output_frequency = (int)value;
    }
}

void write_vector_csv(const std::string &filename, const std::vector<double> &vec)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "[ERREUR] Impossible d'ouvrir " << filename << " pour écriture." << std::endl;
        return;
    }

    for (const auto &value : vec)
    {
        file << value << '\n';
    }
}

void write_two_vectors_csv(const std::string &filename, const std::vector<double> &x, const std::vector<double> &y)
{
    if (x.size() != y.size())
    {
        std::cerr << "[ERREUR] Les vecteurs ont des tailles différentes : " << x.size() << " et " << y.size() << std::endl;
        return;
    }

    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "[ERREUR] Impossible d'ouvrir " << filename << " pour écriture." << std::endl;
        return;
    }

    for (size_t i = 0; i < x.size(); ++i)
    {
        file << x[i] << "," << y[i] << '\n';
    }
}

#include <cmath>

// Calcule l'erreur L2 relative (en %)
double compute_error_l2(const std::vector<double> &P_num,
                        const std::vector<double> &x,
                        double t,
                        double c,
                        double Lx,
                        double P_base) // Pression de base (Pmin)
{
    double error_sum = 0.0;
    double ref_sum = 0.0;

    // Paramètres de la Gaussienne initiale (doivent être les mêmes que dans initialize)
    double center_init = Lx / 2.0;
    double width = 0.02;
    double Amp = 1000.0; // Amplitude de la perturbation

    for (size_t i = 0; i < P_num.size(); ++i)
    {
        // 1. Calculer la position théorique du centre de la bosse à l'instant t
        // (Le fmod gère la périodicité : si l'onde sort à droite, elle revient à gauche)
        double center_t = std::fmod(center_init + c * t, Lx);

        // 2. Calculer la solution EXACTE à cet endroit
        // Astuce : pour la périodicité parfaite, il faut gérer la distance minimale sur le cercle
        double dist = std::abs(x[i] - center_t);
        if (dist > Lx / 2.0)
            dist = Lx - dist; // Gestion bord périodique

        double P_exact = P_base + Amp * std::exp(-(dist * dist) / (2.0 * width * width));

        // 3. Somme des carrés
        error_sum += std::pow(P_num[i] - P_exact, 2);
        ref_sum += std::pow(P_exact, 2);
    }

    return std::sqrt(error_sum / ref_sum); // Erreur relative
}
