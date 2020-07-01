#include "visualization.h"

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

void createPlayPauseButtons(sf::RenderWindow* window, sf::RectangleShape* playButton, sf::RectangleShape* pauseButton)
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
	playButton->setFillColor(sf::Color::Green);
	sf::Vector2f positionPlay((window->getSize().x / 2)-spacingBetweenButtons/2, window->getSize().y - 100);
	playButton->setSize(sf::Vector2f(buttonsWidth, buttonsHeight));
	playButton->setPosition(positionPlay);

	// PAUSEBUTTON	
	sf::Texture pauseImage;
	if (!pauseImage.loadFromFile("sprites/pause.png"))
	{
		std::cout << "cannot load pause.png from file" << std::endl;
	}
	//pauseButton.setTexture(&pauseImage);
	pauseButton->setFillColor(sf::Color::Red);
	sf::Vector2f positionPause(((window->getSize().x / 2) + spacingBetweenButtons / 2)+buttonsWidth, window->getSize().y - 100);
	pauseButton->setSize(sf::Vector2f(buttonsWidth, buttonsHeight));
	pauseButton->setPosition(positionPause);
}

void test(gui::Button& button)
{
	std::cout << "button test" << std::endl;
	if (button.getShape().getPosition() == sf::Vector2f(500,500))
	{
		button.setPosition(sf::Vector2f(700, 700));
	}else
	{
		button.setPosition(sf::Vector2f(500, 500));
	}
	
}

void sfmlVisualization::Visualizer::showMenu()
{
	sf::Event event;
	gui::Button playButton(200, 50, "button test 1");
	gui::Button otherTestButton(50, 100, "just another test");
	playButton.setPosition(sf::Vector2f(500, 500));
	otherTestButton.setPosition(sf::Vector2f(200, 200));
	this->screen.addNode(&playButton);
	this->screen.addNode(&otherTestButton);
	while (this->window.isOpen())
	{
		while (this->window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				this->window.close();
				break;
			case sf::Event::MouseButtonPressed:
			{
				sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);
				this->screen.updateNodesOnClick(mouse);
				break;
			}
			case sf::Event::MouseMoved:
			{
				sf::Vector2f mouse(event.mouseMove.x, event.mouseMove.y);
				this->screen.updateNodesOnHover(mouse);
				break;
			}
			default:
				break;
			}
			//Clear
			this->window.clear(sf::Color::White);
			//Draw
			playButton.draw(window);
			otherTestButton.draw(window);
			/*sf::RectangleShape playButton;
			playButton.setSize(sf::Vector2f(200, 50));
			playButton.setFillColor(sf::Color::White);
			window.draw(playButton);*/
			//Display
			this->window.display();
		}
	}
}

void sfmlVisualization::visualize(wave::WaveReader& wavereader, sf::Music* music)
{
	//std::vector<int> reducedSamples = wavereader.reduceSamples();
	const int channelMask = getMask(wavereader.fmt.bitsPerSample);
	double max = pow(2, wavereader.fmt.bitsPerSample);
	std::cout << "max=" << max << std::endl;
	sf::RenderWindow window;
	int windowWidth = 1500, windowHeight = 800;
	int spacing = 5;
	int offset = 0;
	int length = wavereader.getSamples().size()/wavereader.fmt.numChannels;
	int frameRate = 60;
	bool paused = false;
	double song_length = (double)length/(double)wavereader.fmt.sample_rate;
	double samplesPerFrame = wavereader.fmt.sample_rate / frameRate;
	double rectWidth = windowWidth / samplesPerFrame;
	window.create(sf::VideoMode(windowWidth, windowHeight+200), "Visualization");
	window.setFramerateLimit(frameRate); // change in the future probably
	sf::Clock clock;
	music->play();
	while (window.isOpen())
	{
		//sf::Vector2f previousSize(window.getSize());
		sf::Event event;
		sf::RectangleShape playButton,pauseButton;
		createPlayPauseButtons(&window, &playButton, &pauseButton);
		while(window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				std::cout << event.mouseButton.x << " " << event.mouseButton.y << std::endl;
				auto boundsplay = playButton.getLocalBounds();
				auto boundspause = pauseButton.getLocalBounds();
				if (boundspause.contains(sf::Vector2f(event.mouseButton.x - playButton.getPosition().x, event.mouseButton.y-playButton.getPosition().y)))
				{
					paused = !paused;
					music->play();
				}
				else if (boundspause.contains(sf::Vector2f(event.mouseButton.x-pauseButton.getPosition().x, event.mouseButton.y-pauseButton.getPosition().y)))
				{
					paused = !paused;
					music->pause();
				}
			}
		}
		if (!paused)
		{
			window.clear(sf::Color::Black); // clears the frame with given color
			std::cout << clock.getElapsedTime().asSeconds() << std::endl;
			offset = clock.getElapsedTime().asSeconds() * wavereader.fmt.sample_rate;
			// draw
			for (int i = offset; i < samplesPerFrame + offset && i < length; i++)
			{
				//double height = 600.0 - transform(reducedSamples[i], channelMask);
				double height = (wavereader.getSamples()[i] / max) * windowHeight / 2;
				sf::RectangleShape rectangle(sf::Vector2f(rectWidth, height * -1));
				rectangle.setFillColor(sf::Color::Blue);
				rectangle.setPosition(sf::Vector2f(((i - offset)*rectWidth), windowHeight / 2));
				window.draw(rectangle);
			}
		}
		window.draw(playButton);
		window.draw(pauseButton);
		window.display();
	}
}
