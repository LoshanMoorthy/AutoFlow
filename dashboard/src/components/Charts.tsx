import type { SimMetrics } from '../types/metrics';

interface Props {
  metrics: SimMetrics;
}

function utilColor(u: number): string {
  if (u > 0.85) return '#c94040';
  if (u > 0.65) return '#d4893a';
  return '#3a9e6a';
}

function queueColor(depth: number, cap: number): string {
  const r = depth / cap;
  if (r > 0.85) return '#c94040';
  if (r > 0.6) return '#d4893a';
  return '#3a7bd5';
}

function BarRow({ label, pct, color, valLabel }: { label: string; pct: number; color: string; valLabel: string }) {
  return (
    <div style={{ display: 'flex', alignItems: 'center', gap: 12 }}>
      <div style={{ width: 72, fontSize: 12, color: '#9094a8', textAlign: 'right', flexShrink: 0 }}>{label}</div>
      <div style={{ flex: 1, height: 16, background: '#1a1f2e', borderRadius: 4, overflow: 'hidden' }}>
        <div style={{ height: '100%', borderRadius: 4, background: color, width: `${pct}%`, transition: 'width 0.8s ease' }} />
      </div>
      <div style={{ width: 52, fontSize: 12, fontWeight: 700, color, textAlign: 'right', flexShrink: 0 }}>{valLabel}</div>
    </div>
  );
}

function ChartCard({ title, children }: { title: string; children: React.ReactNode }) {
  return (
    <div style={{
      background: '#0b0f1a', border: '1px solid rgba(255,255,255,0.06)',
      borderRadius: 10, padding: '18px 20px', flex: 1,
    }}>
      <div style={{ fontSize: 11, color: '#7a7f94', textTransform: 'uppercase', letterSpacing: '.08em', marginBottom: 16, fontWeight: 600 }}>
        {title}
      </div>
      <div style={{ display: 'flex', flexDirection: 'column', gap: 12 }}>
        {children}
      </div>
    </div>
  );
}

export default function Charts({ metrics }: Props) {
  return (
    <div style={{ display: 'flex', gap: 12 }}>
      <ChartCard title="Station utilization">
        {metrics.stations.map(s => (
          <BarRow
            key={s.name}
            label={s.name}
            pct={Math.round(s.utilization * 100)}
            color={utilColor(s.utilization)}
            valLabel={`${Math.round(s.utilization * 100)}%`}
          />
        ))}
      </ChartCard>
      <ChartCard title="Queue depth vs capacity">
        {metrics.stations.map(s => (
          <BarRow
            key={s.name}
            label={s.name}
            pct={s.queueCapacity > 0 ? Math.round((s.queueDepth / s.queueCapacity) * 100) : 0}
            color={queueColor(s.queueDepth, s.queueCapacity)}
            valLabel={`${s.queueDepth}/${s.queueCapacity}`}
          />
        ))}
      </ChartCard>
    </div>
  );
}