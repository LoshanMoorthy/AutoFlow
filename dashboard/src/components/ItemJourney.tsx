import { useEffect, useState } from 'react';
import { useItemLog } from '../hooks/useItemLog';
import type { ItemEvent } from '../types/metrics';

const STATIONS = ['Receive', 'Decant', 'Pick', 'Pack', 'Dispatch'];

function stationColor(station: string): string {
  const colors: Record<string, string> = {
    Receive: '#3a7bd5', Decant: '#7c6fcd', Pick: '#2ab5a0',
    Pack: '#d4893a', Dispatch: '#3a9e6a',
  };
  return colors[station] ?? '#888';
}

function formatMs(ms: number): string {
  if (ms < 1000) return `${ms}ms`;
  return `${(ms / 1000).toFixed(1)}s`;
}

function Timeline({ events, totalMs }: { events: ItemEvent[]; totalMs: number }) {
  const start = Math.min(...events.map(e => e.enteredMs));

  return (
    <div style={{ display: 'flex', flexDirection: 'column', gap: 10 }}>
      {STATIONS.map(station => {
        const stationEvents = events.filter(e => e.station === station);
        const color = stationColor(station);
        return (
          <div key={station}>
            <div style={{ display: 'flex', alignItems: 'center', gap: 12, marginBottom: 6 }}>
              <div style={{ width: 72, fontSize: 11, fontWeight: 700, color, textAlign: 'right', letterSpacing: '.04em', textTransform: 'uppercase' }}>
                {station}
              </div>
              <div style={{ flex: 1, height: 28, background: '#0f1420', borderRadius: 4, position: 'relative', overflow: 'hidden' }}>
                {stationEvents.map((e, i) => {
                  const left = totalMs > 0 ? ((e.enteredMs - start) / totalMs) * 100 : 0;
                  const width = totalMs > 0 ? ((e.dwellMs) / totalMs) * 100 : 0;
                  return (
                    <div key={i} title={`Item ${e.itemId} — ${formatMs(e.dwellMs)}`} style={{
                      position: 'absolute', top: 3, height: 22, borderRadius: 3,
                      left: `${left}%`, width: `${Math.max(width, 0.5)}%`,
                      background: color, opacity: 0.8,
                      boxShadow: `0 0 6px ${color}60`,
                      cursor: 'default',
                    }} />
                  );
                })}
              </div>
              <div style={{ width: 60, fontSize: 11, color: '#4a4f66', textAlign: 'right' }}>
                {stationEvents.length > 0
                  ? formatMs(stationEvents.reduce((s, e) => s + e.dwellMs, 0))
                  : '—'}
              </div>
            </div>
          </div>
        );
      })}
      <div style={{ display: 'flex', gap: 12, paddingLeft: 84, marginTop: 4 }}>
        <div style={{ fontSize: 10, color: '#2e3352' }}>0ms</div>
        <div style={{ flex: 1 }} />
        <div style={{ fontSize: 10, color: '#2e3352' }}>{formatMs(totalMs)}</div>
      </div>
    </div>
  );
}

function ItemCard({ events, itemId }: { events: ItemEvent[]; itemId: number }) {
  const totalDwell = events.reduce((s, e) => s + e.dwellMs, 0);
  return (
    <div style={{
      background: '#0b0f1a', border: '1px solid rgba(255,255,255,0.06)',
      borderRadius: 8, padding: '12px 16px', marginBottom: 8,
    }}>
      <div style={{ display: 'flex', justifyContent: 'space-between', marginBottom: 10 }}>
        <div style={{ fontSize: 12, fontWeight: 700, color: '#9094a8' }}>Item #{itemId}</div>
        <div style={{ fontSize: 11, color: '#4a4f66' }}>total: {formatMs(totalDwell)}</div>
      </div>
      <div style={{ display: 'flex', gap: 6, flexWrap: 'wrap' }}>
        {events.map((e, i) => (
          <div key={i} style={{
            fontSize: 10, padding: '3px 8px', borderRadius: 4,
            background: `${stationColor(e.station)}15`,
            color: stationColor(e.station),
            border: `1px solid ${stationColor(e.station)}30`,
          }}>
            {e.station} · {formatMs(e.dwellMs)}
          </div>
        ))}
      </div>
    </div>
  );
}

