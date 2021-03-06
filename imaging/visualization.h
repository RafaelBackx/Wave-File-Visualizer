#ifndef VISUALIZATION_H
#define VISUALIZATION_H
#include "..//wave/wave.h"
#include "gui.h"
#include <SFML/Audio/Music.hpp>


// SFML VISUALIZATION

namespace sfmlVisualization
{
	class Visualizer
	{
	private:
		wave::WaveReader wavereader;
		wave::WaveStreamer streamer;
		sf::RenderWindow window;
		sf::Music music;
		std::string filename;
		gui::Screen screen;
	public:
		Visualizer(int windowWidth, int windowHeight) :screen(), window(), wavereader(), music() 
		{
			window.resetGLStates();
			window.create(sf::VideoMode(windowWidth, windowHeight), "Visualizer", sf::Style::Titlebar | sf::Style::Close);
		}
		void showMenu();
		void visualize(std::string filename);
	};
}
#endif