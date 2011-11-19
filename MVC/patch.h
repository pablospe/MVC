#ifndef PATCH_H
#define PATCH_H

#include "image.h"
#include "common.h"


class Patch
{
public:
	Patch();
	Patch(int width, int height);
	bool addPoint(Point& vertex);
	void computeInterior();
	void highLight(Image* img);

	vector<Point> interior;
	vector<Point> boundary;
	int img_width, img_height;
};

#endif