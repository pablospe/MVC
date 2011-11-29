#ifndef WINDOW_H
#define WINDOW_H

#include "image.h"
#include "patch.h"
#include "common.h"

class Window
{
public:

	Patch pat;
	// Data members
	int height, width;
	int glNum;

	Image* currentImg, originalImg;
};

#endif