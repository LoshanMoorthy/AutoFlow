#pragma once
#include <cstdint>
#include <unordered_map>

struct OrderState {
	std::int32_t requiredItemCount{ 0 };
	std::int32_t completedItemCount{ 0 };
	bool completed{ false };
};

class OrderTracker {
public:
	void registerOrder(std::uint64_t orderId, std::int32_t requiredItemCount);
	bool completeItem(std::uint64_t orderId);

	std::int64_t totalOrders() const;
	std::int64_t completedOrders() const;
	std::int64_t remainingOrders() const;

private:
	std::unordered_map<std::uint64_t, OrderState> _orders;
	std::int64_t _completedOrders{ 0 };
};