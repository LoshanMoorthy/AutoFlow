#include "Metrics.h"

void MetricsRegistry::incCounter(const std::string& name, std::int64_t delta) {
	_counters[name] += delta;
}

void MetricsRegistry::setGauge(const std::string& name, double value) {
	_gauges[name] = value;
}

std::int64_t MetricsRegistry::counter(const std::string& name) const {
	auto it = _counters.find(name);
	return it == _counters.end() ? 0 : it->second;
}

double MetricsRegistry::gauge(const std::string& name) const {
	auto it = _gauges.find(name);
	return it == _gauges.end() ? 0.0 : it->second;
}