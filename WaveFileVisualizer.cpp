// WaveFileVisualizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include "wave/wave.h"

int main()
{
	wave::WaveReader waveReader;
	//waveReader.read("D:\\Andere\\c++\\wave_files\\music.wav");
	//waveReader.read("D:\\Andere\\c++\\wave_files\\stress.wav");
	waveReader.read("D:\\Andere\\c++\\wave_files\\file_example_WAV_10MG.wav");

	//std::ifstream file("D:\\Andere\\c++\\wave_files\\music.wav", std::ios::binary);
	//std::ifstream file("D:\\Andere\\c++\\wave_files\\stress.wav");
	//std::ifstream file("D:\\Andere\\c++\\wave_files\\file_example_WAV_10MG.wav", std::ios::binary);
	//wave::RIFFCHUNK riff;
	//wave::read_RIFFCHUNK(file, riff);
	//wave::FMTCHUNK fmt;
	//wave::read_FMTCHUNK(file, fmt);
	//wave::DATACHUNK data_chunk;
	//auto data = wave::read_DATACHUNK(file, data_chunk,fmt);
	//std::cout << "size unique Pointer: " << sizeof(data) << std::endl;
	//for (int i = 0 ;i<10; i++) // forward loop
	//{
	//	int16_t sample = (data[wave::Position(i, 0)] | (data[wave::Position(i, 1)] << 8));
	//	std::cout << std::hex << +sample << std::endl;
	//}
	//for (int i = data.width -1 ;i>0; i--) // backwards loop
	//{
	//	for (int j=data.length-1; j>=0;j--)
	//	{
	//		std::cout << "data - ( " << i  << ", " << j << ")" << std::hex << +data[wave::Position(i, j)] << std::endl;
	//	}
	//}
	//std::vector<int32_t> reducedSamples = wave::reduceSamples(data, 100);
	//std::cout << std::hex << +data[wave::Position(data.width-1, data.length)];
	//std::cout << std::hex << +data[wave::Position(data.width-1, data.length - 1)] << std::endl;
	//std::cout << "data size = " << data.width  << std::endl;
	//std::cout << "reduced samples = " << reducedSamples.size() << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file