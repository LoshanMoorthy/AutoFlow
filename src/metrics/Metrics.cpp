#include "Metrics.h"
#include <vector>
#include <algorithm>
#include <sstream>

void MetricsRegistry::incCounter(const std::string& name, std::int64_t delta) {
    std::scoped_lock lock(_mtx);
    _counters[name] += delta;
}

void MetricsRegistry::setGauge(const std::string& name, double value) {
    std::scoped_lock lock(_mtx);
    _gauges[name] = value;
}

std::int64_t MetricsRegistry::counter(const std::string& name) const {
    std::scoped_lock lock(_mtx);
    auto it = _counters.find(name);
    return it == _counters.end() ? 0 : it->second;
}

double MetricsRegistry::gauge(const std::string& name) const {
    std::scoped_lock lock(_mtx);
    auto it = _gauges.find(name);
    return it == _gauges.end() ? 0.0 : it->second;
}

std::string MetricsRegistry::sanitizeMetricName(const std::string& raw) {
    std::string s;
    s.reserve(raw.size());

    for (char c : raw) {
        const bool ok =
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            (c == '_');

        s.push_back(ok ? c : '_');
    }

    if (!s.empty() && (s[0] >= '0' && s[0] <= '9'))
        s.insert(s.begin(), '_');

    return s;
}

std::string MetricsRegistry::escapeLabelValue(const std::string& v) {
    std::string out;
    out.reserve(v.size());
    for (char c : v) {
        if (c == '\\' || c == '"') out.push_back('\\');
        out.push_back(c);
    }
    return out;
}

static bool extractBrace(const std::string& raw, std::string& base, std::string& inside) {
    const auto lb = raw.find('{');
    if (lb == std::string::npos) { base = raw; inside.clear(); return false; }
    const auto rb = raw.find('}', lb);
    if (rb == std::string::npos) { base = raw; inside.clear(); return false; }

    base = raw.substr(0, lb);
    inside = raw.substr(lb + 1, rb - (lb + 1));
    return true;
}

static std::vector<std::string> splitComma(const std::string& s) {
    std::vector<std::string> parts;
    std::string cur;
    for (char c : s) {
        if (c == ',') {
            parts.push_back(cur);
            cur.clear();
        }
        else cur.push_back(c);
    }
    parts.push_back(cur);
    return parts;
}

void MetricsRegistry::splitMetricAndLabels(const std::string& raw, std::string& outBase, std::string& outLabelsProm) {
    std::string base, inside;
    if (!extractBrace(raw, base, inside)) {
        outBase = sanitizeMetricName(raw);
        outLabelsProm.clear();
        return;
    }

    outBase = sanitizeMetricName(base);
    const auto parts = splitComma(inside);

    // - foo{Station} => station="Station"
    // - incident_total{type,Station} => type="type",station="Station"
    if (base == "incident_total" && parts.size() >= 2) {
        outLabelsProm =
            "type=\"" + escapeLabelValue(parts[0]) + "\","
            "station=\"" + escapeLabelValue(parts[1]) + "\"";
    }
    else if (parts.size() >= 1) {
        outLabelsProm = "station=\"" + escapeLabelValue(parts[0]) + "\"";
    }
    else {
        outLabelsProm.clear();
    }
}

std::string MetricsRegistry::toPrometheusText() const {
    std::scoped_lock lock(_mtx);

    std::ostringstream out;
    out << "# AutoFlow metrics\n";

    // deterministic ordering for stable diffs
    std::vector<std::string> ckeys;
    ckeys.reserve(_counters.size());
    for (auto& kv : _counters) ckeys.push_back(kv.first);
    std::sort(ckeys.begin(), ckeys.end());

    std::vector<std::string> gkeys;
    gkeys.reserve(_gauges.size());
    for (auto& kv : _gauges) gkeys.push_back(kv.first);
    std::sort(gkeys.begin(), gkeys.end());

    // Counters
    for (const auto& raw : ckeys) {
        std::string base, labels;
        splitMetricAndLabels(raw, base, labels);

        out << "# TYPE " << base << " counter\n";
        out << base;
        if (!labels.empty()) out << "{" << labels << "}";
        out << " " << _counters.at(raw) << "\n";
    }

    // Gauges
    for (const auto& raw : gkeys) {
        std::string base, labels;
        splitMetricAndLabels(raw, base, labels);

        out << "# TYPE " << base << " gauge\n";
        out << base;
        if (!labels.empty()) out << "{" << labels << "}";
        out << " " << _gauges.at(raw) << "\n";
    }

    return out.str();
}

std::unordered_map<std::string, std::int64_t> MetricsRegistry::counters() const {
    std::scoped_lock lock(_mtx);
    return _counters;
}

std::unordered_map<std::string, double> MetricsRegistry::gauges() const {
    std::scoped_lock lock(_mtx);
    return _gauges;
}