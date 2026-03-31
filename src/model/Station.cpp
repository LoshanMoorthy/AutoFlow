#include "Station.h"
#include <utility>

Station::Station(StationConfig cfg)
    : _cfg(std::move(cfg))
    , _in(_cfg.queueCapacity) {
}

bool Station::enqueue(Item item) {
    return _in.tryPush(std::move(item));
}

bool Station::hasCompletedItemReady(std::int64_t simMs) const {
    return _busy && simMs >= _finishAtMs;
}

Item Station::takeCompletedItem() {
    _busy = false;
    _blocked = false;
    return std::move(_current);
}

std::optional<Item> Station::tick(std::int64_t simMs, double dtSeconds, Random& rng, MetricsRegistry& metrics) {
    metrics.setGauge("queue_depth{" + _cfg.name + "}", static_cast<double>(_in.size()));
    metrics.setGauge("queue_capacity{" + _cfg.name + "}", static_cast<double>(_in.capacity()));
    metrics.setGauge("station_down{" + _cfg.name + "}", isDown(simMs) ? 1.0 : 0.0);
    metrics.setGauge("station_blocked{" + _cfg.name + "}", _blocked ? 1.0 : 0.0);

    const std::int64_t dtMs = static_cast<std::int64_t>(dtSeconds * 1000.0);

    if (!isDown(simMs)) {
        _uptimeMs += dtMs;
        if (_busy)
            _busyMs += dtMs;
    }

    const double util = (_uptimeMs > 0)
        ? (static_cast<double>(_busyMs) / static_cast<double>(_uptimeMs))
        : 0.0;

    metrics.setGauge("station_util{" + _cfg.name + "}", util);

    if (isDown(simMs))
        return std::nullopt;

    if (rng.chancePerSecond(_cfg.failureRatePerSec, dtSeconds)) {
        const int dtDownMs = rng.uniformInt(_cfg.downtimeMsMin, _cfg.downtimeMsMax);
        _downUntilMs = simMs + dtDownMs;
        metrics.incCounter("incident_total{down," + _cfg.name + "}");
        return std::nullopt;
    }

    if (!_busy && !_in.empty()) {
        _current = _in.pop();
        _busy = true;
        _blocked = false;

        const int svc = rng.uniformInt(_cfg.serviceTimeMsMin, _cfg.serviceTimeMsMax);
        _finishAtMs = simMs + svc;

        metrics.incCounter("items_started_total{" + _cfg.name + "}");
    }

    if (_busy && simMs >= _finishAtMs) {
        return _current;
    }

    return std::nullopt;
}