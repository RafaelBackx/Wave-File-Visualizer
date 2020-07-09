#include "..//io/endianness.h"
#include "wave.h"
#include <iostream>
#include <fstream>


std::string wave::WaveReader::getDataChunkID(wave::GENERALHEADER& data)
{
	std::string s(data.subChunkID, 4);
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
void wave::WaveReader::read_DATACHUNK(std::istream& input, wave::GENERALHEADER& data, wave::FMTCHUNK& fmt)
{
	input.read(reinterpret_cast<char*>(&data), sizeof(GENERALHEADER));
	while(getDataChunkID(data) != "data")
	{
		if(getDataChunkID(data) == "LIST")
		{
			read_LISTCHUNK(input, data);
		}
		else
		{
			auto list_data_array = std::make_unique<int8_t[]>(data.subChunkSize);
			input.read(reinterpret_cast<char*>(list_data_array.get()), sizeof(char) * data.subChunkSize);
		}
		input.read(reinterpret_cast<char*>(&data), sizeof(GENERALHEADER));
	}
	if (getDataChunkID(data) == "data")
	{
		//std::cout << this->metaData.size();
		readSamples(input); 
	}
	/*else if(getDataChunkID(data) == "LIST")
	{
		read_LISTCHUNK(input, data);
		wave::GENERALHEADER ACTUAL_DATA_HEADER;
		input.read(reinterpret_cast<char*>(&ACTUAL_DATA_HEADER), sizeof(wave::GENERALHEADER));
		data = ACTUAL_DATA_HEADER;
		if (getDataChunkID(ACTUAL_DATA_HEADER) == "data")
		{
			readSamples(input);
		}
		else
		{
			std::cout << "Malformed wave file: the data block is not where it is supposed to be" << std::endl;
		}
	}*/
 	std::cout << "data bytes: " << data.subChunkSize << std::endl;
}

void wave::WaveReader::read_LISTCHUNK(std::istream& input, wave::GENERALHEADER& generalheader)
{
	//auto list_data_array = std::make_unique<char[]>(generalheader.subChunkSize);
	//input.read(reinterpret_cast<char*>(list_data_array.get()), sizeof(char) * generalheader.subChunkSize);
	char info[4];
	input.read(info, sizeof(char) * 4);
	std::string infoString(info, 4);
	if (infoString == "INFO")
	{
		generalheader.subChunkSize -= sizeof(char) * 4; // remove the size of the "INFO"
		//generalheader.subChunkSize -= sizeof(uint32_t); // remove the size of the bytes that hold the size
		while (generalheader.subChunkSize)
		{
			int nullterminationChar = 0;
			ListField listfield;
			// Read the name of the listfield
			input.read(reinterpret_cast<char*>(&listfield.id), 4 * sizeof(char));
			// Read the size of this listfield
			input.read(reinterpret_cast<char*>(&listfield.listFieldSize), sizeof(uint32_t));
			// Read the data of this listfield
			int size = sizeof(uint32_t) + (sizeof(char) * 4) + (listfield.listFieldSize);
			//if (!(generalheader.subChunkSize - size)) nullterminationChar = 0;
			//listfield.listFieldValue = std::make_shared<char[]>(listfield.listFieldSize);
			listfield.listFieldValue = new char[listfield.listFieldSize];
			input.read(listfield.listFieldValue, sizeof(char) * (listfield.listFieldSize));
			char data = input.get();
			int offset = input.tellg();
			while(data == '\0')
			{
				data = input.get();
				offset = input.tellg();
				--generalheader.subChunkSize;
			}
			input.seekg(offset-1, input.beg);
			// Add this listfield to list of listfields in wavereader
			this->list.listFields.push_back(listfield);
			// remove the size of this listfield from the data.subchunk2size so the loop will end
			generalheader.subChunkSize -= (listfield.listFieldSize); // remove the size of the actual list data
			generalheader.subChunkSize -= sizeof(uint32_t); // remove the size of the bytes that hold the size data
			generalheader.subChunkSize -= sizeof(char) * 4; // remove the size of the bytes that hold the id of the listfield
			std::cout << generalheader.subChunkSize << std::endl;
		}
	}
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
	// To prevent memory leaks we first need to clear meta data stored in dumb pointers from a possible previous song
	if (this->list.listFields.size()>0)
	{
		for (ListField field : this->list.listFields)
		{
			delete[] field.listFieldValue;
		}
		this->list.listFields.clear();
	}
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		std::cout << "wrong filename" << std::endl;
		return;
	}
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
	int width = this->data_chunk.subChunkSize / (this->fmt.bitsPerSample / 8);
	std::vector<T> _samples(width);
	input.read(reinterpret_cast<char*>(&_samples[0]), this->data_chunk.subChunkSize);
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
}

void wave::WaveReader::cast(std::vector<uint8_t>& data)
{
	std::vector<int> sample_data(data.begin(),data.end());
	this->samples = sample_data;
}
void wave::WaveReader::cast(std::vector<int16_t>& data)
{
	std::vector<int> sample_data(data.begin(), data.end());
	this->samples = sample_data;
}
void wave::WaveReader::cast(std::vector<int32_t>& data)
{
	std::vector<int> sample_data(data.begin(), data.end());
	this->samples = sample_data;
}

wave::LISTCHUNK::LISTCHUNK()
{
	this->listIds.insert(std::pair<std::string, std::string>("IARL", "Archival location"));
	this->listIds.insert(std::pair<std::string, std::string>("IART", "Artist"));
	this->listIds.insert(std::pair<std::string, std::string>("ICMS", "Commissioned by"));
	this->listIds.insert(std::pair<std::string, std::string>("ICMT", "Comments"));
	this->listIds.insert(std::pair<std::string, std::string>("ICOP", "Copyright"));
	this->listIds.insert(std::pair<std::string, std::string>("ICRD", "Creation date"));
	this->listIds.insert(std::pair<std::string, std::string>("ICRP", "Cropped"));
	this->listIds.insert(std::pair<std::string, std::string>("IDIM", "Dimensions"));
	this->listIds.insert(std::pair<std::string, std::string>("IDPI", "Dots per inch"));
	this->listIds.insert(std::pair<std::string, std::string>("IENG", "Engineer"));
	this->listIds.insert(std::pair<std::string, std::string>("IGNR", "Genre"));
	this->listIds.insert(std::pair<std::string, std::string>("IKEY", "Keywords"));
	this->listIds.insert(std::pair<std::string, std::string>("ILGT", "Lightness"));
	this->listIds.insert(std::pair<std::string, std::string>("IMED", "Medium"));
	this->listIds.insert(std::pair<std::string, std::string>("INAM", "Name"));
	this->listIds.insert(std::pair<std::string, std::string>("IPLT", "Palette"));
	this->listIds.insert(std::pair<std::string, std::string>("IPRD", "Product"));
	this->listIds.insert(std::pair<std::string, std::string>("ISBJ", "Subject"));
	this->listIds.insert(std::pair<std::string, std::string>("ISFT", "Software"));
	this->listIds.insert(std::pair<std::string, std::string>("ISHP", "Sharpness"));
	this->listIds.insert(std::pair<std::string, std::string>("ISRC", "Source Form"));
	this->listIds.insert(std::pair<std::string, std::string>("ITCH", "Technician"));
}