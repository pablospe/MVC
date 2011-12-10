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
	//std::vector<double> meanValueCoordinates(Point pt);
	MVC meanValueCoordinates(Point pt);
	std::vector<Point> targetBoundary();
	std::vector<Pixel> boundaryDiff();
	void boundaryHierarchy();
	double boundaryWeight(Point pt, int index);
private:
	Point start;
	std::vector<std::vector<Pixel> > history;
	//std::vector<std::vector<double> > meanValues;
	std::vector<MVC> meanValues;
	std::vector<std::vector<Point> > hierarchy;
};

#endif