#include "..//io/endianness.h"
#include "wave.h"
#include <iostream>
#include <iterator>

std::string wave::getDataChunkID(wave::DATACHUNK& data)
{
	std::string s(data.subChunk2ID, 4);
	return s;
}

void wave::read_RIFFCHUNK(std::istream& input, RIFFCHUNK& riff)
{
	input.read(reinterpret_cast<char*>(&riff), sizeof(RIFFCHUNK));
	//io::switch_endianness(&riff.chunksize);
}

void wave::read_FMTCHUNK(std::istream& input, wave::FMTCHUNK& fmt)
{
	input.read(reinterpret_cast<char*>(&fmt), sizeof(wave::FMTCHUNK));
	//io::switch_endianness(&fmt.subChunk1Size);
	//io::switch_endianness(&fmt.audioFormat);
	//io::switch_endianness(&fmt.numChannels);
	//io::switch_endianness(&fmt.sample_rate);
	//io::switch_endianness(&fmt.byte_rate);
	//io::switch_endianness(&fmt.block_align);
	//io::switch_endianness(&fmt.bitsPerSample);
}
wave::DATA wave::read_DATACHUNK(std::istream& input, wave::DATACHUNK& data, wave::FMTCHUNK& fmt)
{
	input.read(reinterpret_cast<char*>(&data), sizeof(DATACHUNK));
	if (wave::getDataChunkID(data) == "data")
	{
		/*std::vector<uint8_t> bytes(data.subChunk2Size);
		input.read(reinterpret_cast<char*>(&bytes[0]), data.subChunk2Size);
		std::cout << bytes.size() << std::endl;*/
		auto data_array = std::make_unique<uint8_t[]>(data.subChunk2Size); // declare data as uint8_t data array
		input.read(reinterpret_cast<char*>(data_array.get()), data.subChunk2Size); 
		std::cout << data.subChunk2Size << std::endl;
		wave::DATA data_obj;
		data_obj.data = std::move(data_array);
		data_obj.length = (fmt.bitsPerSample / 8);
		data_obj.width = data.subChunk2Size / (fmt.bitsPerSample / 8);
		return data_obj;
	}
	else if(wave::getDataChunkID(data) == "LIST")
	{
		auto list_data_array = std::make_unique<uint8_t[]>(data.subChunk2Size);
		input.read(reinterpret_cast<char*>(list_data_array.get()), data.subChunk2Size);
		wave::DATACHUNK ACTUAL_DATA_HEADER;
		input.read(reinterpret_cast<char*>(&ACTUAL_DATA_HEADER), sizeof(wave::DATACHUNK));
		auto ACTUAL_DATA = std::make_unique<uint8_t[]>(ACTUAL_DATA_HEADER.subChunk2Size);
		std::cout << "list data" << data.subChunk2Size << "actual data" << ACTUAL_DATA_HEADER.subChunk2Size;
		data = ACTUAL_DATA_HEADER;
		input.read(reinterpret_cast<char*>(ACTUAL_DATA.get()), data.subChunk2Size);
		DATA data_obj;
		data_obj.data = std::move(ACTUAL_DATA);
		data_obj.length = (fmt.bitsPerSample / 8);
		data_obj.width = data.subChunk2Size / (fmt.bitsPerSample / 8);
		return data_obj;
	}
 	std::cout << data.subChunk2Size << std::endl;
}

std::vector<uint32_t> wave::reduceSamples(wave::DATA& data_block, int reduceBy)
{
	uint8_t counter = 0;
	uint32_t data = 0;
	uint32_t sampleCounter = 0;
	std::vector<uint32_t> result;
	result.reserve((data_block.width / data_block.length) / reduceBy);
	for (int x=0;x<data_block.width;x++)
	{
		for (int y=0;y<data_block.length;y++)
		{
			if (counter < reduceBy)
			{
				data += data_block[Position(x, y)];
			}
			else
			{
				data /= reduceBy;
				result.emplace_back(data);
				data = data_block[Position(x,y)];
				counter = 0;
			}
		}
		++counter;
	}
	return result;
}
