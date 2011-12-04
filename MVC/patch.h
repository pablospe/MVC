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

	bool empty();
	bool addPoint(Point& vertex);
	bool interiorPoint(int x, int y);
	bool checkRow(int x,int y, std::vector<int>& yBoundary);
	bool checkAdjaceny(int x, int y, std::vector<int>& yBoundary);

	void addPointHelper(Point& vertex);
	void computeRows();
	void computeInterior();
	void fillBoundary();
	void fillLine(Point pt1, Point pt2, std::vector<Point>& border);

	void highLight();
	void clear();
	void closed();

	// members
	Image* origImg;
	Image* currImg;
	int img_width, img_height;
	int lowX, lowY, highX, highY;
	std::vector<Point> interior;
	std::vector<Point> boundary;
private:
	std::map<int, std::vector<int> > rows;
};

class Window
{
public:
	Window();
	void update();

	// Data members
	Point pastePoint;
	Patch patch;
	int height, width;
	int glNum;
	bool dClone, cClone, paste, batch;

	Image* currentImg;
	Image* originalImg;
};

#endif