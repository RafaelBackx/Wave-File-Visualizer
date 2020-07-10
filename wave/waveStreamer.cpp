#include "wave.h"

void wave::WaveStreamer::read_RIFFCHUNK(std::istream& input, RIFFCHUNK& riff)
{
	input.read(reinterpret_cast<char*>(&riff), sizeof(RIFFCHUNK));
}

void wave::WaveStreamer::read_FMTCHUNK(std::istream& input, wave::FMTCHUNK& fmt)
{
	input.read(reinterpret_cast<char*>(&fmt), sizeof(wave::FMTCHUNK));
}

void wave::WaveStreamer::read_LISTCHUNK(std::istream& input, wave::GENERALHEADER& generalheader)
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
			while (data == '\0')
			{
				data = input.get();
				offset = input.tellg();
				--generalheader.subChunkSize;
			}
			input.seekg(offset - 1, input.beg);
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

void wave::WaveStreamer::getDataOffset(std::istream& input)
{
	wave::GENERALHEADER subChunk;
	input.read(reinterpret_cast<char*>(&subChunk), sizeof(GENERALHEADER));
	while(getGeneralHeaderId(subChunk) != "data")
	{
		if (getGeneralHeaderId(subChunk) == "LIST")
		{
			read_LISTCHUNK(input, subChunk);
		}
		else
		{
			// read data and ignore it
			auto list_data_array = std::make_unique<int8_t[]>(subChunk.subChunkSize);
			input.read(reinterpret_cast<char*>(list_data_array.get()), sizeof(char) * subChunk.subChunkSize);
		}
		input.read(reinterpret_cast<char*>(&subChunk), sizeof(GENERALHEADER));
	}
	if (getGeneralHeaderId(subChunk) == "data")
	{
		this->dataOffset = input.tellg();
		//std::cout << "Data offset in dec:" << this->dataOffset << " data offset int hex: " << std::hex << this->dataOffset << std::endl;
	}
}

template<typename T>
void wave::WaveStreamer::updateBuffer(unsigned int offset)
{
	if (offset%sizeof(T) != 0) // offset is not in sync with sample size
	{
		double divide = ceil((double)offset / (double)sizeof(T));
		offset = divide * sizeof(T);
	}
	this->input.seekg(this->dataOffset + offset, this->input.beg);
	std::vector<T> bufferType(this->bufferSize);
	input.read(reinterpret_cast<char*>(&bufferType[0]), static_cast<long int>(this->bufferSize)*sizeof(T));
	std::vector<int> buffer(bufferType.begin(), bufferType.end());
	this->buffer = buffer;
}

void wave::WaveStreamer::updateBuffer(unsigned int offset)
{
	switch (this->fmt.bitsPerSample)
	{
	case 8:
		updateBuffer<uint8_t>(offset);
		break;
	case 16:
		updateBuffer<int16_t>(offset);
		break;
	case 24:
		updateBuffer<int32_t>(offset);
		break;
	case 32:
		updateBuffer<int32_t>(offset);
		break;
	default:
		break;
	}
}

void wave::WaveStreamer::setFile(std::string filename)
{
	// To prevent memory leaks we first need to clear meta data stored in dumb pointers from a possible previous song
	if (this->list.listFields.size() > 0)
	{
		for (ListField field : this->list.listFields)
		{
			delete[] field.listFieldValue;
		}
		this->list.listFields.clear();
	}
	this->input = std::ifstream(filename, std::ios::binary);
	read_RIFFCHUNK(input, riff);
	read_FMTCHUNK(input, fmt);
	getDataOffset(input);
}