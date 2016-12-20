#pragma once

#include <cinttypes>

template <typename T, uint32_t size>
constexpr uint32_t array_size(const T(&a)[size])
{
	return size;
}