export default function ItemJourney() {
  const { journey, allOrderIds, loading, error, loadOrders, loadJourney } = useItemLog();
  const [selectedOrder, setSelectedOrder] = useState<number | ''>('');

  useEffect(() => { loadOrders(); }, [loadOrders]);

  const itemIds = journey ? [...new Set(journey.events.map(e => e.itemId))].sort((a, b) => a - b) : [];

  return (
    <div style={{ display: 'flex', flexDirection: 'column', gap: 16 }}>
      <div style={{
        background: '#0b0f1a', border: '1px solid rgba(255,255,255,0.06)',
        borderRadius: 10, padding: '18px 20px',
      }}>
        <div style={{ fontSize: 10, color: '#2e3352', textTransform: 'uppercase', letterSpacing: '.1em', fontWeight: 600, marginBottom: 14 }}>
          Order journey
        </div>
        <div style={{ display: 'flex', gap: 10, alignItems: 'center' }}>
          <select
            value={selectedOrder}
            onChange={e => setSelectedOrder(e.target.value === '' ? '' : parseInt(e.target.value))}
            style={{
              background: '#0f1420', border: '1px solid rgba(255,255,255,0.1)',
              color: '#e8eaf0', borderRadius: 6, padding: '8px 12px', fontSize: 13, flex: 1,
            }}
          >
            <option value="">Select an order...</option>
            {allOrderIds.map(id => (
              <option key={id} value={id}>Order #{id}</option>
            ))}
          </select>
          <button
            onClick={() => selectedOrder !== '' && loadJourney(selectedOrder as number)}
            disabled={selectedOrder === '' || loading}
            style={{
              background: '#2ab5a0', border: 'none', color: '#080b12',
              borderRadius: 6, padding: '8px 20px', fontSize: 13, fontWeight: 700,
              cursor: selectedOrder === '' ? 'not-allowed' : 'pointer',
              opacity: selectedOrder === '' ? 0.4 : 1,
            }}
          >
            {loading ? 'Loading...' : 'Trace'}
          </button>
          <button
            onClick={loadOrders}
            style={{
              background: 'transparent', border: '1px solid rgba(255,255,255,0.1)',
              color: '#4a4f66', borderRadius: 6, padding: '8px 14px', fontSize: 13, cursor: 'pointer',
            }}
          >
            ↻
          </button>
        </div>
        {error && <div style={{ marginTop: 10, fontSize: 12, color: '#c94040' }}>{error}</div>}
      </div>

      {journey && (
        <>
          <div style={{
            background: '#0b0f1a', border: '1px solid rgba(255,255,255,0.06)',
            borderRadius: 10, padding: '18px 20px',
          }}>
            <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: 16 }}>
              <div style={{ fontSize: 10, color: '#2e3352', textTransform: 'uppercase', letterSpacing: '.1em', fontWeight: 600 }}>
                Pipeline timeline — Order #{journey.orderId}
              </div>
              <div style={{ display: 'flex', gap: 16, fontSize: 11, color: '#4a4f66' }}>
                <span>{itemIds.length} items</span>
                <span>total span: {formatMs(journey.totalMs)}</span>
              </div>
            </div>
            <Timeline events={journey.events} totalMs={journey.totalMs} />
          </div>

          <div style={{
            background: '#0b0f1a', border: '1px solid rgba(255,255,255,0.06)',
            borderRadius: 10, padding: '18px 20px',
          }}>
            <div style={{ fontSize: 10, color: '#2e3352', textTransform: 'uppercase', letterSpacing: '.1em', fontWeight: 600, marginBottom: 14 }}>
              Item breakdown
            </div>
            {itemIds.map(id => (
              <ItemCard
                key={id}
                itemId={id}
                events={journey.events.filter(e => e.itemId === id)}
              />
            ))}
          </div>
        </>
      )}
    </div>
  );
}