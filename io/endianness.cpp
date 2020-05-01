#include <iostream>
#include "endianness.h"

void io::switch_endianness(uint16_t* n)
{
	uint16_t first_byte, second_byte;
	uint16_t result;
	first_byte = ((*n) & 0x00ff);
	second_byte = ((*n) >> 8 & 0x00ff);
	result = (first_byte) << 8 | second_byte;
	(*n) = result;
}

void io::switch_endianness(uint32_t* n)
{
	uint16_t first_word, second_word;
	first_word = ((*n) & 0x0000FFFF);
	second_word = ((*n) >> 16 & 0x0000FFFF);
	io::switch_endianness(&first_word);
	io::switch_endianness(&second_word);
	(*n) = (static_cast<uint32_t>(first_word) << 16) | second_word;
}

void io::switch_endianness(uint64_t* n)
{
	uint32_t first_dword, second_dword;
	first_dword = ((*n) & 0x00000000FFFFFFFF);
	second_dword = ((*n) >> 32 & 0x00000000FFFFFFFF);
	io::switch_endianness(&first_dword);
	io::switch_endianness(&second_dword);
	(*n) = (static_cast<uint64_t>(first_dword) << 32) | second_dword;
}