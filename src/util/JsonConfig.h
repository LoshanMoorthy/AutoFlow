#pragma once
#include "../core/SimulationEngine.h"
#include "../model/Station.h"
#include <string>
#include <vector>

struct AppConfig {
    EngineConfig engine;
    std::vector<StationConfig> stations;
};

AppConfig loadConfigOrDefaults(const std::string& path);