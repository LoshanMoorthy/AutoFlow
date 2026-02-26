#include "Pipeline.h"

void Pipeline::addStation(StationConfig cfg) {
	_stations.push_back(std::make_unique<Station>(std::move(cfg)));
}

Station& Pipeline::station(std::size_t idx) {
	return *_stations.at(idx);
}

void Pipeline::pushToFirst(Item item) {
	if (_stations.empty()) return;
	_stations.front()->enqueue(std::move(item));
}

std::int64_t Pipeline::tick(std::int64_t simMs, double dtSeconds, Random& rng, MetricsRegistry& metrics) {
	std::int64_t completed = 0;

	for (std::size_t i = 0; i < _stations.size(); i++) {
		auto out = _stations[i]->tick(simMs, dtSeconds, rng, metrics);
		if (!out.has_value()) continue;

		if (i + 1 < _stations.size()) {
			_stations[i + 1]->enqueue(std::move(*out));
		}
		else {
			completed++;
			metrics.incCounter("items_completed_total");
		}
	}

	return completed;
}