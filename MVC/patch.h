#ifndef PATCH_H
#define PATCH_H

#include "image.h"
#include "common.h"
#include "membrane.h"

class Patch
{
public:
	Patch();
	bool empty();
	bool addPoint(Point& vertex);

	void clear();
	void closed();

	void setOrigImg(Image*);
	void setCurrImg(Image*);
	void setImgWidth(int);
	void setImgHeight(int);

	void removeLastBoundary();
	Point lastBoundary();
	size_t boundarySize();

	friend class Membrane;

private:
	void init(Image* imgOrig, Image* imgCurr);
	void addPointHelper(Point& vertex);
	void computeRows();
	void computeInterior();
	void colorFirstPoint();
	void fillBoundary();
	void fillLine(Point pt1, Point pt2, std::vector<Point>& border);

	bool interiorPoint(int x, int y);
	bool checkRow(int x,int y, std::vector<int>& yBoundary);
	bool checkAdjaceny(int x, int y, std::vector<int>& yBoundary);

	// Data members
	Image*				origImg;
	Image*				currImg;
	int					lowX, lowY, highX, highY;
	int					imgWidth, imgHeight;
	std::vector<Point>	interior;
	std::vector<Point>	boundary;
	std::map<int, std::vector<int> >	rows;
};

class Window
{
public:
	Window();
	void update();

	// Data members
	Point	pastePoint;
	Patch	patch;
	int		height, width;
	int		glNum;
	bool	dClone, cClone, paste, naive, batch;
	Image*	currentImg;
	Image*	originalImg;
};

#endif