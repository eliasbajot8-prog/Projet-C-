#include "P_Q_System.h"

// Constructeur par défault
P_Q_System::P_Q_System() : P(0.0), Q(0.0) {}

// Constructeur principal
P_Q_System::P_Q_System(double P, double Q) : P(P), Q(Q) {}

// Opérateur d'addition
P_Q_System P_Q_System::operator+(const P_Q_System &other) const
{
    return P_Q_System(this->P + other.P, this->Q + other.Q);
}

P_Q_System &P_Q_System::operator+=(const P_Q_System &other)
{
    this->P += other.P;
    this->Q += other.Q;
    return *this;
}

// Opérateur de soustraction
P_Q_System P_Q_System::operator-(const P_Q_System &other) const
{
    return P_Q_System(this->P - other.P, this->Q - other.Q);
}

// Multiplication par un scalaire
P_Q_System P_Q_System::operator*(double s) const
{
    return P_Q_System(this->P * s, this->Q * s);
}

// Multiplication par un scalaire à gauche s * X
P_Q_System operator*(double s, const P_Q_System &st)
{
    return P_Q_System(s * st.P, s * st.Q);
}
