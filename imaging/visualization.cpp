#include "visualization.h"
#include <SFML/Graphics.hpp>

void visualization::drawRect(const Position& start, const Position& end, imaging::Bitmap& image, const imaging::Color color)
{
	int width = end.x - start.x;
	int height = end.y - start.y;
	for (int i = start.x; i <= start.x + width; i++)
	{
		for (int j = start.y; j < start.y + height; j++)
		{
			if (image.is_inside(Position(i, j)))
			{
				image[Position(i, j)] = color;
			}
			else {
				std::cout << "oopsie" << std::endl;
			}
		}
	}
}

const int getMask(uint16_t bitsPerSample) 
{
	switch (bitsPerSample)
	{
	case 8:
		return 0xFF;
	case 16:
		return 0xFF00;
	case 24:
		return 0xFF00FF;
	case 32:
		return 0xFF00FF00;
	default:
		break;
	}
}

void visualization::visualize(wave::WaveReader& wavereader) 
{
	std::vector<int> reducedSamples = wavereader.reduceSamples();
	int length = reducedSamples.size();
	int height = 104;
	imaging::Bitmap test(length + 1, height);
	std::cout << "creating bitmap..." << std::endl;
	std::cout << "drawing on bitmap... (this may take a while)" << std::endl;
	const int channelMask = getMask(wavereader.fmt.bitsPerSample);
	for (int i = 0; i < length; i++)
	{
		Position start(i, height);
		if ((reducedSamples[i] & channelMask) != 0) {
			Position end(i, height - (abs(20 * log10(abs((double)(reducedSamples[i] & channelMask)) / (wavereader.fmt.bitsPerSample*8))))*2);
			visualization::drawRect(end, start, test, imaging::Color(1, 1, 1));
		}
		//std::cout << +(reducedSamples[i])/256 << std::endl;
		//std::cout << 20 * log10(abs((double)reducedSamples[i]) / 256.0) << std::endl;
	}
	std::cout << "saving the image..." << std::endl;
	imaging::save_as_bmp("abstraction-test-1.bmp", test);
	std::cout << "splicing..." << std::endl;
	visualization::splice(test, 1900, 30);
}

std::string paddWithLeadingZeros(int targetNumber, int length) 
{
	if (length == 1)
	{
		return std::to_string(targetNumber);
	}
	else
	{
		std::string result = "";
		while (--length)
		{
			result += "0";
		}
		result += std::to_string(targetNumber);
		return result;
	}
}

void visualization::splice(imaging::Bitmap& image,unsigned frameWidth, unsigned space)
{
	int frameCounter = 0;
	while (((frameCounter*space)+frameWidth)<=image.width())
	{
		std::string path = "D:\\Andere\\c++\\visualizations\\love_wreck\\love_wreck" + paddWithLeadingZeros(frameCounter, 5) + ".bmp";
		auto slice = image.slice(frameCounter * space, 0, frameWidth, image.height());
		imaging::save_as_bmp(path , *slice);
		frameCounter++;
	}
}

double transform(int number,const int channelMask)
{
	return abs(20 * log10(abs((double)(number & channelMask))));
}

void getMinMax(const std::vector<int> samples, double* min, double* max)
{
	for (int i=0;i<samples.size();i++)
	{
		if (samples[i]<*min)
		{
			*min = samples[i];
		}
		else if (samples[i]>*max)
		{
			*max = samples[i];
		}
	}
}

