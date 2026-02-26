#include "../util/JsonConfig.h"
#include "../model/Pipeline.h"
#include "../core/SimulationEngine.h"
#include <iostream>

int main(int argc, char** argv) {
    std::string configPath = "config/sample.json";
    if (argc >= 2) configPath = argv[1];

    auto cfg = loadConfigOrDefaults(configPath);

    Pipeline p;
    for (const auto& st : cfg.stations)
        p.addStation(st);

    cfg.engine.csvPath = "autoflow_run.csv";

    SimulationEngine engine(cfg.engine, std::move(p));
    engine.run();

    std::cout << "\nCSV written: autoflow_run.csv\n";
    return 0;
}