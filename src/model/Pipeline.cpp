#include "Pipeline.h"

void Pipeline::addStation(StationConfig cfg) {
    _stations.push_back(std::make_unique<Station>(std::move(cfg)));
}

Station& Pipeline::station(std::size_t idx) {
    return *_stations.at(idx);
}

void Pipeline::pushToFirst(Item item) {
    if (_stations.empty())
        return;

    if (!_stations.front()->enqueue(std::move(item))) { }
}

std::int64_t Pipeline::itemsInSystem() const {
    std::int64_t sum = 0;
    for (const auto& st : _stations) {
        sum += static_cast<std::int64_t>(st->queueDepth());
        if (st->isBusy())
            sum += 1;
    }
    return sum;
}

std::int64_t Pipeline::tick(std::int64_t simMs, double dtSeconds, Random& rng, MetricsRegistry& metrics, OrderTracker& orderTracker) {
    std::int64_t completed = 0;

    for (std::size_t i = 0; i < _stations.size(); ++i) {
        auto out = _stations[i]->tick(simMs, dtSeconds, rng, metrics);
        if (!out.has_value())
            continue;

        if (i + 1 < _stations.size()) {
            if (_stations[i + 1]->canAccept()) {
                Item moved = _stations[i]->takeCompletedItem();
                _stations[i + 1]->enqueue(std::move(moved));
                _stations[i]->setBlocked(false);
            }
            else {
                _stations[i]->setBlocked(true);
                metrics.incCounter("items_blocked_total{" + _stations[i]->name() + "}");
            }
        }
        else {
            Item finished = _stations[i]->takeCompletedItem();
            completed++;
            metrics.incCounter("items_completed_total");

            const std::int64_t latencyMs = simMs - finished.enteredPipelineMs;
            metrics.incCounter("latency_sum_ms", latencyMs);

            const auto completedTotal = metrics.counter("items_completed_total");
            const auto sum = metrics.counter("latency_sum_ms");
            const double avg = (completedTotal > 0)
                ? (static_cast<double>(sum) / static_cast<double>(completedTotal))
                : 0.0;

            metrics.setGauge("latency_avg_ms", avg);

            const auto prevMax = static_cast<std::int64_t>(metrics.gauge("latency_max_ms"));
            if (latencyMs > prevMax)
                metrics.setGauge("latency_max_ms", static_cast<double>(latencyMs));

            const bool orderCompletedNow = orderTracker.completeItem(finished.orderId);
            if (orderCompletedNow)
                metrics.incCounter("orders_completed_total");
        }
    }

    return completed;
}