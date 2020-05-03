#include "..//io/endianness.h"
#include "wave.h"
#include <iostream>
#include <fstream>


std::string wave::WaveReader::getDataChunkID(wave::DATACHUNK& data)
{
	std::string s(data.subChunk2ID, 4);
	return s;
}

void wave::WaveReader::read_RIFFCHUNK(std::istream& input, RIFFCHUNK& riff)
{
	input.read(reinterpret_cast<char*>(&riff), sizeof(RIFFCHUNK));
	//io::switch_endianness(&riff.chunksize);
}

void wave::WaveReader::read_FMTCHUNK(std::istream& input, wave::FMTCHUNK& fmt)
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
Array<int8_t> wave::WaveReader::read_DATACHUNK(std::istream& input, wave::DATACHUNK& data, wave::FMTCHUNK& fmt)
{
	input.read(reinterpret_cast<char*>(&data), sizeof(DATACHUNK));
	if (getDataChunkID(data) == "data")
	{
		auto data_array = std::make_unique<int8_t[]>(data.subChunk2Size); // declare data as uint8_t data array
		input.read(reinterpret_cast<char*>(data_array.get()), data.subChunk2Size); 
		Array<int8_t> data_obj;
		data_obj.data = std::move(data_array);
		data_obj.length = (fmt.bitsPerSample / 8);
		data_obj.width = data.subChunk2Size / (fmt.bitsPerSample / 8);
		return data_obj;
	}
	else if(getDataChunkID(data) == "LIST")
	{
		auto list_data_array = std::make_unique<int8_t[]>(data.subChunk2Size);
		input.read(reinterpret_cast<char*>(list_data_array.get()), data.subChunk2Size);
		wave::DATACHUNK ACTUAL_DATA_HEADER;
		input.read(reinterpret_cast<char*>(&ACTUAL_DATA_HEADER), sizeof(wave::DATACHUNK));
		auto ACTUAL_DATA = std::make_unique<int8_t[]>(ACTUAL_DATA_HEADER.subChunk2Size);
		std::cout << "list data" << data.subChunk2Size << "actual data" << ACTUAL_DATA_HEADER.subChunk2Size;
		data = ACTUAL_DATA_HEADER;
		input.read(reinterpret_cast<char*>(ACTUAL_DATA.get()), data.subChunk2Size);
		Array<int8_t> data_obj;
		data_obj.data = std::move(ACTUAL_DATA);
		data_obj.length = (fmt.bitsPerSample / 8);
		data_obj.width = data.subChunk2Size / (fmt.bitsPerSample / 8);
		return data_obj;
	}
 	std::cout << data.subChunk2Size << std::endl;
}

//std::vector<int32_t> wave::reduceSamples(wave::DATA& data_block, int reduceBy)
//{
//	uint8_t counter = 0;
//	int32_t data = 0;
//	int32_t sampleCounter = 0;
//	std::vector<int32_t> result;
//	result.reserve((data_block.width / data_block.length) / reduceBy);
//	for (int x=0;x < data_block.width;x++)
//	{
//		for (int y=0;y < data_block.length;y++)
//		{
//			if (counter < reduceBy)
//			{
//				data += data_block[Position(x, y)];
//			}
//			else
//			{
//				data /= reduceBy;
//				result.emplace_back(data);
//				data = data_block[Position(x,y)];
//				counter = 0;
//			}
//		}
//		++counter;
//	}
//	return result;
//}

void wave::WaveReader::read(std::string filename)
{
	std::ifstream file(filename, std::ios::binary);
	//read riff chunk
	RIFFCHUNK riff;
	read_RIFFCHUNK(file, riff);
	//read fmt chunk
	FMTCHUNK fmt;
	read_FMTCHUNK(file, fmt);
	//read data chunk, fills data object
	DATACHUNK data_chunk;
	Array<int8_t> data = read_DATACHUNK(file,data_chunk,fmt);
	std::cout << sizeof(Array<int8_t>) << std::endl;


	//// debugging purposes

	for (int i = 0 ;i<10; i++) // forward loop
	{
		int16_t sample = (data[Position(i, 0)] | (data[Position(i, 1)] << 8));
		std::cout << std::hex << +sample << std::endl;
	}
	//for (int i = data.width - 1; i > 0; i--) // backwards loop
	//{
	//	for (int j = data.length - 1; j >= 0; j--)
	//	{
	//		std::cout << "data - ( " << i << ", " << j << ")" << std::hex << +data[wave::Position(i, j)] << std::endl;
	//	}
	//}
}
