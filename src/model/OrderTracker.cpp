#include "OrderTracker.h"

void OrderTracker::registerOrder(std::uint64_t orderId, std::int32_t requiredItemCount) {
    _orders.emplace(orderId, OrderState{ requiredItemCount, 0, false });
}

bool OrderTracker::completeItem(std::uint64_t orderId) {
    auto it = _orders.find(orderId);
    if (it == _orders.end())
        return false;

    auto& order = it->second;
    order.completedItemCount++;

    if (!order.completed && order.completedItemCount >= order.requiredItemCount) {
        order.completed = true;
        _completedOrders++;
        return true;
    }

    return false;
}

std::int64_t OrderTracker::totalOrders() const {
    return static_cast<std::int64_t>(_orders.size());
}

std::int64_t OrderTracker::completedOrders() const {
    return _completedOrders;
}

std::int64_t OrderTracker::remainingOrders() const {
    return totalOrders() - completedOrders();
}