#pragma once
#include "../model/Pipeline.h"
#include "../metrics/Metrics.h"
#include "../metrics/CsvReporter.h"
#include "../util/Random.h"
#include "../model/OrderTracker.h"
#include "SimClock.h"
#include <cstdint>
#include <string>

struct EngineConfig {
    int tickMs{ 100 };
    int durationSeconds{ 60 };
    int spawnPerSecond{ 10 };
    unsigned int rngSeed{ 1337 };
    int csvEveryMs{ 1000 };
    std::string csvPath{ "autoflow_run.csv" };

    int realTime{ 1 };
    int metricsPort{ 9100 };

    int ordersTarget{ 100 };
    int orderItemsMin{ 1 };
    int orderItemsMax{ 5 };
};

class SimulationEngine {
public:
    SimulationEngine(EngineConfig cfg, Pipeline pipeline);
    void run();

private:
    EngineConfig _cfg;
    Pipeline _pipeline;
    OrderTracker _orderTracker;

    MetricsRegistry _metrics;
    Random _rng;
    CsvReporter _csv;
    SimClock _clock;

    std::uint64_t _nextItemId{ 1 };
    std::int64_t _nextCsvAtMs{ 0 };
    std::uint64_t _nextOrderId{ 1 };
    std::int64_t _ordersCreated{ 0 };

    void spawnItems(double dtSeconds);
};