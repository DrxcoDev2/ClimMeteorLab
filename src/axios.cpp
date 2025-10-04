#include "../include/axios.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

double getWindSpeed(double latitude, double longitude) {
    std::string url = "https://api.met.no/weatherapi/locationforecast/2.0/compact?lat=" 
                    + std::to_string(latitude)
                    + "&lon=" + std::to_string(longitude);

    auto response = cpr::Get(cpr::Url{url});

    if (response.status_code != 200) {
        std::cerr << "Error: " << response.status_code << std::endl;
        return -1;
    }

    try {
        auto data = json::parse(response.text);
        return data["properties"]["timeseries"][0]["data"]["instant"]["details"]["wind_speed"];
    } catch (...) {
        std::cerr << "Error al parsear JSON" << std::endl;
        return -1;
    }
}
