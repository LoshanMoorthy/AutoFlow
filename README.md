# AutoFlow
 
A event warehouse logistics simulator written in C++, with a live React dashboard for real-time monitoring and post-run item journey tracing.
 
## What it does

AutoFlow simulates items flowing through a 5-station warehouse pipeline:

**Receive → Decant → Pick → Pack → Dispatch**

Each station has configurable service times, failure rates, and queue capacity. Items are grouped into orderse. The sim tracks throughput, latency, utilization, blockages, and downtime which is exposed live over a Prometheus HTTP endpoint.

The dashboard visualises the pipeline in real time and allows you to trace any order's journey through every station after a run.

---

## Requirements
 
**Sim:**
- CMake 3.20+
- MSVC / GCC / Clang with C++20
- vcpkg with `httplib` and `nlohmann-json`

**Dashboard:**
- Node 18+

---

## Building the sim
 
```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake
cmake --build .
```
 
---

## Running
 
**1. Start the sim**
 
```bash
# Run a named scenario
./AutoFlow Normal
./AutoFlow PeakDemand
./AutoFlow BottleneckAtPick
./AutoFlow EquipmentFailure
./AutoFlow RecoveryShift
 
# Or load a custom config
./AutoFlow config/sample.json
```

The sim shows metrics at `http://localhost:9100/metrics` and item events at `http://localhost:9100/items`.

**2. Start the dashboard**
 
```bash
cd dashboard
npm install
npm run dev
```
 
Open `http://localhost:5173`.

---

## Scenarios
 
| Scenario | Description |
|---|---|
| `Normal` | Baseline - normal load across all the stations |
| `PeakDemand` | 3x spawn rate, stress tests queue capacity |
| `BottleneckAtPick` | Pick station severely lowered - simulates understaffing |
| `EquipmentFailure` | Decant has high failure rate and long downtime |
| `RecoveryShift` | Faster service times across all stations |

---

## Dashboard
 
### Pipeline view
- Live station status - busy / blocked / down / idle
- Queue depth vs capacity per station
- Utilization bars with color coding (green → amber → red)
- Order completion progress
- Items dropped warning when first queue is full
### Item journey view
- Select any order from a completed run
- Gantt-style timeline showing each item's path through every station
- Per-station dwell time breakdown
- Total end-to-end latency per item
---
 
## Output files
 
| File | Contents |
|---|---|
| `autoflow_run.csv` | Metric snapshots every N ms |
| `autoflow_items.csv` | Per-item station entry/exit events |
 
---
 
## Optional: Prometheus + Grafana
 
```bash
cd ops
docker compose up -d
```
 
Prometheus scrapes `localhost:9100/metrics`. Grafana available at `localhost:3000`.
