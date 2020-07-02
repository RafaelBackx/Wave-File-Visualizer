#ifndef VISUALIZATION_H
#define VISUALIZATION_H
#include "bmp-format.h"
#include "..//wave/wave.h"
#include "gui.h"
#include <SFML/Audio/Music.hpp>
namespace visualization 
{
	void drawRect(const Position& start, const Position& end, imaging::Bitmap& image, const imaging::Color color);
	void visualize(wave::WaveReader& wavereader);
	void splice(imaging::Bitmap& img, unsigned frameWidth, unsigned space);
}


// SFML VISUALIZATION

namespace sfmlVisualization
{
	class Visualizer
	{
	private:
		wave::WaveReader wavereader;
		sf::RenderWindow window;
		sf::Music music;
		std::string filename;
		gui::Screen screen;
	public:
		Visualizer(std::string filename, int windowWidth, int windowHeight) :screen(), filename(filename), window(), wavereader(), music() 
		{
			wavereader.read(filename);
			if (!music.openFromFile(filename))
			{
				std::cout << "Cannot open music file! " << std::endl;
			}
			window.create(sf::VideoMode(windowWidth, windowHeight), "Visualizer", sf::Style::Titlebar | sf::Style::Close);
		}
		void showMenu();
		void visualize();
	};
	//void chooseFile(); // TODO implement, Look for api with windows file explorer functionality
	void visualize(wave::WaveReader& wavereader, sf::Music* music);
}
#endif