#include "JsonConfig.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

AppConfig loadConfigOrDefaults(const std::string& path) {
    AppConfig cfg;

    cfg.stations = {
        {"Receive",  200, 500, 0.0005, 2000, 5000, 50},
        {"Decant",   300, 700, 0.0008, 2000, 6000, 30},
        {"Pick",     250, 600, 0.0012, 1500, 4500, 20},
        {"Pack",     300, 800, 0.0007, 2000, 5000, 15},
        {"Dispatch", 200, 500, 0.0004, 2000, 5000, 15},
    };

    std::ifstream f(path);
    if (!f) {
        std::cout << "Config not found, using defaults.\n";
        return cfg;
    }

    json j;
    try {
        f >> j;
    }
    catch (const json::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << ", using defaults.\n";
        return cfg;
    }

    if (j.contains("tickMs"))          cfg.engine.tickMs = j["tickMs"];
    if (j.contains("durationSeconds")) cfg.engine.durationSeconds = j["durationSeconds"];
    if (j.contains("spawnPerSecond"))  cfg.engine.spawnPerSecond = j["spawnPerSecond"];
    if (j.contains("csvEveryMs"))      cfg.engine.csvEveryMs = j["csvEveryMs"];
    if (j.contains("realTime"))        cfg.engine.realTime = j["realTime"];
    if (j.contains("metricsPort"))     cfg.engine.metricsPort = j["metricsPort"];
    if (j.contains("ordersTarget"))    cfg.engine.ordersTarget = j["ordersTarget"];
    if (j.contains("orderItemsMin"))   cfg.engine.orderItemsMin = j["orderItemsMin"];
    if (j.contains("orderItemsMax"))   cfg.engine.orderItemsMax = j["orderItemsMax"];
    if (j.contains("rngSeed"))         cfg.engine.rngSeed = j["rngSeed"];
    if (j.contains("csvPath"))         cfg.engine.csvPath = j["csvPath"];

    if (j.contains("stations") && j["stations"].is_array()) {
        cfg.stations.clear();
        for (const auto& s : j["stations"]) {
            StationConfig sc;
            if (s.contains("name"))            sc.name = s["name"];
            if (s.contains("serviceTimeMsMin")) sc.serviceTimeMsMin = s["serviceTimeMsMin"];
            if (s.contains("serviceTimeMsMax")) sc.serviceTimeMsMax = s["serviceTimeMsMax"];
            if (s.contains("failureRatePerSec")) sc.failureRatePerSec = s["failureRatePerSec"];
            if (s.contains("downtimeMsMin"))   sc.downtimeMsMin = s["downtimeMsMin"];
            if (s.contains("downtimeMsMax"))   sc.downtimeMsMax = s["downtimeMsMax"];
            if (s.contains("queueCapacity"))   sc.queueCapacity = s["queueCapacity"];
            cfg.stations.push_back(sc);
        }
    }

    if (cfg.engine.metricsPort <= 0)  cfg.engine.metricsPort = 9100;
    if (cfg.engine.ordersTarget <= 0) cfg.engine.ordersTarget = 100;
    if (cfg.engine.orderItemsMin <= 0) cfg.engine.orderItemsMin = 1;
    if (cfg.engine.orderItemsMax < cfg.engine.orderItemsMin)
        cfg.engine.orderItemsMax = cfg.engine.orderItemsMin;

    return cfg;
}