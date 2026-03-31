#pragma once
#include <thread>
#include <atomic>
#include <string>
#include "Metrics.h"

class MetricsServer {
public:
	MetricsServer(MetricsRegistry& metrics, int port);
	~MetricsServer();

	void start();
	void stop();

private:
	MetricsRegistry& _metrics;
	int _port;
	std::thread _thread;
	std::atomic<bool> _running{ false };
};