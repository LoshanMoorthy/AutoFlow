#pragma once
#include <string>
#include <fstream>
#include <cstdint>
#include "Metrics.h"

class CsvReporter {
public:
    explicit CsvReporter(const std::string& path);
    void writeHeader();
    void writeSnapshot(std::int64_t simMs, const MetricsRegistry& metrics);

private:
    std::ofstream _out;
    bool _headerWritten{ false };
};