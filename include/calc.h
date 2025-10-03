#ifndef CALC_H
#define CALC_H

// Fenómeno periódico simple (ej. temperatura anual)
double fenomeno(double anual, double variacion, int dia) {
    return variacion * std::sin((2 * M_PI / 365.0) * dia) + anual;
}

// Fenómeno con periodo y desfase (ej. estaciones)
double temperature(double anual, double variacion, int periodo, int desfase, int dia) {
    return variacion * std::sin((2 * M_PI / periodo) * dia + desfase) + anual;
}

double to_kelvin(double t) {
    return t + 273.15;
}

double pression(int h, int t) {
    int pha_mar = 101325; //Pa (Pascales)
    int M = 0.029; //kg/mole
    int g = 9.81; //m/s^2
    int R = 8.314; //J/(mol*K)
    int tk = to_kelvin(t);

    return pha_mar * expm1(exp(-M*g*h/(R*tk)));

}

#endif // CALC_H