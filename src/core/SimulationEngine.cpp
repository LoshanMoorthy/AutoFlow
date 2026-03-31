#include "SimulationEngine.h"
#include "../metrics/MetricsServer.h"
#include <iostream>
#include <thread>
#include <chrono>

SimulationEngine::SimulationEngine(EngineConfig cfg, Pipeline pipeline)
    : _cfg(std::move(cfg))
    , _pipeline(std::move(pipeline))
    , _rng(_cfg.rngSeed)
    , _csv(_cfg.csvPath) {
    _nextCsvAtMs = 0;
}

void SimulationEngine::spawnItems(double dtSeconds) {
    if (_ordersCreated >= _cfg.ordersTarget)
        return;

    const double expected = static_cast<double>(_cfg.spawnPerSecond) * dtSeconds;
    int count = static_cast<int>(expected);
    const double frac = expected - count;
    if (_rng.uniform01() < frac) count++;

    for (int i = 0; i < count; ++i) {
        if (_ordersCreated >= _cfg.ordersTarget)
            break;

        const std::uint64_t orderId = _nextOrderId++;
        const int itemCount = _rng.uniformInt(_cfg.orderItemsMin, _cfg.orderItemsMax);

        _orderTracker.registerOrder(orderId, itemCount);
        _ordersCreated++;

        _metrics.incCounter("orders_created_total");

        for (int itemIndex = 0; itemIndex < itemCount; ++itemIndex) {
            Item it;
            it.id = _nextItemId++;
            it.orderId = orderId;
            it.orderItemIndex = itemIndex;
            it.createdMs = _clock.simMs;
            it.enteredPipelineMs = _clock.simMs;

            _pipeline.pushToFirst(std::move(it));
            _metrics.incCounter("items_created_total");
        }
    }
}

void SimulationEngine::run() {
    if (_cfg.tickMs <= 0) {
        std::cerr << "ERROR: tickMs must be > 0. Current: " << _cfg.tickMs << "\n";
        return;
    }
    if (_cfg.durationSeconds <= 0) {
        std::cerr << "ERROR: durationSeconds must be > 0. Current: " << _cfg.durationSeconds << "\n";
        return;
    }

    const std::int64_t endMs = static_cast<std::int64_t>(_cfg.durationSeconds) * 1000;
    const double dtSeconds = static_cast<double>(_cfg.tickMs) / 1000.0;

    std::cout << "tickMs=" << _cfg.tickMs << ", dtSeconds=" << dtSeconds << "\n";

    std::cout << "AutoFlow Phase 1 starting...\n";
    std::cout << "Duration: " << _cfg.durationSeconds << "s, Tick: " << _cfg.tickMs << "ms\n";
    std::cout << "Spawn: " << _cfg.spawnPerSecond << "/sec\n";
    std::cout << "CSV: " << _cfg.csvPath << "\n\n";

    MetricsServer metricsServer(_metrics, _cfg.metricsPort);
    metricsServer.start();

    std::cout << "Metrics: http://localhost:" << _cfg.metricsPort << "/metrics\n";

    while (_clock.simMs <= endMs) {
        spawnItems(dtSeconds);

        _pipeline.tick(_clock.simMs, dtSeconds, _rng, _metrics, _orderTracker);

        _metrics.setGauge("items_in_system", static_cast<double>(_pipeline.itemsInSystem()));

        _metrics.setGauge("orders_target", static_cast<double>(_cfg.ordersTarget));
        _metrics.setGauge("orders_total", static_cast<double>(_orderTracker.totalOrders()));
        _metrics.setGauge("orders_completed", static_cast<double>(_orderTracker.completedOrders()));
        _metrics.setGauge("orders_remaining", static_cast<double>(_orderTracker.remainingOrders()));

        const double pct = (_cfg.ordersTarget > 0)
            ? (100.0 * static_cast<double>(_orderTracker.completedOrders()) / static_cast<double>(_cfg.ordersTarget))
            : 0.0;

        _metrics.setGauge("orders_completion_pct", pct > 100.0 ? 100.0 : pct);

        if (_clock.simMs >= _nextCsvAtMs) {
            if (!_csv.writeSnapshot(_clock.simMs, _metrics)) {
                _metrics.incCounter("csv_write_skipped_total");
            }
            _nextCsvAtMs += _cfg.csvEveryMs;
        }

        if (_cfg.realTime == 1)
            std::this_thread::sleep_for(std::chrono::milliseconds(_cfg.tickMs));

        _clock.simMs += _cfg.tickMs;
    }

    std::cout << "Done.\n";
    std::cout << "Created:   " << _metrics.counter("items_created_total") << "\n";
    std::cout << "Completed: " << _metrics.counter("items_completed_total") << "\n";
    std::cout << "InSystem:  " << static_cast<std::int64_t>(_metrics.gauge("items_in_system")) << "\n";
    std::cout << "Orders total:      " << _orderTracker.totalOrders() << "\n";
    std::cout << "Orders completed:  " << _orderTracker.completedOrders() << "\n";
    std::cout << "Orders remaining:  " << _orderTracker.remainingOrders() << "\n";
}