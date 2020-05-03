#ifndef WAVE_H
#define WAVE_H
#include <cstdint>
#include <istream>
#include <memory>
#include <vector>
#include <string>
#include "..//util/position.h"
#include "..//util/array.h"
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
	//raw data in bytes not in samples yet
	/*struct DATA
	{
		std::unique_ptr<int8_t[]>data;
		uint32_t length;
		uint32_t width;
		int8_t operator [](const Position& pos) { return data[(pos.x * length) + pos.y]; }
	};*/
	//std::string getDataChunkID(DATACHUNK& data);
	//void read_RIFFCHUNK(std::istream& input, RIFFCHUNK& riff);
	//void read_FMTCHUNK(std::istream& input, FMTCHUNK& fmt);
	//DATA read_DATACHUNK(std::istream& input, DATACHUNK& data, FMTCHUNK& fmt);
	//std::vector<int32_t> reduceSamples(DATA& data, int reduceBy);
	class WaveReader
	{
	private:
		RIFFCHUNK riff;
		FMTCHUNK fmt;
		DATACHUNK data_chunk;
		Array<int8_t> data;
		//typedef uint8_t bytes;
	public:
		void read(std::string filepath);
		//std::vector<uint32_t> reduceSamples(int reduceBy); TODO implement
		//std::vector<uint32_t> getSamplesOfChannel(int channel_no); TODO implement
	private:
		//void readSamples(); TODO implement
		std::string getDataChunkID(DATACHUNK& data);
		void read_RIFFCHUNK(std::istream& input, RIFFCHUNK& riff);
		void read_FMTCHUNK(std::istream& input, FMTCHUNK& fmt);
		Array<int8_t> read_DATACHUNK(std::istream& input, DATACHUNK& data, FMTCHUNK& fmt);
	};
}

#endif // !WAVE_H

