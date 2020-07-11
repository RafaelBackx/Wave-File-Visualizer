#include <iostream>
#include <fstream>
#include "wave/wave.h"
#include "util//position.h"
#include "imaging/visualization.h"
#include <cmath>
int main()
{
	sfmlVisualization::Visualizer vis(300, 500);
	vis.showMenu();
	return 0;
}
