#ifndef WAVE_H
#define WAVE_H
#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include "..//util/position.h"
#include <variant>
#include <map>
#include <fstream>

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
	struct GENERALHEADER
	{
		char subChunkID[4]; // should be equal to data; BIG ENDIAN
		uint32_t subChunkSize; // this is the number of bytes in the data, you can also think of this als the size of the read of the subchunk following this number; LITTLE ENDIAN
	};
	struct ListField
	{
		char id[4]; // Holds the id of the list field e.g INAM,IART,IGNR,...
		uint32_t listFieldSize;
		char* listFieldValue;
		//std::shared_ptr<char[]> listFieldValue;
		std::string getId() { return std::string(id, 4); }
		std::string getValue() { return std::string(listFieldValue, listFieldSize-1); } // -1 because the last character in the array is an null terminating character
	};
	struct LISTCHUNK
	{
		std::vector<ListField> listFields;
		std::map<std::string, std::string> listIds;
		LISTCHUNK();
	};
	
	class WaveReader
	{
	private:
		std::variant <std::vector<uint8_t>, std::vector<int16_t>, std::vector<int32_t>> variant;
		std::vector<int> samples;
	public:
		RIFFCHUNK riff;
		FMTCHUNK fmt;
		GENERALHEADER data_chunk;
		LISTCHUNK list;
		void read(std::string filepath);
		std::vector<int>& getSamples() { return this->samples; }
		std::vector<int> reduceSamples(int factor = 100);
		std::vector<ListField>& getMetaData() { return this->list.listFields; }
		//std::vector<uint32_t> getSamplesOfChannel(int channel_no); //TODO implement
	private:
		void read_RIFFCHUNK(std::istream& input, RIFFCHUNK& riff);
		void read_FMTCHUNK(std::istream& input, FMTCHUNK& fmt);
		void read_DATACHUNK(std::istream& input, GENERALHEADER& data, FMTCHUNK& fmt);
		void read_LISTCHUNK(std::istream& input, wave::GENERALHEADER& generalheader);
		std::string getDataChunkID(GENERALHEADER& data);
		void readSamples(std::istream& input);
		template<typename T>
		void readSamples(std::istream& input);
		void cast(std::vector<uint8_t>& data);
		void cast(std::vector<int16_t>& data);
		void cast(std::vector<int32_t>& data);
	};

	class WaveStreamer
	{
	private:
		std::vector<int> buffer;
		unsigned long int bufferSize;
		unsigned long int dataOffset;
	public:
		std::ifstream input;
		RIFFCHUNK riff;
		FMTCHUNK fmt;
		LISTCHUNK list;
		WaveStreamer() : bufferSize(1024), dataOffset(0), buffer(0) {}
		WaveStreamer(std::string filename) : bufferSize(1024), dataOffset(0), buffer(0), input(filename,std::ios::binary)
		{
			read_RIFFCHUNK(input, riff);
			read_FMTCHUNK(input, fmt);
			getDataOffset(input);
		}
		std::vector<int>& getBuffer() { return buffer; }
		void setBufferSize(unsigned int bufferSize) { this->bufferSize = bufferSize; }
		void updateBuffer(unsigned int offset);
		void setFile(std::string filename);
		std::vector<ListField>& getMetaData() { return this->list.listFields; }
		unsigned long int getBufferSize() { return this->bufferSize; }
	private:
		void read_RIFFCHUNK(std::istream& input, RIFFCHUNK& riff);
		void read_FMTCHUNK(std::istream& input, FMTCHUNK& fmt);
		void read_LISTCHUNK(std::istream& input, wave::GENERALHEADER& generalheader);
		void getDataOffset(std::istream& input);
		std::string getGeneralHeaderId(wave::GENERALHEADER& header)
		{
			return std::string(header.subChunkID, 4);
		}
		template<typename T>
		void updateBuffer(unsigned int offset);
	};
}

#endif // !WAVE_H

