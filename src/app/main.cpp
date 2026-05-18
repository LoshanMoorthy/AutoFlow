#include "../scenario/Scenario.h"
#include "../core/SimulationEngine.h"
#include "../util/JsonConfig.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string arg = (argc > 1) ? argv[1] : "Normal";

    Pipeline pipeline;
    EngineConfig engineCfg;

    if (arg.size() > 4 && arg.substr(arg.size() - 5) == ".json") {
        AppConfig cfg = loadConfigOrDefaults(arg);
        engineCfg = cfg.engine;
        for (auto& s : cfg.stations)
            pipeline.addStation(s);
    }
    else {
        ScenarioType type = ScenarioFactory::fromString(arg);
        ScenarioDefinition def = ScenarioFactory::create(type);
        engineCfg = def.engine;
        for (auto& s : def.stations)
            pipeline.addStation(s);
    }

    SimulationEngine engine(engineCfg, std::move(pipeline));
    engine.run();

    return 0;
}