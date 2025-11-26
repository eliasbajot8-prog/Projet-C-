#include <iostream>
#include <vector>
#include <fstream>
#include "utils.h"
#include "coupling.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./solver input/parameters.in\n";
        return 1;
    }

    std::string filename = argv[1];

    Parameters params;
    params.load_from_file(filename);

    double dt = params.getDt();
    double t_final = params.getTfinal();

    Coupling system(params);

    std::vector<double> t_save, P_wk_save, Q_wk_save;

    double t = 0.0;

    std::ofstream file_xt("data/P_xt.csv");
    if (!file_xt.is_open())
    {
        std::cerr << "ERREUR : impossible de créer data/P_xt.csv" << std::endl;
        return 1;
    }
    int step_count = 0;
    int output_freq = params.getOutputFrequency();

    while (t < t_final)
    {
        system.step(t);
        bool write_step = (step_count % output_freq == 0);
        if (write_step)
        {
            const auto &X = system.getPde().getX();
            const auto &P = system.getPde().getP();
            for (size_t i = 0; i < X.size(); ++i)
                file_xt << X[i] << " " << t << " " << P[i] << "\n";
            file_xt << "\n"; // séparation entre chaque instant (important pour gnuplot)
        }
        // Sauvegarde des données WK
        t_save.push_back(t);
        P_wk_save.push_back(system.getP_wk());
        Q_wk_save.push_back(system.getQ_out_PDE());

        t += dt;
        step_count++;
    }
    file_xt.close();

    // On test le winkessel seul pour comparaison
    WindkesselModel wk_model(params);
    OdeSolver ode_solver(dt);
    P_Q_System X_wk;
    t = 0.0;
    std::vector<double> t_wk_save, P_wk_alone_save, Q_wk_alone_save;

    X_wk.P = params.getPmin();
    X_wk.Q = 0.0;

    while (t < t_final)
    {
        double P_coeur = wk_model.Pin(t);
        double Q_in = 0.0;
        double t_loc = std::fmod(t, params.getT());

        // --- Simulation d'un débit réaliste (Triangulaire asymétrique) ---
        if (t_loc < params.getTs())
        {
            double Q_max = 5.0e-4;                // 500 mL/s en pointe (plus puissant)
            double T_peak = params.getTs() * 0.3; // Le pic arrive à 30% de la systole (montée rapide)

            if (t_loc < T_peak)
            {
                // Montée rapide
                Q_in = Q_max * (t_loc / T_peak);
            }
            else
            {
                // Descente plus lente
                Q_in = Q_max * (params.getTs() - t_loc) / (params.getTs() - T_peak);
            }
        }
        else
        {
            // Diastole (Débit nul, valves fermées)
            Q_in = 0.0;
        }

        wk_model.set_Q_in(Q_in);
        X_wk = ode_solver.RK4_step(t, X_wk, wk_model);

        t_wk_save.push_back(t);
        P_wk_alone_save.push_back(X_wk.P);
        Q_wk_alone_save.push_back(X_wk.Q);

        t += dt;
    }

    write_two_vectors_csv("data/P_wk_coupled.csv", t_save, P_wk_save);
    write_two_vectors_csv("data/Q_wk_coupled.csv", t_save, Q_wk_save);

    write_two_vectors_csv("data/P_wk_alone.csv", t_wk_save, P_wk_alone_save);
    write_two_vectors_csv("data/Q_wk_alone.csv", t_wk_save, Q_wk_alone_save);

    std::cout << "[OK] Couplage PDE / Windkessel terminé." << std::endl;

    return 0;
}