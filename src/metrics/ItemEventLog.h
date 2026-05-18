#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <cstdint>

class ItemEventLog {
public:
    explicit ItemEventLog(const std::string& path);

    void record(
        std::uint64_t itemId,
        std::uint64_t orderId,
        const std::string& station,
        std::int64_t enteredMs,
        std::int64_t leftMs
    );

private:
    std::ofstream _file;
    std::mutex _mtx;
    bool _headerWritten{ false };
};