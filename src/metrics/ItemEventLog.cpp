#include "ItemEventLog.h"

ItemEventLog::ItemEventLog(const std::string& path) {
    _file.open(path, std::ios::out | std::ios::trunc);
}

void ItemEventLog::record(
    std::uint64_t itemId,
    std::uint64_t orderId,
    const std::string& station,
    std::int64_t enteredMs,
    std::int64_t leftMs) {
    std::scoped_lock lock(_mtx);

    if (!_headerWritten) {
        _file << "item_id,order_id,station,entered_ms,left_ms,dwell_ms\n";
        _headerWritten = true;
    }

    _file << itemId << ","
        << orderId << ","
        << station << ","
        << enteredMs << ","
        << leftMs << ","
        << (leftMs - enteredMs) << "\n";

    _file.flush();
}