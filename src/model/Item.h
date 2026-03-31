#pragma once
#include <cstdint>

struct Item {
	std::uint64_t id{};
	std::uint64_t orderId{};
	std::int32_t orderItemIndex{};
	std::int64_t createdMs{};
	std::int64_t enteredPipelineMs{};
};