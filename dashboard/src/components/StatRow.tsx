import type { SimMetrics } from '../types/metrics';

interface Props {
  metrics: SimMetrics;
}

function StatCard({ label, value, sub, color, accent }: {
  label: string; value: string | number; sub?: string; color?: string; accent?: string;
}) {
  return (
    <div style={{
      background: '#0b0f1a',
      border: '1px solid rgba(255,255,255,0.06)',
      borderRadius: 10, padding: '16px 20px',
      position: 'relative', overflow: 'hidden',
      boxShadow: accent ? `0 0 20px -8px ${accent}40` : 'none',
    }}>
      {accent && (
        <div style={{
          position: 'absolute', top: 0, left: 0, right: 0, height: 2,
          background: `linear-gradient(90deg, transparent, ${accent}, transparent)`,
        }} />
      )}
      <div style={{ fontSize: 11, color: '#5a6080', textTransform: 'uppercase', letterSpacing: '.1em', marginBottom: 10, fontWeight: 600 }}>
        {label}
      </div>
      <div style={{ fontSize: 28, fontWeight: 700, lineHeight: 1, color: color ?? '#c8ccd8', fontVariantNumeric: 'tabular-nums' }}>
        {value}
      </div>
      {sub && <div style={{ fontSize: 12, color: '#5a6080', marginTop: 6 }}>{sub}</div>}
    </div>
  );
}

export default function StatRow({ metrics }: Props) {
  const pct = Math.round(metrics.ordersCompletionPct);
  return (
    <>
      {metrics.itemsDropped > 0 && (
        <div style={{
          display: 'flex', alignItems: 'center', gap: 10,
          background: 'rgba(201,64,64,0.07)', border: '1px solid rgba(201,64,64,0.25)',
          borderRadius: 8, padding: '10px 16px', fontSize: 13, color: '#e07070',
        }}>
          <span>⚠</span>
          <span><strong style={{ color: '#c94040' }}>{Math.round(metrics.itemsDropped)}</strong> items dropped — Receive queue is full</span>
        </div>
      )}
      <div style={{ display: 'grid', gridTemplateColumns: 'repeat(6, 1fr)', gap: 10 }}>
        <StatCard label="In system"   value={Math.round(metrics.itemsInSystem)}   color="#2ab5a0" accent="#2ab5a0" />
        <StatCard label="Created"     value={Math.round(metrics.itemsCreated)}     color="#c8ccd8" />
        <StatCard label="Completed"   value={Math.round(metrics.itemsCompleted)}   color="#3a9e6a" accent="#3a9e6a" />
        <StatCard label="Orders done" value={Math.round(metrics.ordersCompleted)}  color="#3a9e6a" accent="#3a9e6a" sub={`of ${Math.round(metrics.ordersTarget)}`} />
        <StatCard label="Avg latency" value={`${Math.round(metrics.latencyAvgMs)}ms`} color="#c8ccd8" />
        <StatCard label="Max latency" value={`${Math.round(metrics.latencyMaxMs)}ms`} color="#d4893a" accent="#d4893a" />
      </div>
      <div style={{
        background: '#0b0f1a', border: '1px solid rgba(255,255,255,0.06)',
        borderRadius: 10, padding: '16px 20px',
      }}>
        <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: 10 }}>
          <div style={{ fontSize: 12, color: '#7a7f94', textTransform: 'uppercase', letterSpacing: '.1em', fontWeight: 600 }}>
            Order completion
          </div>
          <div style={{ fontSize: 15, fontWeight: 700, color: '#2ab5a0' }}>{pct}%</div>
        </div>
        <div style={{ height: 8, background: '#0f1420', borderRadius: 4, overflow: 'hidden', position: 'relative' }}>
          <div style={{
            height: '100%', borderRadius: 4,
            background: 'linear-gradient(90deg, #1d9e75, #2ab5a0)',
            width: `${pct}%`, transition: 'width 1s ease',
            position: 'relative', overflow: 'hidden',
          }}>
            <div style={{
              position: 'absolute', top: 0, bottom: 0, width: '30%',
              background: 'linear-gradient(90deg, transparent, rgba(255,255,255,0.15), transparent)',
              animation: 'shimmer 2.5s infinite',
            }} />
          </div>
        </div>
        <div style={{ display: 'flex', justifyContent: 'space-between', marginTop: 8, fontSize: 12, color: '#7a7f94' }}>
          <span>{Math.round(metrics.ordersCompleted)} completed</span>
          <span>{Math.round(metrics.ordersTarget - metrics.ordersCompleted)} remaining</span>
        </div>
      </div>
    </>
  );
}