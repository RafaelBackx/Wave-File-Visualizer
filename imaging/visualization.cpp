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

void sfmlVisualization::visualize(wave::WaveReader& wavereader)
{
	std::vector<int> reducedSamples = wavereader.reduceSamples();
	const int channelMask = getMask(wavereader.fmt.bitsPerSample);
	double min = 0, max = 0;
	getMinMax(reducedSamples,&min,&max);
	std::cout << "min: " << min << " max: " << max << std::endl;
	sf::RenderWindow window;
	int windowWidth = 800, windowHeight = 800;
	int spacing = 5;
	int offset = 0;
	int length = reducedSamples.size();
	int frameRate = 60;
	//window.setVerticalSyncEnabled(true); // Apparently you cannot use these functions with each other
	int song_length = (wavereader.getSamples().size()/wavereader.fmt.numChannels)/wavereader.fmt.sample_rate;
	int samplesPerSecond = reducedSamples.size() / song_length;
	std::cout << "reduced samples: " << reducedSamples.size() << std::endl;
	std::cout << "song length: " << song_length << std::endl;
	std::cout << "samples/second: " << samplesPerSecond << std::endl;
	window.create(sf::VideoMode(samplesPerSecond, windowHeight), "Visualization");
	window.setFramerateLimit(frameRate); // change in the future probably
	while (window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::Resized)
			{
				std::cout << "yeees debugging time!!!!!" << std::endl;
			}
		}
		window.clear(sf::Color::White); // clears the frame with given color
		// draw
		for (int i=offset;i<(samplesPerSecond/frameRate)+offset && i<length;i++)
		{
			//double height = 600.0 - transform(reducedSamples[i], channelMask);
			double height = (reducedSamples[i] / max) * windowHeight/2;
			//std::cout <<reducedSamples[i] << " " <<  height << std::endl;
			sf::RectangleShape rectangle(sf::Vector2f(frameRate-5,height*-1));
			rectangle.setFillColor(sf::Color::Blue);
			//abs(20 * log10(abs((double)(reducedSamples[i] & channelMask))
			//std::cout << abs(20 * log10(abs((double)(reducedSamples[i] & channelMask)))) << std::endl;
			//std::cout << transform(reducedSamples[i],channelMask) << std::endl;
			//std::cout << reducedSamples[i] << std::endl;
			//std::cout << (i - offset) * 5 << std::endl;
			rectangle.setPosition(sf::Vector2f(((i-offset)*frameRate),windowHeight/2));
			window.draw(rectangle);
		}
		offset+=samplesPerSecond/frameRate;
		window.display();
	}
}
