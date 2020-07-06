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
	//std::string musicFile = "D:\\Andere\\c++\\wave_files\\bab.wav";
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

	//sf::RenderWindow window;
	//window.create(sf::VideoMode(500, 500), "Test");
	//gui::TextBox textbox(200, 50);
	//textbox.setBorderColor(sf::Color::Black);
	//textbox.setBorderWidth(2.0f);
	//textbox.setColor(sf::Color::Color(150, 150, 150));
	//textbox.setPadding(10.0f);
	//textbox.setOnClick([&]()
	//	{
	//		std::cout << "wajow gij bent gay" << std::endl;
	//	});
	//gui::Screen screen;
	//screen.addNode(&textbox);
	//sf::Clipboard clipboard;
	//bool ctrlDown = false;
	//while(window.isOpen())
	//{
	//	sf::Event event;
	//	while (window.pollEvent(event))
	//	{
	//		if (event.type == sf::Event::Closed)
	//		{
	//			window.close();
	//		}
	//		else if (event.type == sf::Event::KeyPressed)
	//		{
	//			ctrlDown = event.key.control;
	//		}
	//		else if(event.type == sf::Event::KeyReleased)
	//		{
	//			ctrlDown = event.key.control;
	//		}
	//		else if (event.type == sf::Event::TextEntered)
	//		{
	//			std::cout << event.text.unicode << std::endl;
	//			if (event.text.unicode < 128)
	//			{
	//				if (event.text.unicode == 8)
	//				{
	//					textbox.removeLastChar();
	//				}
	//				else if(event.text.unicode == 22 && ctrlDown) // check for control paste
	//				{
	//					std::cout << "pasting " << clipboard.getString().toAnsiString() << std::endl;
	//					textbox.setString(clipboard.getString().toAnsiString());
	//				}
	//				else if (event.text.unicode == 3 && ctrlDown) // check for control copy
	//				{
	//					std::cout << "copying" << textbox.getText() << std::endl;
	//					clipboard.setString(textbox.getText());
	//				}
	//				else
	//				{
	//					char t = static_cast<char>(event.text.unicode);
	//					textbox.addText(std::string(1, t));
	//				}
	//			}
	//		}
	//		else if(event.type == sf::Event::MouseButtonPressed)
	//		{
	//			sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);
	//			screen.updateNodesOnClick(mouse);
	//		}
	//	}
	//	window.clear(sf::Color::White);
	//	textbox.draw(window);
	//	window.display();
	//}

	sfmlVisualization::Visualizer vis(300, 500);
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
