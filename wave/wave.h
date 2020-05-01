#ifndef WAVE_H
#define WAVE_H
#include <cstdint>
#include <istream>
#include <memory>
#include <vector>
#include <string>
namespace wave
{
	struct RIFFCHUNK
	{
		char chunkID[4]; // should be equal to RIFF; BIG ENDIAN
		uint32_t chunksize; // size of the overall file;LITTLE ENDIAN
		char format[4]; // should be equal to WAVE; BIG ENDIAN
	};
	struct FMTCHUNK
	{
		char subChunk1ID[4]; // should be equal to fmt; BIG ENDIAN
		uint32_t subChunk1Size; // the size of this chunk after this number; LITTLE ENDIAN
		uint16_t audioFormat; // PCM (look this up) values other than 1 indicate compression
		uint16_t numChannels; // ;LITTLE ENDIAN
		uint32_t sample_rate; // ;LITTLE ENDIAN
		uint32_t byte_rate; // ==sample_rate * numChannels * bitsPerSample /8; LITTLE ENDIAN
		uint16_t block_align; // the number of bytes for one sample including all channels. // LITTLE ENDIAN
		uint16_t bitsPerSample; // LITTLE ENDIAN
	};
	struct DATACHUNK
	{
		char subChunk2ID[4]; // should be equal to data; BIG ENDIAN
		uint32_t subChunk2Size; // this is the number of bytes in the data, you can also think of this als the size of the read of the subchunk following this number; LITTLE ENDIAN
	};
	struct Position
	{
		int x, y;
		Position(int x, int y) : x(x), y(y) {}
	};
	struct DATA
	{
		std::unique_ptr<uint8_t[]>data;
		uint32_t length;
		uint32_t width;
		uint8_t operator [](const Position& pos) { return data[(pos.x * length) + pos.y]; }
	};
	std::string getDataChunkID(DATACHUNK& data);
	void read_RIFFCHUNK(std::istream& input, RIFFCHUNK& riff);
	void read_FMTCHUNK(std::istream& input, FMTCHUNK& fmt);
	DATA read_DATACHUNK(std::istream& input, DATACHUNK& data, FMTCHUNK& fmt);
	std::vector<uint32_t> reduceSamples(DATA& data, int reduceBy);
}

#endif // !WAVE_H