void createPlayPauseButtons(sf::RenderWindow* window, sf::RectangleShape& playButton, sf::RectangleShape& pauseButton)
{
	int spacingBetweenButtons = 10;
	int buttonsHeight = 70;
	int buttonsWidth = 70;
	// PLAYBUTTON
	sf::Texture playImage;
	if (!playImage.loadFromFile("sprites/play.png"))
	{
		std::cout << "cannot load play.png from file" << std::endl;
	}
	//playButton.setTexture(&playImage);
	playButton.setFillColor(sf::Color::Green);
	sf::Vector2f positionPlay((window->getSize().x / 2)-spacingBetweenButtons/2, window->getSize().y - 100);
	playButton.setSize(sf::Vector2f(buttonsWidth, buttonsHeight));
	playButton.setPosition(positionPlay);

	// PAUSEBUTTON	
	sf::Texture pauseImage;
	if (!pauseImage.loadFromFile("sprites/pause.png"))
	{
		std::cout << "cannot load pause.png from file" << std::endl;
	}
	//pauseButton.setTexture(&pauseImage);
	pauseButton.setFillColor(sf::Color::Red);
	sf::Vector2f positionPause(((window->getSize().x / 2) + spacingBetweenButtons / 2)+buttonsWidth, window->getSize().y - 100);
	pauseButton.setSize(sf::Vector2f(buttonsWidth, buttonsHeight));
	pauseButton.setPosition(positionPause);
}

void sfmlVisualization::visualize(wave::WaveReader& wavereader, sf::Music* music)
{
	std::vector<int> reducedSamples = wavereader.reduceSamples();
	const int channelMask = getMask(wavereader.fmt.bitsPerSample);
	double min = 0, max = 0;
	getMinMax(reducedSamples,&min,&max);
	sf::RenderWindow window;
	int windowWidth = 800, windowHeight = 800;
	int spacing = 5;
	int offset = 0;
	int length = reducedSamples.size();
	int frameRate = 60;
	bool paused = false;
	//window.setVerticalSyncEnabled(true); // Apparently you cannot use these functions with each other
	double song_length = ((double)wavereader.getSamples().size()/(double)wavereader.fmt.numChannels)/(double)wavereader.fmt.sample_rate;
	double samplesPerSecond = (double)reducedSamples.size() / song_length;
	window.create(sf::VideoMode(samplesPerSecond, windowHeight+200), "Visualization");
	window.setFramerateLimit(frameRate); // change in the future probably
	music->play();
	while (window.isOpen())
	{
		sf::Vector2f previousSize(window.getSize());
		sf::Event event;
		sf::RectangleShape playButton,pauseButton;
		createPlayPauseButtons(&window, playButton, pauseButton);
		while(window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::Resized)
			{
				double scaleX = event.size.width / previousSize.x;
				double scaleY = event.size.height / previousSize.y;
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				std::cout << event.mouseButton.x << " " << event.mouseButton.y << std::endl;
				auto boundsPlay = playButton.getLocalBounds();
				auto boundsPause = pauseButton.getLocalBounds();
				if (playButton.getLocalBounds().contains(sf::Vector2f(event.mouseButton.x - playButton.getPosition().x, event.mouseButton.y-playButton.getPosition().y)))
				{
					paused = !paused;
					music->play();
				}
				else if (pauseButton.getLocalBounds().contains(sf::Vector2f(event.mouseButton.x-pauseButton.getPosition().x, event.mouseButton.y-pauseButton.getPosition().y)))
				{
					paused = !paused;
					music->pause();
				}
			}
		}
		if (!paused)
		{
			window.clear(sf::Color::Black); // clears the frame with given color

			// draw
			for (int i = offset; i < (samplesPerSecond / frameRate) + offset && i < length; i++)
			{
				//double height = 600.0 - transform(reducedSamples[i], channelMask);
				double height = (reducedSamples[i] / max) * windowHeight / 2;
				sf::RectangleShape rectangle(sf::Vector2f(frameRate - (spacing / 2), height * -1));
				rectangle.setFillColor(sf::Color::Blue);
				rectangle.setPosition(sf::Vector2f(((i - offset) * frameRate), windowHeight / 2));
				window.draw(rectangle);
			}
			offset += samplesPerSecond / frameRate;
		}
		//createPlayPauseButton(&window);
		window.draw(playButton);
		window.draw(pauseButton);
		window.display();
	}
}
