#ifndef ARRAY_H
#define ARRAY_H
#include <memory>
#include <cstdint>
#include "position.h"
template <typename T>
struct Array
{
	//std::unique_ptr<T[]> data;
	std::vector<T> vData;
	uint32_t length;
	uint32_t width;
	Array(int width) : width(width), vData(width){}
	Array() {}
	/*Array() : width(0), length(0)
	{
		data = std::make_unique<T[]>(0);
	}
	T operator[](const Position& pos) { return data[(pos.x * length) + pos.y]; }
	Array<T> operator=(const Array& copy)
	{
		this->data = std::move(copy.data);
		this->width = copy.width;
		this->length = copy.length;
	}*/
};

#endif // !ARRAY_H

