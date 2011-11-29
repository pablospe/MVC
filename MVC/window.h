#ifndef WINDOW_H
#define WINDOW_H

#include "image.h"
#include "patch.h"
#include "common.h"

class Window
{
public:
	Window();

	// Data members
	Patch patch;
	int height, width;
	int glNum;
	bool dClone, cClone, paste;

	Image* currentImg;
	Image* originalImg;
};

#endif