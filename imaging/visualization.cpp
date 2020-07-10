#include "visualization.h"
#include "..//util/portable-file-dialogs.h"

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
	window.setSize(sf::Vector2u(300, 500));
	sf::View view(sf::FloatRect(0, 0, 300, 500));
	window.setView(view);
	sf::Event event;
	gui::Button openFile(100, 30, "Open File...");
	openFile.setPosition(sf::Vector2f((this->window.getSize().x / 2) - (openFile.getWidth() / 2), 100));
	openFile.setOnClick([&]()
		{
			auto selection = pfd::open_file("Select a wav file", ".", { "Audio Files", "*.wav" }).result();
			if (!selection.empty())
			{
				this->visualize(selection[0]);
			}			
		});
	sf::Clipboard clipboard;
	bool ctrlDown=false;
	this->screen.addNode(&openFile);
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
			case sf::Event::KeyPressed:
			{
				ctrlDown = event.key.control;
			}
			case sf::Event::KeyReleased:
				ctrlDown = event.key.control;
			default:
				break;
			}
			//Clear
			this->window.clear(sf::Color::White);
			//Draw
			openFile.draw(this->window);
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

void sfmlVisualization::Visualizer::visualize(std::string filename)
{
	//this->wavereader.read(filename);
	this->streamer.updateBuffer(0);
	this->streamer.setFile(filename);
	std::cout << filename << std::endl;
	if (!music.openFromFile(filename))
	{
		std::cout << "Cannot open music file" << std::endl;
	}
	const int channelMask = getMask(this->streamer.fmt.bitsPerSample);
	double max = pow(2, this->streamer.fmt.bitsPerSample);
	int windowWidth = 1500, windowHeight = 700;
	int spacing = 5;
	int spacingBetweenButtons = 10;
	int offset = 0;
	int frameRate = 30;
	double samplesPerFrame = this->streamer.fmt.sample_rate / frameRate;
	double rectWidth = (double) windowWidth / (double) this->streamer.getBufferSize();
	std::cout << rectWidth << std::endl;
	this->window.setFramerateLimit(frameRate); // change in the future probably
	sf::Color color = sf::Color::Red;
	sf::Image img;
	this->window.setPosition(sf::Vector2i(25, 25));
	this->window.setSize(sf::Vector2u(windowWidth, windowHeight+200));
	sf::Texture play;
	play.loadFromFile("sprites/play.png");
	sf::Texture pause;
	pause.loadFromFile("sprites/pause.png");
	gui::Button playButton(75, 75, "");
	sf::View view(sf::FloatRect(0, 0, windowWidth, windowHeight+200));
	bool playing = true;
	playButton.setOnClick([&]()
		{
			if (playing)
			{
				playButton.setTexture(play);
				music.pause();
				playing = false;
			}
			else
			{
				playButton.setTexture(pause);
				music.play();
				playing = true;
			}
		});
	playButton.setTexture(pause);
	playButton.setPosition(sf::Vector2f(windowWidth/2 - playButton.getWidth()/2, this->window.getSize().y - ((this->window.getSize().y - windowHeight)/2)-playButton.getHeight()));
	gui::Slider volume(300, 10, 15);
	volume.setCircleColor(sf::Color::Color(48,75,194));
	volume.setInnerRectColor(sf::Color::Color(50, 156, 66));
	volume.setOuterRectColor(sf::Color::Color(81,82,87));
	volume.setPosition(sf::Vector2f(windowWidth * 0.75 , this->window.getSize().y - ((this->window.getSize().y - windowHeight) * 0.25) - volume.getHeight()));
	volume.setOnDrag([&]()
		{
			music.setVolume(volume.getValue());
		});
	volume.setOnClick([&]()
		{
			music.setVolume(volume.getValue());
		});
	volume.setValue(25.0f);
	gui::Slider musicTime(400,10,15);
	musicTime.setCircleColor(sf::Color::Color(48, 75, 194));
	musicTime.setInnerRectColor(sf::Color::Color(50,156,66));
	musicTime.setOuterRectColor(sf::Color::Color(81, 82, 87));
	musicTime.setPosition(sf::Vector2f(windowWidth / 2 - (musicTime.getWidth() / 2) + spacingBetweenButtons, this->window.getSize().y - ((this->window.getSize().y - windowHeight) / 4) - musicTime.getHeight()));
	musicTime.setMaxValue(music.getDuration().asSeconds());
	musicTime.setOnClick([&]()
		{
			music.pause();
			sf::Time offset(sf::microseconds(static_cast<uint64_t>(musicTime.getValue()) * 1000000));
			music.setPlayingOffset(offset);
			music.play();
		});
	musicTime.setOnDrag([&]()
		{
			music.pause();
			sf::Time offset(sf::microseconds(static_cast<uint64_t>(musicTime.getValue()) * 1000000));
			music.setPlayingOffset(offset);
			music.play();
		});
	gui::Button backToMenu(200, 50, "Back To Menu");
	backToMenu.setOnClick([&]()
		{
			music.stop();
			this->showMenu();
		});
	backToMenu.setPosition(sf::Vector2f((windowWidth / 4) - backToMenu.getWidth(), this->window.getSize().y - ((this->window.getSize().y - windowHeight) / 4) - backToMenu.getHeight()));
	gui::ListBox listBoxMeta(0, 0);
	for (wave::ListField field : this->streamer.getMetaData())
	{
		std::string text = this->streamer.list.listIds.at(field.getId()) + ": " + field.getValue();
		std::cout << text << std::endl;
		std::unique_ptr<gui::ListItem> item = std::make_unique<gui::ListItem>(text ,100,50);
		item->setPadding(2.0f);
		item->setColor(sf::Color::Color(210, 210, 210, 240));
		item->setCharacterSize(21);
		listBoxMeta.addItem(std::move(item));
	}
	listBoxMeta.setPosition(windowWidth - listBoxMeta.getSize().x,0);
	bool mouseDown = false;
	this->screen.addNode(&playButton);
	this->screen.addNode(&volume);
	this->screen.addNode(&musicTime);
	this->screen.addNode(&backToMenu);
	music.play();
	while (this->window.isOpen())
	{
		sf::Event event;
		while(this->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				this->window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				mouseDown = true;
				sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);
				this->screen.updateNodesOnClick(mouse);
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				mouseDown = false;
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				if (mouseDown)
				{
					this->screen.updateNodesOnDrag(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
				}
				this->screen.updateNodesOnHover(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
			}
			else if (event.type == sf::Event::Resized)
			{
				sf::View visibleArea(sf::FloatRect(0, 0, event.size.width, event.size.height));
				this->window.setView(visibleArea);
			}
		}
		music.setVolume(volume.getValue());
		if (music.Playing)
		{
			musicTime.setValue(music.getPlayingOffset().asSeconds());
			window.clear(sf::Color::White); // clears the frame with given color
			offset = (this->music.getPlayingOffset().asSeconds() * this->streamer.fmt.sample_rate)*(this->streamer.fmt.bitsPerSample/8);
			color = getColor(color);
			// draw
			//for (int i = offset; i < samplesPerFrame + offset && i < length; i++)
			for (int i=0;i<this->streamer.getBuffer().size();i++)
			{
				//double height = 600.0 - transform(reducedSamples[i], channelMask);
				//int value = this->wavereader.getSamples()[i];
				int value = this->streamer.getBuffer()[i];
				//	std::cout << "Test: " << std::dec << value << std::endl;
				double height = (value / max) * windowHeight / 2;
				sf::RectangleShape rectangle(sf::Vector2f(rectWidth, height * -1));// times -1 because we want to draw up for positive and down for negative
				rectangle.setFillColor(color);
				//rectangle.setPosition(sf::Vector2f(((i - offset)*rectWidth), (windowHeight / 2)));
				rectangle.setPosition(sf::Vector2f((i*rectWidth), (windowHeight / 2)));
				this->window.draw(rectangle);
			}
			this->streamer.updateBuffer(offset);
		}
		playButton.draw(this->window);
		volume.draw(this->window);
		musicTime.draw(this->window);
		listBoxMeta.draw(this->window);
		backToMenu.draw(this->window);
		this->window.display();
	}
}
