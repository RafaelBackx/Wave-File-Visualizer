#ifndef ARRAY_H
#define ARRAY_H
#include <memory>
#include <cstdint>
#include "position.h"
template <typename T>
struct Array
{
	std::unique_ptr<T[]> data;
	uint32_t length;
	uint32_t width;
	T operator[](const Position& pos) { return data[(pos.x * length) + pos.y]; }
};

#endif // !ARRAY_H

