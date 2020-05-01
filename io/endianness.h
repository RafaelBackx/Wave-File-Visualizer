#ifndef ENDIANNESS_H
#define ENDIANNESS_H
#include <cstdint>

namespace io
{
	void switch_endianness(uint16_t* value);
	void switch_endianness(uint32_t* value);
	void switch_endianness(uint64_t* value);
}
#endif // !ENDIANNESS_H
