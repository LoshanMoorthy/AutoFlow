#include "Scenario.h"
#include <stdexcept>

ScenarioDefinition ScenarioFactory::create(ScenarioType type) {
    ScenarioDefinition def{};

    def.engine.tickMs = 100;
    def.engine.durationSeconds = 120;
    def.engine.spawnPerSecond = 10;
    def.engine.rngSeed = 1337;
    def.engine.csvEveryMs = 1000;
    def.engine.csvPath = "autoflow_run.csv";
    def.engine.realTime = 1;
    def.engine.metricsPort = 9100;

    def.engine.ordersTarget = 100;
    def.engine.orderItemsMin = 1;
    def.engine.orderItemsMax = 5;

    switch (type) {
    case ScenarioType::Normal:
    def.name = "Normal";
    def.stations = {
        {"Receive",  200, 500, 0.0005, 2000, 5000, 50},
        {"Decant",   300, 700, 0.0008, 2000, 6000, 30},
        {"Pick",     250, 600, 0.0012, 1500, 4500, 20},
        {"Pack",     300, 800, 0.0007, 2000, 5000, 15},
        {"Dispatch", 200, 500, 0.0004, 2000, 5000, 15},
    };
    break;

    case ScenarioType::PeakDemand:
    def.name = "PeakDemand";
    def.engine.spawnPerSecond = 30;
    def.engine.ordersTarget = 250;
    def.stations = {
        {"Receive",  200, 500, 0.0005, 2000, 5000, 50},
        {"Decant",   300, 700, 0.0008, 2000, 6000, 30},
        {"Pick",     250, 600, 0.0012, 1500, 4500, 20},
        {"Pack",     300, 800, 0.0007, 2000, 5000, 15},
        {"Dispatch", 200, 500, 0.0004, 2000, 5000, 15},
    };
    break;

    case ScenarioType::BottleneckAtPick:
    def.name = "BottleneckAtPick";
    def.engine.spawnPerSecond = 15;
    def.engine.ordersTarget = 150;
    def.stations = {
        {"Receive",  200, 500, 0.0005, 2000, 5000, 50},
        {"Decant",   300, 700, 0.0008, 2000, 6000, 30},
        {"Pick",    1200, 2200, 0.0012, 1500, 4500, 8},
        {"Pack",     300, 800, 0.0007, 2000, 5000, 15},
        {"Dispatch", 200, 500, 0.0004, 2000, 5000, 15},
    };
    break;

    case ScenarioType::EquipmentFailure:
    def.name = "EquipmentFailure";
    def.engine.spawnPerSecond = 12;
    def.engine.ordersTarget = 120;
    def.stations = {
        {"Receive",  200, 500, 0.0005, 2000, 5000, 50},
        {"Decant",   300, 700, 0.0050, 5000, 12000, 30},
        {"Pick",     250, 600, 0.0012, 1500, 4500, 20},
        {"Pack",     300, 800, 0.0007, 2000, 5000, 15},
        {"Dispatch", 200, 500, 0.0004, 2000, 5000, 15},
    };
    break;

    case ScenarioType::RecoveryShift:
    def.name = "RecoveryShift";
    def.engine.spawnPerSecond = 18;
    def.engine.ordersTarget = 180;
    def.stations = {
        {"Receive",  150, 350, 0.0004, 1500, 3500, 60},
        {"Decant",   220, 500, 0.0005, 1500, 4000, 40},
        {"Pick",     200, 450, 0.0008, 1200, 3000, 30},
        {"Pack",     220, 550, 0.0005, 1500, 3500, 20},
        {"Dispatch", 150, 350, 0.0003, 1500, 3500, 20},
    };
    break;

    default:
    throw std::runtime_error("Unknown scenario type");
    }

    return def;
}

ScenarioType ScenarioFactory::fromString(const std::string& name) {
    if (name == "Normal") return ScenarioType::Normal;
    if (name == "PeakDemand") return ScenarioType::PeakDemand;
    if (name == "BottleneckAtPick") return ScenarioType::BottleneckAtPick;
    if (name == "EquipmentFailure") return ScenarioType::EquipmentFailure;
    if (name == "RecoveryShift") return ScenarioType::RecoveryShift;

    return ScenarioType::Normal;
}

std::string ScenarioFactory::toString(ScenarioType type) {
    switch (type) {
    case ScenarioType::Normal: return "Normal";
    case ScenarioType::PeakDemand: return "PeakDemand";
    case ScenarioType::BottleneckAtPick: return "BottleneckAtPick";
    case ScenarioType::EquipmentFailure: return "EquipmentFailure";
    case ScenarioType::RecoveryShift: return "RecoveryShift";
    default: return "Normal";
    }
}