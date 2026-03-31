#pragma once
#include <string>
#include <unordered_map>
#include <cstdint>
#include <mutex>

class MetricsRegistry {
public:
    void incCounter(const std::string& name, std::int64_t delta = 1);
    void setGauge(const std::string& name, double value);

    std::int64_t counter(const std::string& name) const;
    double gauge(const std::string& name) const;

    std::unordered_map<std::string, std::int64_t> counters() const;
    std::unordered_map<std::string, double> gauges() const;

    std::string toPrometheusText() const;

private:
    static std::string sanitizeMetricName(const std::string& raw);
    static std::string escapeLabelValue(const std::string& v);

    static void splitMetricAndLabels(
        const std::string& raw,
        std::string& outBase,
        std::string& outLabelsProm
    );

private:
    mutable std::mutex _mtx;
    std::unordered_map<std::string, std::int64_t> _counters;
    std::unordered_map<std::string, double> _gauges;
};