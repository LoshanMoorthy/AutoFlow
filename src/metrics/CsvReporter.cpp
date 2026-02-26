#include "CsvReporter.h"
#include <vector>
#include <algorithm>

CsvReporter::CsvReporter(const std::string& path) {
    _out.open(path, std::ios::out | std::ios::trunc);
}

void CsvReporter::writeHeader() {
    if (_headerWritten) return;
    _out << "sim_ms,metric_type,metric_name,value\n";
    _headerWritten = true;
}

void CsvReporter::writeSnapshot(std::int64_t simMs, const MetricsRegistry& metrics) {
    writeHeader();

    std::vector<std::string> ckeys;
    ckeys.reserve(metrics.counters().size());
    for (auto& kv : metrics.counters()) ckeys.push_back(kv.first);
    std::sort(ckeys.begin(), ckeys.end());

    for (auto& k : ckeys)
        _out << simMs << ",counter," << k << "," << metrics.counter(k) << "\n";

    std::vector<std::string> gkeys;
    gkeys.reserve(metrics.gauges().size());
    for (auto& kv : metrics.gauges()) gkeys.push_back(kv.first);
    std::sort(gkeys.begin(), gkeys.end());

    for (auto& k : gkeys)
        _out << simMs << ",gauge," << k << "," << metrics.gauge(k) << "\n";
}