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

inline double to_kelvin(double c) { return c + 273.15; }

// convierte altura geométrica (m) a altura geopotencial (m) — ligera corrección para mayor exactitud
inline double geopotential_height(double h_m) {
    const double Re = 6371000.0; // radio terrestre (m)
    return Re * h_m / (Re + h_m);
}

/*
  pressure_ISA:
   - h_m: altura geométrica en metros
   - T0_C: temperatura a nivel del mar en °C (si no la conoces, usa 15.0)
   - P0: presión a nivel del mar en Pa (por defecto 101325)
   - rh: humedad relativa [0..1] opcional para corrección aproximada (0 = seco)
  Devuelve presión en Pascales (Pa).
  Modelo: ISA (0-11 km: L = -6.5 K/km; 11-20 km isoterma a 216.65 K).
*/
double pressure_ISA(double h_m, double T0_C, double P0 = 101325.0, double rh = 0.0) {
    // Constantes físicas
    const double g0 = 9.80665;           // m/s^2
    const double R = 287.05287;          // J/(kg·K) — constante específica del aire seco
    const double L = -0.0065;            // K/m (lapse rate en troposfera, negativo)
    const double tropopause_h = 11000.0; // m
    const double T_iso = 216.65;         // K en la capa isoterma (11-20 km)

    // convertir
    double T0 = to_kelvin(T0_C);
    double h_geo = geopotential_height(h_m);

    double Tv0 = T0;       // temperatura virtual en la base
    double r = 0.0;        // mixing ratio (kg/kg)
    if (rh > 0.0) {
        double es_hPa = 6.112 * std::exp(17.62 * T0_C / (T0_C + 243.12)); // hPa
        double es = es_hPa * 100.0; // Pa
        double e = rh * es;         // Pa (vapor parcial estimado al nivel del mar)
        if (e >= P0) e = 0.999 * P0; // evitar división por cero
        r = 0.622 * e / (P0 - e);    // aproximación
        Tv0 = T0 * (1.0 + 0.61 * r); // temperatura virtual aproximada
    }

    // troposfera (0..11000 m)
    if (h_geo <= tropopause_h) {
        double T_h = T0 + L * h_geo;
        if (T_h <= 0.0) throw std::runtime_error("Temperatura no física en la altura pedida.");
        double Tv_h = (rh > 0.0) ? T_h * (1.0 + 0.61 * r) : T_h;

        double exponent = -g0 / (R * L);
        return P0 * std::pow(Tv_h / Tv0, exponent);
    }

    double T11 = T0 + L * tropopause_h;
    if (T11 <= 0.0) throw std::runtime_error("Temperatura no física en tropopausa.");
    double Tv11 = (rh > 0.0) ? T11 * (1.0 + 0.61 * r) : T11;
    double exponent = -g0 / (R * L);
    double P11 = P0 * std::pow(Tv11 / Tv0, exponent);


    double delta_h = h_geo - tropopause_h;
    return P11 * std::exp(-g0 * delta_h / (R * T_iso));
}

// ==========================================
// PRESIÓN DE VAPOR DE SATURACIÓN (hPa)
// ==========================================
// Fórmula de Magnus-Tetens (precisión alta entre -40°C y +50°C)
double saturation_vapor_pressure(double T) {
    // T en °C, devuelve eₛ en hPa
    return 6.112 * std::exp((17.625 * T) / (T + 243.04));
}

// ==========================================
// PRESIÓN PARCIAL DEL VAPOR DESDE T Y HR
// ==========================================
double vapor_pressure_from_RH(double T, double RH) {
    // T en °C, RH en %
    // Devuelve e en hPa
    return (RH / 100.0) * saturation_vapor_pressure(T);
}

// ==========================================
// PRESIÓN PARCIAL DEL VAPOR DESDE PUNTO DE ROCÍO
// ==========================================
double vapor_pressure_from_dewpoint(double Td) {
    // Td en °C
    // Devuelve e en hPa
    return saturation_vapor_pressure(Td);
}

// ==========================================
// HUMEDAD RELATIVA DESDE T Y PUNTO DE ROCÍO
// ==========================================
double relative_humidity(double T, double Td) {
    // T y Td en °C
    // Devuelve HR en %
    double e  = saturation_vapor_pressure(Td); // presión real
    double es = saturation_vapor_pressure(T);  // presión de saturación
    return (e / es) * 100.0;
}




#endif // CALC_H