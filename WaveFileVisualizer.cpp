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

	//sf::RenderWindow window;
	//window.create(sf::VideoMode(500, 500), "Test");
	//gui::TextBox textbox(200, 50);
	//textbox.setBorderColor(sf::Color::Black);
	//textbox.setBorderWidth(2.0f);
	//textbox.setPosition(100, 100);
	//textbox.setColor(sf::Color::Color(150, 150, 150));
	//textbox.setPadding(10.0f);
	//textbox.setOnClick([&]()
	//	{
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
	//			if (event.key.code == 71)//Left arrow key
	//			{
	//				textbox.decrementTextOffset();
	//			}
	//			else if (event.key.code == 72)//Right arrow key
	//			{
	//				textbox.incrementTextOffset();
	//			}
	//			ctrlDown = event.key.control;
	//		}
	//		else if(event.type == sf::Event::KeyReleased)
	//		{
	//			ctrlDown = event.key.control;
	//		}
	//		else if (event.type == sf::Event::TextEntered)
	//		{
	//			textbox.handleEvent(event, window);
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
	return 0;
}
