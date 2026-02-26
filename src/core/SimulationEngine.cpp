#include "SimulationEngine.h"
#include <iostream>

SimulationEngine::SimulationEngine(EngineConfig cfg, Pipeline pipeline)
    : _cfg(std::move(cfg))
    , _pipeline(std::move(pipeline))
    , _rng(_cfg.rngSeed)
    , _csv(_cfg.csvPath) {
    _nextCsvAtMs = 0;
}

void SimulationEngine::spawnItems(double dtSeconds) {
    // spawn rate per second -> approximate spawnPerSecond * dt
    const double expected = static_cast<double>(_cfg.spawnPerSecond) * dtSeconds;

    int count = static_cast<int>(expected);
    const double frac = expected - count;
    if (_rng.uniform01() < frac) count++;

    for (int i = 0; i < count; ++i) {
        Item it;
        it.id = _nextItemId++;
        it.createdMs = _clock.simMs;
        it.enteredPipelineMs = _clock.simMs;

        _pipeline.pushToFirst(std::move(it));
        _metrics.incCounter("items_created_total");
    }
}

void SimulationEngine::run() {
    const std::int64_t endMs = static_cast<std::int64_t>(_cfg.durationSeconds) * 1000;
    const double dtSeconds = static_cast<double>(_cfg.tickMs) / 1000.0;

    std::cout << "AutoFlow Phase 1 starting...\n";
    std::cout << "Duration: " << _cfg.durationSeconds << "s, Tick: " << _cfg.tickMs << "ms\n";
    std::cout << "Spawn: " << _cfg.spawnPerSecond << "/sec\n";
    std::cout << "CSV: " << _cfg.csvPath << "\n\n";

    while (_clock.simMs <= endMs) {
        spawnItems(dtSeconds);

        _pipeline.tick(_clock.simMs, dtSeconds, _rng, _metrics);

        if (_clock.simMs >= _nextCsvAtMs) {
            _csv.writeSnapshot(_clock.simMs, _metrics);
            _nextCsvAtMs += _cfg.csvEveryMs;
        }

        _clock.simMs += _cfg.tickMs;
    }

    std::cout << "Done.\n";
    std::cout << "Created:   " << _metrics.counter("items_created_total") << "\n";
    std::cout << "Completed: " << _metrics.counter("items_completed_total") << "\n";
}