#pragma once
#include <vector>
#include <memory>
#include "Station.h"
#include "OrderTracker.h"

class Pipeline {
public:
    void addStation(StationConfig cfg);
    Station& station(std::size_t idx);
    std::size_t count() const { return _stations.size(); }

    void pushToFirst(Item item);
    std::int64_t itemsInSystem() const;

    std::int64_t tick(std::int64_t simMs, double dtSeconds, Random& rng, MetricsRegistry& metrics, OrderTracker& orderTracker);

private:
    std::vector<std::unique_ptr<Station>> _stations;
};