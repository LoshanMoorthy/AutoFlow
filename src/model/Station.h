#ifndef AUTOFLOW_MODEL_STATION_H
#define AUTOFLOW_MODEL_STATION_H

#include <string>
#include <optional>
#include <cstdint>

#include "Queue.h"
#include "Item.h"
#include "../util/Random.h"
#include "../metrics/Metrics.h"

struct StationConfig {
    std::string name{};
    int serviceTimeMsMin{ 300 };
    int serviceTimeMsMax{ 900 };

    double failureRatePerSec{ 0.001 };
    int downtimeMsMin{ 2000 };
    int downtimeMsMax{ 6000 };

    std::size_t queueCapacity{ 100 };

    StationConfig() = default;

    StationConfig(
        std::string n,
        int svcMin,
        int svcMax,
        double failRate,
        int downMin,
        int downMax,
        std::size_t qCap = 100)
        : name(std::move(n))
        , serviceTimeMsMin(svcMin)
        , serviceTimeMsMax(svcMax)
        , failureRatePerSec(failRate)
        , downtimeMsMin(downMin)
        , downtimeMsMax(downMax)
        , queueCapacity(qCap) {
    }
};

class Station {
public:
    explicit Station(StationConfig cfg);

    bool enqueue(Item item);
    std::size_t queueDepth() const { return _in.size(); }
    std::size_t queueCapacity() const { return _in.capacity(); }
    bool canAccept() const { return _in.canPush(); }

    bool isBusy() const { return _busy; }
    bool hasCompletedItemReady(std::int64_t simMs) const;
    Item takeCompletedItem();
    void setBlocked(bool blocked) { _blocked = blocked; }
    bool isBlocked() const { return _blocked; }

    std::optional<Item> tick(std::int64_t simMs, double dtSeconds, Random& rng, MetricsRegistry& metrics);

    const std::string& name() const { return _cfg.name; }
    bool isDown(std::int64_t simMs) const { return _downUntilMs > simMs; }

private:
    StationConfig _cfg;
    Queue<Item> _in;

    bool _busy{ false };
    bool _blocked{ false };
    Item _current{};
    std::int64_t _finishAtMs{ 0 };

    std::int64_t _downUntilMs{ 0 };

    std::int64_t _uptimeMs{ 0 };
    std::int64_t _busyMs{ 0 };
};

#endif