#include "MetricsServer.h"
#include <httplib.h>
#include <fstream>
#include <sstream>

MetricsServer::MetricsServer(MetricsRegistry& metrics, int port)
    : _metrics(metrics), _port(port) {
}

MetricsServer::~MetricsServer() {
    stop();
}

void MetricsServer::start() {
    if (_running.exchange(true)) return;

    _srv.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("ok", "text/plain");
    });

    _srv.Get("/metrics", [this](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        const auto body = _metrics.toPrometheusText();
        res.set_content(body, "text/plain; version=0.0.4");
    });

    _srv.Get("/items", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        std::ifstream f("autoflow_items.csv");
        if (!f) { res.set_content("", "text/plain"); return; }
        std::ostringstream ss;
        ss << f.rdbuf();
        res.set_content(ss.str(), "text/plain");
    });

    _thread = std::thread([this]() {
        _srv.listen("0.0.0.0", _port);
    });
}

void MetricsServer::stop() {
    if (!_running.exchange(false)) return;

    _srv.stop();

    if (_thread.joinable())
        _thread.join();
}