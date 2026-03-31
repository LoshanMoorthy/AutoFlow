#include "CsvReporter.h"
#include <vector>
#include <algorithm>

CsvReporter::CsvReporter(std::string path) : _path(std::move(path)) {}

bool CsvReporter::tryOpen(std::ofstream& out) {
    out.open(_path, std::ios::out | std::ios::app);
    return out.good();
}

void CsvReporter::writeHeader(std::ofstream& out) {
    if (_headerWritten) return;
    out << "sim_ms,metric_type,metric_name,value\n";
    _headerWritten = true;
}

bool CsvReporter::writeSnapshot(std::int64_t simMs, const MetricsRegistry& metrics) {
    std::ofstream out;
    if (!tryOpen(out))
        return false; // locked or cannot open

    writeHeader(out);

    std::vector<std::string> ckeys;
    ckeys.reserve(metrics.counters().size());
    for (auto& kv : metrics.counters()) ckeys.push_back(kv.first);
    std::sort(ckeys.begin(), ckeys.end());

    for (auto& k : ckeys)
        out << simMs << ",counter," << k << "," << metrics.counter(k) << "\n";

    std::vector<std::string> gkeys;
    gkeys.reserve(metrics.gauges().size());
    for (auto& kv : metrics.gauges()) gkeys.push_back(kv.first);
    std::sort(gkeys.begin(), gkeys.end());

    for (auto& k : gkeys)
        out << simMs << ",gauge," << k << "," << metrics.gauge(k) << "\n";

    out.flush();
    return true;
}