#ifndef VISUALIZATION_H
#define VISUALIZATION_H
#include "bmp-format.h"
#include "..//wave/wave.h"
namespace visualization 
{
	void drawRect(const Position& start, const Position& end, imaging::Bitmap& image, const imaging::Color color);
	void visualize(wave::WaveReader& wavereader);
	void splice(imaging::Bitmap& img, unsigned frameWidth, unsigned space);
}


// SFML VISUALIZATION

namespace sfmlVisualization
{
	void visualize(wave::WaveReader& wavereader);
}
#endif