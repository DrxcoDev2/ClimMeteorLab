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

#endif // CALC_H