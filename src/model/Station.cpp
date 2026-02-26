#include "Station.h"

Station::Station(StationConfig cfg) : _cfg(std::move(cfg)) {}

void Station::enqueue(Item item) {
	_in.push(std::move(item));
}

std::optional<Item> Station::tick(std::int64_t simMs, double dtSeconds, Random& rng, MetricsRegistry& metrics) {
    metrics.setGauge("queue_depth{" + _cfg.name + "}", static_cast<double>(_in.size()));
    metrics.setGauge("station_down{" + _cfg.name + "}", isDown(simMs) ? 1.0 : 0.0);

    if (isDown(simMs))
        return std::nullopt;

    // random downtime incidents
    if (rng.chancePerSecond(_cfg.failureRatePerSec, dtSeconds)) {
        const int dtMs = rng.uniformInt(_cfg.downtimeMsMin, _cfg.downtimeMsMax);
        _downUntilMs = simMs + dtMs;
        metrics.incCounter("incident_total{down," + _cfg.name + "}");
        return std::nullopt;
    }

    if (!_busy && !_in.empty()) {
        _current = _in.pop();
        _busy = true;

        const int svc = rng.uniformInt(_cfg.serviceTimeMsMin, _cfg.serviceTimeMsMax);
        _finishAtMs = simMs + svc;

        metrics.incCounter("items_started_total{" + _cfg.name + "}");
    }

    if (_busy && simMs >= _finishAtMs) {
        _busy = false;
        metrics.incCounter("items_processed_total{" + _cfg.name + "}");
        return _current;
    }

    return std::nullopt;
}