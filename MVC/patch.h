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
	void fillBoundary();
	void linearize(Point pt1, Point pt2, vector<Point>& border);
	void highLight(Image* img) const;

	vector<Point> interior;
	vector<Point> boundary;
	int img_width, img_height;
};

#endif