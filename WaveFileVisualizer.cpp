// WaveFileVisualizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include "wave/wave.h"
#include "util//position.h"
#include "imaging/visualization.h"
#include <cmath>
int main()
{
	/// TODO 
	/// Implement meta data with streamer
	/// size the rectangles over the whole screen

	//std::string musicFile = "D:\\Andere\\c++\\wave_files\\Mr.BlueSky.wav";
	//wave::WaveStreamer streamer(musicFile);

	sfmlVisualization::Visualizer vis(300, 500);
	vis.showMenu();
	return 0;
}
