#include "../include/axios.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

double getWindSpeed(double latitude, double longitude) {
    std::string url = "https://api.met.no/weatherapi/locationforecast/2.0/compact?lat=" 
                    + std::to_string(latitude)
                    + "&lon=" + std::to_string(longitude);

    // Añadimos User-Agent obligatorio
    auto response = cpr::Get(
        cpr::Url{url},
        cpr::Header{{"User-Agent", "climapp/1.0 "}}
    );

    if (response.status_code != 200) {
        std::cerr << "Error: HTTP " << response.status_code << std::endl;
        return 0;
    }

    try {
        auto data = json::parse(response.text);

        auto timeseries = data["properties"]["timeseries"];
        if (!timeseries.empty()) {
            auto details = timeseries[0]["data"]["instant"]["details"];
            if (details.contains("wind_speed")) {
                return details["wind_speed"].get<double>();
            } else {
                std::cerr << "Advertencia: 'wind_speed' no disponible en este momento." << std::endl;
            }
        } else {
            std::cerr << "Advertencia: 'timeseries' vacío." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error al parsear JSON: " << e.what() << std::endl;
    }

    return 0;
}
