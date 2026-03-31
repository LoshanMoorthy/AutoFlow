#include "JsonConfig.h"
#include <fstream>
#include <sstream>

static bool tryReadInt(const std::string& s, const std::string& key, int& out) {
    const std::string pat = "\"" + key + "\"";
    auto p = s.find(pat);
    if (p == std::string::npos) return false;

    p = s.find(':', p);
    if (p == std::string::npos) return false;
    p++;

    while (p < s.size() && (s[p] == ' ' || s[p] == '\t')) p++;

    int sign = 1;
    if (p < s.size() && s[p] == '-') { sign = -1; p++; }

    int v = 0;
    bool any = false;
    while (p < s.size() && s[p] >= '0' && s[p] <= '9') {
        v = v * 10 + (s[p] - '0');
        p++;
        any = true;
    }

    if (!any) return false;
    out = v * sign;
    return true;
}

static std::string readAll(const std::string& path) {
    std::ifstream f(path);
    if (!f) return {};
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

AppConfig loadConfigOrDefaults(const std::string& path) {
    AppConfig cfg;

    // Default pipeline
    cfg.stations = {
        {"Receive",  200, 500, 0.0005, 2000, 5000, 50},
        {"Decant",   300, 700, 0.0008, 2000, 6000, 30},
        {"Pick",     250, 600, 0.0012, 1500, 4500, 20},
        {"Pack",     300, 800, 0.0007, 2000, 5000, 15},
        {"Dispatch", 200, 500, 0.0004, 2000, 5000, 15},
    };

    auto text = readAll(path);
    if (text.empty()) return cfg;

    (void)tryReadInt(text, "tickMs", cfg.engine.tickMs);
    (void)tryReadInt(text, "durationSeconds", cfg.engine.durationSeconds);
    (void)tryReadInt(text, "spawnPerSecond", cfg.engine.spawnPerSecond);
    (void)tryReadInt(text, "csvEveryMs", cfg.engine.csvEveryMs);
    (void)tryReadInt(text, "realTime", cfg.engine.realTime);
    (void)tryReadInt(text, "metricsPort", cfg.engine.metricsPort);
    (void)tryReadInt(text, "ordersTarget", cfg.engine.ordersTarget);
    (void)tryReadInt(text, "orderItemsMin", cfg.engine.orderItemsMin);
    (void)tryReadInt(text, "orderItemsMax", cfg.engine.orderItemsMax);

    if (cfg.engine.metricsPort <= 0) cfg.engine.metricsPort = 9100;
    if (cfg.engine.realTime != 0) cfg.engine.realTime = 1;
    if (cfg.engine.ordersTarget <= 0) cfg.engine.ordersTarget = 100;
    if (cfg.engine.orderItemsMin <= 0) cfg.engine.orderItemsMin = 1;
    if (cfg.engine.orderItemsMax < cfg.engine.orderItemsMin) cfg.engine.orderItemsMax = cfg.engine.orderItemsMin;

    return cfg;
}