#ifndef PATCH_H
#define PATCH_H

#include "image.h"
#include "common.h"
#include <algorithm>


class Patch
{
public:
	Patch();
	void init(Image* imgOrig, Image* imgCurr);

	bool addPoint(Point& vertex);
	bool interiorPoint(int x, int y);
	bool checkRow(int x,int y, vector<int>& yBoundary);
	bool checkAdjaceny(int x, int y, vector<int>& yBoundary);

	void computeRows();
	void computeInterior();
	void fillBoundary();
	void fillLine(Point pt1, Point pt2, vector<Point>& border);

	void highLight();
	void color();
	void clear();
	void closed();

	// members
	Image* origImg;
	Image* currImg;
	int img_width, img_height;
	int lowX, lowY, highX, highY;
	vector<Point> interior;
	vector<Point> boundary;
	map<int, std::vector<int> > rows;
};

#endif