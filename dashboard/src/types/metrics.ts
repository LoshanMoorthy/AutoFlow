export interface StationMetrics {
  name: string;
  utilization: number;
  queueDepth: number;
  queueCapacity: number;
  isDown: boolean;
  isBlocked: boolean;
  isBusy: boolean;
}

export interface SimMetrics {
  itemsInSystem: number;
  itemsCreated: number;
  itemsCompleted: number;
  itemsDropped: number;
  ordersTotal: number;
  ordersCompleted: number;
  ordersTarget: number;
  ordersCompletionPct: number;
  latencyAvgMs: number;
  latencyMaxMs: number;
  stations: StationMetrics[];
}

export interface ItemEvent {
  itemId: number;
  orderId: number;
  station: string;
  enteredMs: number;
  leftMs: number;
  dwellMs: number;
}

export interface ItemJourney {
  itemId: number;
  orderId: number;
  events: ItemEvent[];
  totalMs: number;
}