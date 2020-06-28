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
}

void wave::WaveReader::read_FMTCHUNK(std::istream& input, wave::FMTCHUNK& fmt)
{
	input.read(reinterpret_cast<char*>(&fmt), sizeof(wave::FMTCHUNK));
}
void wave::WaveReader::read_DATACHUNK(std::istream& input, wave::DATACHUNK& data, wave::FMTCHUNK& fmt)
{
	input.read(reinterpret_cast<char*>(&data), sizeof(DATACHUNK));
	if (getDataChunkID(data) == "data")
	{
		readSamples(input);
	}
	else if(getDataChunkID(data) == "LIST")
	{
		auto list_data_array = std::make_unique<int8_t[]>(data.subChunk2Size);
		input.read(reinterpret_cast<char*>(list_data_array.get()), data.subChunk2Size);
		wave::DATACHUNK ACTUAL_DATA_HEADER;
		input.read(reinterpret_cast<char*>(&ACTUAL_DATA_HEADER), sizeof(wave::DATACHUNK));
		data = ACTUAL_DATA_HEADER;
		readSamples(input);
	}
 	std::cout << data.subChunk2Size << std::endl;
}

std::vector<int> wave::WaveReader::reduceSamples(int factor)
{
	uint8_t counter = 0;
	int32_t data = 0;
	int32_t sampleCounter = 0;
	std::vector<int> result;
	int length = this->getSamples().size();
	std::cout << length/factor << std::endl;
	result.reserve(length / factor);
	std::cout << result.size() << std::endl;
	for (int x=0;x < length;x++)
	{
		if (counter < factor)
		{
			data += this->getSamples()[x];
		}
		else
		{
			data /= factor;
			result.push_back(data);
			data = this->getSamples()[x];
			counter = 0;
		}
		++counter;
	}
	return result;
}

void wave::WaveReader::read(std::string filename)
{
	std::ifstream file(filename, std::ios::binary);
	//read riff chunk
	read_RIFFCHUNK(file, this->riff);
	//read fmt chunk
	read_FMTCHUNK(file, this->fmt);
	//read data chunk, fills data object
	read_DATACHUNK(file, this->data_chunk, fmt);
	//this->readSamples(file);
}

void wave::WaveReader::readSamples(std::istream& input)
{
	//get Type
	switch (fmt.bitsPerSample)
	{
	case 8:
		readSamples <uint8_t> (input);
		break;
	case 16:
		readSamples<int16_t>(input);
		break;
	case 24:
		readSamples<int32_t>(input);
		break;
	case 32:
		readSamples<int32_t>(input);
		break;
	default:
		abort(); // need to change this probably
	}
}

template<typename T>
void wave::WaveReader::readSamples(std::istream& input)
{
	int width = this->data_chunk.subChunk2Size / (this->fmt.bitsPerSample / 8);
	std::vector<T> _samples(width);
	input.read(reinterpret_cast<char*>(&_samples[0]), this->data_chunk.subChunk2Size);
	this->variant = _samples;
	switch (this->variant.index())
	{
	case 0:
		this->cast(std::get<std::vector<uint8_t>>(this->variant));
		break;
	case 1:
		this->cast(std::get<std::vector<int16_t>>(this->variant));
		break;
	case 2:
		this->cast(std::get<std::vector<int32_t>>(this->variant));
		break;
	default:
		break;
	}
	//for (int i=0;i<10;i++)
	//{
	//	std::cout << this->samples[i] << std::endl;
	//}
	//std::cout << "hello" << std::endl;
}

void wave::WaveReader::cast(std::vector<uint8_t> data)
{
	std::vector<int> sample_data(data.begin(),data.end());
	this->samples = sample_data;
}
void wave::WaveReader::cast(std::vector<int16_t> data)
{
	std::vector<int> sample_data(data.begin(), data.end());
	this->samples = sample_data;
}
void wave::WaveReader::cast(std::vector<int32_t> data)
{
	std::vector<int> sample_data(data.begin(), data.end());
	this->samples = sample_data;
}