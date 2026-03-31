#include "MetricsServer.h"
#include <httplib.h>

MetricsServer::MetricsServer(MetricsRegistry& metrics, int port)
    : _metrics(metrics), _port(port) {
}

MetricsServer::~MetricsServer() {
    stop();
}

void MetricsServer::start() {
    if (_running.exchange(true)) return;

    _thread = std::thread([this]() {
        httplib::Server srv;

        srv.Get("/health", [](const httplib::Request&, httplib::Response& res) {
            res.set_content("ok", "text/plain");
        });

        srv.Get("/metrics", [this](const httplib::Request&, httplib::Response& res) {
            const auto body = _metrics.toPrometheusText();
            res.set_content(body, "text/plain; version=0.0.4");
        });

        // Blocking listen loop
        srv.listen("0.0.0.0", _port);
    });
}

void MetricsServer::stop() {
    if (!_running.exchange(false)) return;

    if (_thread.joinable())
        _thread.detach();
}