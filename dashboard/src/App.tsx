import { useState } from 'react';
import { useMetrics } from './hooks/useMetrics';
import Header from './components/Header';
import StatRow from './components/StatRow';
import Pipeline from './components/Pipeline';
import Charts from './components/Charts';
import ItemJourney from './components/ItemJourney';

type View = 'live' | 'journey';

export default function App() {
  const { metrics, connected, lastUpdate } = useMetrics();
  const [view, setView] = useState<View>('live');

  const tabStyle = (v: View) => ({
    fontSize: 11, fontWeight: 600, padding: '5px 14px',
    borderRadius: 5, cursor: 'pointer', border: 'none',
    textTransform: 'uppercase' as const, letterSpacing: '.06em',
    background: view === v ? 'rgba(42,181,160,0.12)' : 'transparent',
    color: view === v ? '#2ab5a0' : '#7a7f94',
  });

  return (
    <div style={{ minHeight: '100vh', width: '100%', display: 'flex', flexDirection: 'column', background: '#080b12' }}>
      <Header connected={connected} lastUpdate={lastUpdate}>
        <div style={{ display: 'flex', gap: 4 }}>
          <button style={tabStyle('live')} onClick={() => setView('live')}>Pipeline</button>
          <button style={tabStyle('journey')} onClick={() => setView('journey')}>Item journey</button>
        </div>
      </Header>
      <div style={{ flex: 1, padding: '20px 24px', display: 'flex', flexDirection: 'column', gap: 14, width: '100%' }}>
        {view === 'live' && (
          metrics ? (
            <>
              <StatRow metrics={metrics} />
              <Pipeline metrics={metrics} />
              <Charts metrics={metrics} />
            </>
          ) : (
            <div style={{ flex: 1, display: 'flex', alignItems: 'center', justifyContent: 'center', color: '#2e3352' }}>
              Waiting for sim...
            </div>
          )
        )}
        {view === 'journey' && <ItemJourney />}
      </div>
    </div>
  );
}