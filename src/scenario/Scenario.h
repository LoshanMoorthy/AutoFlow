#pragma once
#include <string>
#include <vector>

#include "../core/SimulationEngine.h"
#include "../model/Station.h"

enum class ScenarioType {
    Normal,
    PeakDemand,
    BottleneckAtPick,
    EquipmentFailure,
    RecoveryShift
};

struct ScenarioDefinition {
    std::string name;
    EngineConfig engine;
    std::vector<StationConfig> stations;
};

class ScenarioFactory {
public:
    static ScenarioDefinition create(ScenarioType type);
    static ScenarioType fromString(const std::string& name);
    static std::string toString(ScenarioType type);
};