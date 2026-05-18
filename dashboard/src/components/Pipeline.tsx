import type { SimMetrics } from '../types/metrics';
import StationCard from './StationCard';

interface Props {
  metrics: SimMetrics;
}

function FlowArrow() {
  return (
    <div style={{ width: 40, flexShrink: 0, display: 'flex', flexDirection: 'column', alignItems: 'center', gap: 4 }}>
      <div style={{ width: 28, height: 1, background: 'linear-gradient(90deg, rgba(42,181,160,0.15), rgba(42,181,160,0.4), rgba(42,181,160,0.15))' }} />
      <div style={{ fontSize: 9, color: 'rgba(42,181,160,0.3)', animation: 'flow 1.8s ease-in-out infinite' }}>▶</div>
    </div>
  );
}

export default function Pipeline({ metrics }: Props) {
  return (
    <div style={{
      background: '#0b0f1a', border: '1px solid rgba(255,255,255,0.06)',
      borderRadius: 10, padding: '18px 20px',
    }}>
      <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: 16 }}>
        <div style={{ fontSize: 10, color: '#2e3352', textTransform: 'uppercase', letterSpacing: '.1em', fontWeight: 600 }}>
          Live pipeline
        </div>
        <div style={{ fontSize: 10, color: '#2e3352' }}>
          {metrics.itemsInSystem} items in flight
        </div>
      </div>
      <div style={{ display: 'flex', alignItems: 'stretch' }}>
        {metrics.stations.map((station, i) => (
          <div key={station.name} style={{ display: 'flex', alignItems: 'center', flex: 1 }}>
            <StationCard station={station} />
            {i < metrics.stations.length - 1 && <FlowArrow />}
          </div>
        ))}
      </div>
    </div>
  );
}