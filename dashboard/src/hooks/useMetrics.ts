import { useState, useEffect } from 'react';
import type { SimMetrics, StationMetrics } from '../types/metrics';

const METRICS_URL = 'http://localhost:9100/metrics';
const STATIONS = ['Receive', 'Decant', 'Pick', 'Pack', 'Dispatch'];
const REFRESH_MS = 1500;

function parsePrometheus(text: string): Record<string, number> {
  const result: Record<string, number> = {};
  for (const line of text.split('\n')) {
    if (line.startsWith('#') || !line.trim()) continue;
    const m = line.match(/^([a-zA-Z_]+)(?:\{([^}]*)\})?\s+([\d.e+\-]+)/);
    if (!m) continue;
    const [, name, labels, valStr] = m;
    const key = labels ? `${name}{${labels}}` : name;
    result[key] = parseFloat(valStr);
  }
  return result;
}

function get(data: Record<string, number>, name: string, station?: string): number {
  if (station) {
    return data[`${name}{station="${station}"}`] ?? 0;
  }
  return data[name] ?? 0;
}

function buildMetrics(data: Record<string, number>): SimMetrics {
  const stations: StationMetrics[] = STATIONS.map(name => ({
    name,
    utilization: get(data, 'station_util', name),
    queueDepth: get(data, 'queue_depth', name),
    queueCapacity: get(data, 'queue_capacity', name),
    isDown: get(data, 'station_down', name) === 1,
    isBlocked: get(data, 'station_blocked', name) === 1,
    isBusy: get(data, 'station_util', name) > 0.1,
  }));

  return {
    itemsInSystem: get(data, 'items_in_system'),
    itemsCreated: get(data, 'items_created_total'),
    itemsCompleted: get(data, 'items_completed_total'),
    itemsDropped: get(data, 'items_dropped_total'),
    ordersTotal: get(data, 'orders_total'),
    ordersCompleted: get(data, 'orders_completed'),
    ordersTarget: get(data, 'orders_target'),
    ordersCompletionPct: get(data, 'orders_completion_pct'),
    latencyAvgMs: get(data, 'latency_avg_ms'),
    latencyMaxMs: get(data, 'latency_max_ms'),
    stations,
  };
}

export function useMetrics() {
  const [metrics, setMetrics] = useState<SimMetrics | null>(null);
  const [connected, setConnected] = useState(false);
  const [lastUpdate, setLastUpdate] = useState<Date | null>(null);

  useEffect(() => {
    let timer: number;

    async function poll() {
      try {
        const res = await fetch(METRICS_URL, { cache: 'no-store' });
        const text = await res.text();
        const data = parsePrometheus(text);
        setMetrics(buildMetrics(data));
        setConnected(true);
        setLastUpdate(new Date());
      } catch {
        setConnected(false);
      }
      timer = window.setTimeout(poll, REFRESH_MS);
    }

    poll();
    return () => clearTimeout(timer);
  }, []);

  return { metrics, connected, lastUpdate };
}