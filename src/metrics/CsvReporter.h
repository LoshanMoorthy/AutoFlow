#pragma once
#include <string>
#include <fstream>
#include <cstdint>
#include "Metrics.h"

class CsvReporter {
public:
    explicit CsvReporter(std::string path);

    bool writeSnapshot(std::int64_t simMs, const MetricsRegistry& metrics);

private:
    std::string _path;
    bool _headerWritten{ false };

    bool tryOpen(std::ofstream& out);
    void writeHeader(std::ofstream& out);
};