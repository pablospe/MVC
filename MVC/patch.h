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
	void computeRows();
	void computeInterior();
	bool interiorPoint(int x, int y);
	void fillBoundary();
	void linearize(Point pt1, Point pt2, vector<Point>& border);
	void highLight(Image* img);
	void color(Image* img);
	void clear();

	int lowX, lowY, highX, highY;
	vector<Point> interior;
	vector<Point> boundary;
	int img_width, img_height;

	map<int, std::vector<int> > rows;
};

#endif