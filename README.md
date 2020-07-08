# Wave-File-Visualizer

## Setup instructions

First you need to install [sfml](https://www.sfml-dev.org/download/sfml/2.5.1/) (I downloaded visual c++ 15 32 bit), next you need to link the libraries in visual studio.
Go to Project -> properties 
and under C/C++ -> general -> Additional Include Directories you put the \include folder where you downloaded sfml.

![](docs\img1.jpg)

Repeat the process for "All Options"

![](docs\img2.JPG)

Next go to Linker -> General and under Additional Library Directories you put again the \include directory.
![](docs\img3.JPG)

For the last step go to the \lib folder where you installed sfml and copy the sfml-audio-d.lib and the sfml-graphics-d.lib and paste them in the same directory as the solution file. Now you should be able to run it in visual studio.