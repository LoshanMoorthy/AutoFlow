import type { StationMetrics } from '../types/metrics';

interface Props {
  station: StationMetrics;
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

export default function StationCard({ station }: Props) {
  const uColor = utilColor(station.utilization);
  const qColor = queueColor(station.queueDepth, station.queueCapacity);
  const qPct = station.queueCapacity > 0 ? (station.queueDepth / station.queueCapacity) : 0;
  const dots = Math.min(station.queueDepth, 24);

  let borderColor = 'rgba(255,255,255,0.06)';
  let animClass = '';
  let statusText = 'idle';
  let statusBg = 'rgba(255,255,255,0.04)';
  let statusColor = '#7a7f94';

  if (station.isDown) {
    borderColor = 'rgba(201,64,64,0.4)';
    animClass = 'station-down';
    statusText = 'down'; statusBg = 'rgba(201,64,64,0.12)'; statusColor = '#c94040';
  } else if (station.isBlocked) {
    borderColor = 'rgba(212,137,58,0.4)';
    animClass = 'station-blocked';
    statusText = 'blocked'; statusBg = 'rgba(212,137,58,0.12)'; statusColor = '#d4893a';
  } else if (station.isBusy) {
    borderColor = 'rgba(58,158,106,0.2)';
    animClass = 'station-busy';
    statusText = 'busy'; statusBg = 'rgba(58,158,106,0.1)'; statusColor = '#3a9e6a';
  }

  return (
    <div className={animClass} style={{
      flex: 1, background: '#0b0f1a',
      border: `1px solid ${borderColor}`,
      borderRadius: 10, padding: '14px 14px',
      transition: 'border-color 0.4s',
    }}>
      <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: 10 }}>
        <div style={{ fontSize: 13, fontWeight: 700, textTransform: 'uppercase', letterSpacing: '.06em', color: '#c8ccd8' }}>
          {station.name}
        </div>
        <span style={{
          fontSize: 10, fontWeight: 700, padding: '3px 8px',
          borderRadius: 3, textTransform: 'uppercase', letterSpacing: '.06em',
          background: statusBg, color: statusColor,
        }}>
          {statusText}
        </span>
      </div>

      <div style={{ display: 'flex', flexWrap: 'wrap', gap: 3, marginBottom: 12, minHeight: 20 }}>
        {Array.from({ length: dots }, (_, i) => (
          <div key={i} style={{
            width: 6, height: 6, borderRadius: 2,
            background: i >= 20 ? '#c94040' : '#3a7bd5',
            opacity: i >= 20 ? 0.9 : 0.5,
            animation: `dot-in 0.2s ease ${i * 0.02}s both`,
          }} />
        ))}
      </div>

      <div style={{ marginBottom: 10 }}>
        <div style={{ display: 'flex', justifyContent: 'space-between', marginBottom: 5 }}>
          <span style={{ fontSize: 11, color: '#7a7f94', letterSpacing: '.04em' }}>Util</span>
          <span style={{ fontSize: 12, fontWeight: 700, color: uColor, fontVariantNumeric: 'tabular-nums' }}>
            {Math.round(station.utilization * 100)}%
          </span>
        </div>
        <div style={{ height: 5, background: '#0f1420', borderRadius: 2, overflow: 'hidden' }}>
          <div style={{
            height: '100%', borderRadius: 2, background: uColor,
            width: `${Math.round(station.utilization * 100)}%`,
            transition: 'width 0.8s ease',
            boxShadow: `0 0 6px ${uColor}80`,
          }} />
        </div>
      </div>

      <div>
        <div style={{ display: 'flex', justifyContent: 'space-between', marginBottom: 5 }}>
          <span style={{ fontSize: 11, color: '#7a7f94', letterSpacing: '.04em' }}>Queue</span>
          <span style={{ fontSize: 12, fontWeight: 700, color: qColor, fontVariantNumeric: 'tabular-nums' }}>
            {station.queueDepth}/{station.queueCapacity}
          </span>
        </div>
        <div style={{ height: 5, background: '#0f1420', borderRadius: 2, overflow: 'hidden' }}>
          <div style={{
            height: '100%', borderRadius: 2, background: qColor,
            width: `${Math.round(qPct * 100)}%`,
            transition: 'width 0.8s ease',
            boxShadow: `0 0 6px ${qColor}80`,
          }} />
        </div>
      </div>
    </div>
  );
}