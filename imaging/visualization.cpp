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

void sfmlVisualization::Visualizer::showMenu()
{
	sf::Event event;
	gui::Button start(100, 30, "start visulisation");
	start.setPosition(sf::Vector2f((this->window.getSize().x/2)-(start.getWidth()/2), 100));
	sf::RenderWindow* window = &this->window;
	start.setOnClick([&]()
	{
		//this->window.close();
		this->visualize();
	});
	this->screen.addNode(&start);
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
			start.draw(this->window);
			//Display
			this->window.display();
		}
	}
}

sf::Color getColor(sf::Color c)
{
	int r = c.r, g = c.g, b = c.b;
	if (r > 0 && b == 0) 
	{
		r--;
		g++;
	}
	if (g > 0 && r == 0) 
	{
		g--;
		b++;
	}
	if (b > 0 && g == 0) 
	{
		r++;
		b--;
	}
	return sf::Color(r, g, b);
}

void sfmlVisualization::Visualizer::visualize()
{
	//std::vector<int> reducedSamples = wavereader.reduceSamples();
	const int channelMask = getMask(this->wavereader.fmt.bitsPerSample);
	double max = pow(2, this->wavereader.fmt.bitsPerSample);
	//sf::RenderWindow window;
	int windowWidth = 1500, windowHeight = 700;
	int spacing = 5;
	int spacingBetweenButtons = 10;
	int offset = 0;
	int length = this->wavereader.getSamples().size()/this->wavereader.fmt.numChannels;
	int frameRate = 30;
	double song_length = (double)length/(double)this->wavereader.fmt.sample_rate;
	double samplesPerFrame = this->wavereader.fmt.sample_rate / frameRate;
	double rectWidth = windowWidth / samplesPerFrame;
	//window.create(sf::VideoMode(windowWidth, windowHeight+200), "Visualization");
	this->window.setFramerateLimit(frameRate); // change in the future probably
	sf::Color color = sf::Color::Red;
	//music.setPlayingOffset(sf::Time(sf::microseconds(240000000)));
	music.play();
	sf::Image img;
	this->window.setPosition(sf::Vector2i(25, 25));
	this->window.setSize(sf::Vector2u(windowWidth, windowHeight+200));
	gui::Button playButton(75, 75, "");
	sf::View view(sf::FloatRect(0, 0, windowWidth, windowHeight+200));
	playButton.setOnClick([&]()
		{
			music.play();
		});
	playButton.setTexture("sprites/play.png");
	playButton.setPosition(sf::Vector2f(windowWidth/2-playButton.getWidth() - spacingBetweenButtons, this->window.getSize().y - ((this->window.getSize().y - windowHeight)/2)-playButton.getHeight()));
	gui::Button pauseButton(75, 75, "");
	pauseButton.setOnClick([&]()
		{
			music.pause();
		});
	pauseButton.setTexture("sprites/pause.png");
	pauseButton.setPosition(sf::Vector2f(windowWidth / 2 + playButton.getWidth() + spacingBetweenButtons, this->window.getSize().y - ((this->window.getSize().y - windowHeight) / 2) - playButton.getHeight()));
	this->screen.addNode(&playButton);
	this->screen.addNode(&pauseButton);
	music.setVolume(50.0f);
	while (this->window.isOpen())
	{
		sf::Event event;
		while(this->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				this->window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);
				this->screen.updateNodesOnClick(mouse);
			}
			if (event.type == sf::Event::Resized)
			{
				sf::View visibleArea(sf::FloatRect(0, 0, event.size.width, event.size.height));
				this->window.setView(visibleArea);
			}
		}
		if (music.Playing)
		{
			window.clear(sf::Color::White); // clears the frame with given color
			offset = this->music.getPlayingOffset().asSeconds() * this->wavereader.fmt.sample_rate;
			color = getColor(color);
			// draw
			for (int i = offset; i < samplesPerFrame + offset && i < length; i++)
			{
				//double height = 600.0 - transform(reducedSamples[i], channelMask);
				double height = (this->wavereader.getSamples()[i] / max) * windowHeight / 2;
				sf::RectangleShape rectangle(sf::Vector2f(rectWidth, height * -1));
				rectangle.setFillColor(color);
				rectangle.setPosition(sf::Vector2f(((i - offset)*rectWidth), (windowHeight / 2)));
				this->window.draw(rectangle);
			}
		}
		playButton.draw(this->window);
		pauseButton.draw(this->window);
		this->window.display();
	}
}
