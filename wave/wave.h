#ifndef WAVE_H
#define WAVE_H
#include <cstdint>
#include <istream>
#include <memory>
#include <vector>
#include <string>
#include "..//util/position.h"
#include <variant>

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
	
	class WaveReader
	{
	private:
		std::variant <std::vector<uint8_t>, std::vector<int16_t>, std::vector<int32_t>> variant;
		std::vector<int> samples;
	public:
		RIFFCHUNK riff;
		FMTCHUNK fmt;
		DATACHUNK data_chunk;
		void read(std::string filepath);
		std::vector<int>& getSamples() { return this->samples; }
		std::vector<int> reduceSamples(int factor = 100); //TODO implement
		//std::vector<uint32_t> getSamplesOfChannel(int channel_no); //TODO implement
	private:

		void readSamples(std::istream& input);
		template<typename T>
		void readSamples(std::istream& input);
		std::string getDataChunkID(DATACHUNK& data);
		void read_RIFFCHUNK(std::istream& input, RIFFCHUNK& riff);
		void read_FMTCHUNK(std::istream& input, FMTCHUNK& fmt);
		void read_DATACHUNK(std::istream& input, DATACHUNK& data, FMTCHUNK& fmt);
		void cast(std::vector<uint8_t> data);
		void cast(std::vector<int16_t> data);
		void cast(std::vector<int32_t> data);
	};
}

#endif // !WAVE_H

