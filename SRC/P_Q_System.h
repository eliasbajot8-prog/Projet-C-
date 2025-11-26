#ifndef PQ_SYSTEM_H
#define PQ_SYSTEM_H

// Classe mère ODE

class P_Q_System
{
public:
    double P;
    double Q;

    // Constructeur par défaut
    P_Q_System();

    // Constructeur principal
    P_Q_System(double P, double Q);

    // Opérateur d'addition
    P_Q_System operator+(const P_Q_System &other) const;
    P_Q_System &operator+=(const P_Q_System &other);

    // Opérateur de soustraction
    P_Q_System operator-(const P_Q_System &other) const;

    // Multiplication par un scalaire X * s
    P_Q_System operator*(double s) const;

    // Multiplication par un scalaire à gauche s * X
    friend P_Q_System operator*(double s, const P_Q_System &st);
};

#endif