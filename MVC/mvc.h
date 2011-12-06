#ifndef MVC_H
#define MVC_H

#include "common.h"
#include "image.h"
#include "patch.h"
#include "geometry.h"

class MVC {
public:
	MVC(Point Start);
	// Top-level algorithm
	void composite();

	// Helper functions
	std::vector<double> meanValueCoordinates(Point pt);
	std::vector<Point> targetBoundary();
	std::vector<Pixel> boundaryDiff();
	void boundaryHierarchy();
	double boundaryWeight(Point pt, int index);
private:
	Point start;
	std::vector<std::vector<Pixel> > history;
	std::vector<std::vector<double> > meanValues;
	std::vector<std::vector<Point> > hierarchy;
};

#endif