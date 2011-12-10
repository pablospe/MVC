#ifndef MEMBRANE_H
#define MEMBRANE_H

#include "common.h"
#include "image.h"
#include "patch.h"
#include "geometry.h"

class Membrane {
public:
	typedef std::pair<std::vector<double>,std::vector<int> > MVC;
	Membrane(Point Start);
	// Top-level algorithm
	void composite();

	// Helper functions
	MVC meanValueCoordinates(Point pt);
	std::vector<Point> targetBoundary();
	std::vector<Pixel> boundaryDiff();
	std::vector<int> refinedBoundary(Point pt);
	void boundaryHierarchy();
	double boundaryWeight(Point pt, int index);
	bool refinedEnough(Point pt, Point vertex, Point next, Point prev, int level);
	void buildBoundary(Point pt, int index, int step, int level, std::vector<int>& refBoundary);
	int wrap(int index);
private:
	Point start;
	std::vector<std::vector<Pixel> > history;
	std::vector<MVC> meanValues;
	std::vector<std::vector<Point> > hierarchy;
};

#endif