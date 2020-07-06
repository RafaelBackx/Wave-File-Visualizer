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
	gui::TextBox textbox(200, 50);
	textbox.setPosition((this->window.getSize().x / 2) - (textbox.getWidth()/2), 100);
	textbox.setOnClick([&]()
		{
			
		});
	textbox.setBorderWidth(1.0f);
	textbox.setBorderColor(sf::Color::Black);
	gui::Button start(100, 30, "start visulisation");
	start.setPosition(sf::Vector2f((this->window.getSize().x / 2) - (start.getWidth() / 2), 250));
	start.setOnClick([&]()
		{
			this->visualize(textbox.getText());
		});
	sf::Clipboard clipboard;
	bool ctrlDown=false;
	this->screen.addNode(&textbox);
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
			case sf::Event::TextEntered:
			{
				std::cout << event.text.unicode << std::endl;
				if (event.text.unicode < 128)
				{
					if (event.text.unicode == 8)
					{
						textbox.removeLastChar();
					}
					else if(event.text.unicode == 22 && ctrlDown) // check for control paste
					{
						textbox.setString(clipboard.getString().toAnsiString());
					}
					else if (event.text.unicode == 3 && ctrlDown) // check for control copy
					{
						clipboard.setString(textbox.getText());
					}
					else
					{
						char t = static_cast<char>(event.text.unicode);
						textbox.addText(std::string(1, t));
						int index = textbox.getText().length() - 1;
						if (textbox.getSFMLText().findCharacterPos(index).x > this->window.getSize().x - 50)
						{
							textbox.addText(std::string("\n"));
						}
					}
				}
			}
			case sf::Event::KeyPressed:
			{
				if (event.key.code == 71)//Left arrow key
				{
					textbox.decrementTextOffset();
				}
				else if (event.key.code == 72)//Right arrow key
				{
					textbox.incrementTextOffset();
				}
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
			start.draw(this->window);
			textbox.draw(this->window);
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
	this->wavereader.read(filename);
	if (!music.openFromFile(filename))
	{
		std::cout << "Cannot open music file" << std::endl;
	}
	const int channelMask = getMask(this->wavereader.fmt.bitsPerSample);
	double max = pow(2, this->wavereader.fmt.bitsPerSample);
	int windowWidth = 1500, windowHeight = 700;
	int spacing = 5;
	int spacingBetweenButtons = 10;
	int offset = 0;
	int length = this->wavereader.getSamples().size()/this->wavereader.fmt.numChannels;
	int frameRate = 30;
	double song_length = (double)length/(double)this->wavereader.fmt.sample_rate;
	double samplesPerFrame = this->wavereader.fmt.sample_rate / frameRate;
	double rectWidth = windowWidth / samplesPerFrame;
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
			std::cout << "new circle x position: "  << volume.getCircle().getPosition().x << std::endl;
		});
	volume.setValue(50.0f);
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
	gui::ListBox listBoxMeta(0, 0);
	for (wave::ListField field : this->wavereader.getMetaData())
	{
		std::string text = this->wavereader.list.listIds.at(field.getId()) + ": " + field.getValue();
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
		volume.draw(this->window);
		musicTime.draw(this->window);
		listBoxMeta.draw(this->window);
		this->window.display();
	}
}
