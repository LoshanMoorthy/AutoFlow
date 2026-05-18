import { useState, useCallback } from 'react';
import type { ItemEvent, ItemJourney } from '../types/metrics';

const CSV_URL = 'http://localhost:9100/items';

export function useItemLog() {
  const [journey, setJourney] = useState<ItemJourney | null>(null);
  const [allOrderIds, setAllOrderIds] = useState<number[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

  const loadOrders = useCallback(async () => {
    setLoading(true);
    setError(null);
    try {
      const res = await fetch(CSV_URL, { cache: 'no-store' });
      const text = await res.text();
      const lines = text.trim().split('\n').slice(1);
      const ids = new Set<number>();
      for (const line of lines) {
        const parts = line.split(',');
        if (parts.length >= 2) ids.add(parseInt(parts[1]));
      }
      setAllOrderIds([...ids].sort((a, b) => a - b));
    } catch {
      setError('Could not load item log');
    }
    setLoading(false);
  }, []);

  const loadJourney = useCallback(async (orderId: number) => {
    setLoading(true);
    setError(null);
    try {
      const res = await fetch(CSV_URL, { cache: 'no-store' });
      const text = await res.text();
      const lines = text.trim().split('\n').slice(1);

      const events: ItemEvent[] = [];
      for (const line of lines) {
        const [itemId, oId, station, enteredMs, leftMs, dwellMs] = line.split(',');
        if (parseInt(oId) !== orderId) continue;
        events.push({
          itemId: parseInt(itemId),
          orderId: parseInt(oId),
          station,
          enteredMs: parseInt(enteredMs),
          leftMs: parseInt(leftMs),
          dwellMs: parseInt(dwellMs),
        });
      }

      if (events.length === 0) {
        setError(`No events found for order ${orderId}`);
        setJourney(null);
        setLoading(false);
        return;
      }

      const totalMs = Math.max(...events.map(e => e.leftMs)) - Math.min(...events.map(e => e.enteredMs));

      setJourney({ itemId: events[0].itemId, orderId, events, totalMs });
    } catch {
      setError('Could not load item log');
    }
    setLoading(false);
  }, []);

  return { journey, allOrderIds, loading, error, loadOrders, loadJourney };
}