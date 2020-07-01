// WaveFileVisualizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include "wave/wave.h"
#include "imaging/bmp-format.h"
#include "util//position.h"
#include "imaging/visualization.h"
#include <cmath>
#pragma comment (lib,"shell32")
int main()
{
	std::string musicFile = "D:\\Andere\\c++\\wave_files\\Mr.BlueSky.wav";
	/*wave::WaveReader waveReader;
	waveReader.read(musicFile);
	std::cout << "number of channels: " << waveReader.fmt.numChannels << std::endl;
	std::cout << "sample rate: " << waveReader.fmt.sample_rate << std::endl;
	std::cout << "number of samples: " << waveReader.getSamples().size() << std::endl;
	std::cout << "sample width: " << waveReader.fmt.bitsPerSample << std::endl;
	std::cout << "encoding: " << waveReader.fmt.audioFormat << std::endl;
	sf::Music music;
	if (!music.openFromFile(musicFile))
	{
		std::cout << "Error when reading music from file! " << std::endl;
		return -1;
	}*/

	sfmlVisualization::Visualizer vis(musicFile, 1500, 800);
	std::cout << "setup completed" << std::endl;
	vis.showMenu();

	//sfmlVisualization::visualize(waveReader,&music);


	return 0;
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
