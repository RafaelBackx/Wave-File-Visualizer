// WaveFileVisualizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include "wave/wave.h"
#include <SFML/Graphics.hpp>
#include "imaging/bmp-format.h"
#include "util//position.h"

void drawRect(const Position& start, const Position& end, imaging::Bitmap& image, const imaging::Color color) 
{
	int width = end.x - start.x;
	int height = end.y - start.y;
	for (int i =start.x;i<=start.x+width;i++)
	{
		//std::cout << "drawing...x" << std::endl;
		for (int j = start.y; j<start.y + height; j++)
		{
			if (image.is_inside(Position(i, j))) 
			{
				//std::cout << "drawing..." << std::endl;
				image[Position(i, j)] = color;
			}
			else {
				std::cout << "oopsie"<< std::endl;
			}
		}
	}
}

int main()
{
	//waveReader.read("D:\\Andere\\c++\\wave_files\\stress.wav");
	//waveReader.read("D:\\Andere\\c++\\wave_files\\file_example_WAV_10MG.wav");
	//waveReader.read("D:\\Andere\\c++\\wave_files\\love_wreck.wav");
	wave::WaveReader waveReader;
	waveReader.read("D:\\Andere\\c++\\wave_files\\love_wreck.wav");
	std::cout << "number of channels: " << waveReader.fmt.numChannels << std::endl;
	std::cout << "sample rate: " << waveReader.fmt.sample_rate << std::endl;
	std::cout << "number of samples: " << waveReader.getSamples().size() << std::endl;
	std::cout << "sample width: " << waveReader.fmt.bitsPerSample << std::endl;
	std::cout << "reducing... " << std::endl;
	std::vector<int> reducedSamples = waveReader.reduceSamples(100);
	std::cout << "number of reduced samples: " << reducedSamples.size() << std::endl;

	//Bitmap test
	int length = reducedSamples.size();
	std::cout << "creating bitmap..." << std::endl;
	imaging::Bitmap test(length+1, 201);
	std::cout << "drawing on bitmap... (this may take a while)" << std::endl;
	for (int i=0;i<length;i++)
	{
		Position start(i,200);
		if ((reducedSamples[i]) != 0) {
			Position end(i, 200 - ((double)(reducedSamples[i]) / 255) * 200);
			//std::cout << start << std::endl;
			//std::cout << end << std::endl;
			drawRect(end, start, test, imaging::Color(1, 1, 1));
		}
		//std::cout << +(reducedSamples[i]) << std::endl;
	}
	//imaging::Bitmap testImage(500, 500);
	//drawRect(Position(100, 100), Position(400, 400), test, imaging::Color(0.16,0.33,0.58));
	std::cout << "saving the image..." << std::endl;
	imaging::save_as_bmp("1mg.bmp", test);
	//SFML
	//sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	//sf::CircleShape shape(100.f);
	//shape.setFillColor(sf::Color::Green);

	//while (window.isOpen())
	//{
	//	sf::Event event;
	//	while (window.pollEvent(event))
	//	{
	//		if (event.type == sf::Event::Closed)
	//			window.close();
	//	}

	//	window.clear();
	//	window.draw(shape);
	//	window.display();
	//}

	//return 0;
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
