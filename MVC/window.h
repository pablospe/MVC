#ifndef WINDOW_H
#define WINDOW_H

#include "image.h"
#include "patch.h"

class Window
{
public:

	// Data members
	int height, width;
	int glNum;

	Patch patch;
	Image* currentImg, originalImg;
};

#endif