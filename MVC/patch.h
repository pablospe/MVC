#ifndef PATCH_H
#define PATCH_H

#include "image.h"
#include "common.h"


class Patch
{
public:
	Patch(int width, int height);
	void addPoint(Point& vertex);
	void computeInterior();

	vector<Point> interior;
	vector<Point> boundary;
	int img_width, img_height;
};

#endif