#pragma once
#include <cstdint>

struct Item {
	std::uint64_t id{};
	std::int64_t createdMs{};
	std::int64_t enteredPipelineMs{};
};