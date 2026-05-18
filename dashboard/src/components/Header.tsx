interface Props {
  connected: boolean;
  lastUpdate: Date | null;
  children?: React.ReactNode;
}

export default function Header({ connected, lastUpdate, children }: Props) {
  return (
    <header style={{
      display: 'flex', alignItems: 'center', justifyContent: 'space-between',
      padding: '14px 28px', borderBottom: '1px solid rgba(255,255,255,0.08)',
      background: 'rgba(8,11,18,0.95)', backdropFilter: 'blur(8px)',
      position: 'sticky', top: 0, zIndex: 10,
    }}>
      <div style={{ display: 'flex', alignItems: 'center', gap: 14 }}>
        <div style={{ fontSize: 17, fontWeight: 700, letterSpacing: '.03em', color: '#e8eaf0' }}>
          Auto<span style={{ color: '#2ab5a0' }}>Flow</span>
        </div>
        <div style={{
          fontSize: 9, fontWeight: 700, color: '#2ab5a0',
          background: 'rgba(42,181,160,0.08)', border: '1px solid rgba(42,181,160,0.2)',
          borderRadius: 3, padding: '2px 7px', letterSpacing: '.1em', textTransform: 'uppercase',
        }}>
          Simulation
        </div>
      </div>

      {children}

      <div style={{ display: 'flex', alignItems: 'center', gap: 24 }}>
        {lastUpdate && (
          <div style={{ fontSize: 12, color: '#7a7f94', fontVariantNumeric: 'tabular-nums' }}>
            {lastUpdate.toLocaleTimeString()}
          </div>
        )}
        <div style={{
          display: 'flex', alignItems: 'center', gap: 8, fontSize: 12, fontWeight: 600,
          color: connected ? '#3a9e6a' : '#c94040',
          background: connected ? 'rgba(58,158,106,0.07)' : 'rgba(201,64,64,0.07)',
          border: `1px solid ${connected ? 'rgba(58,158,106,0.25)' : 'rgba(201,64,64,0.25)'}`,
          borderRadius: 6, padding: '5px 12px',
        }}>
          <div style={{
            width: 7, height: 7, borderRadius: '50%',
            background: connected ? '#3a9e6a' : '#c94040',
            boxShadow: connected ? '0 0 6px #3a9e6a' : '0 0 6px #c94040',
          }} />
          {connected ? 'Live' : 'Disconnected'}
        </div>
      </div>
    </header>
  );
}