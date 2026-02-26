#pragma once
#include <string>
#include <unordered_map>
#include <cstdint>

class MetricsRegistry {
public:
	void incCounter(const std::string& name, std::int64_t delta = 1);
	void setGauge(const std::string& name, double value);

	std::int64_t counter(const std::string& name) const;
	double gauge(const std::string& name) const;

	const std::unordered_map<std::string, std::int64_t>& counters() const { return _counters; }
	const std::unordered_map<std::string, double>& gauges() const { return _gauges; }

private:
	std::unordered_map<std::string, std::int64_t> _counters;
	std::unordered_map<std::string, double> _gauges;
};