#ifndef MEMBRANE_H
#define MEMBRANE_H

#include "common.h"
#include "image.h"
#include "patch.h"
#include "geometry.h"

class Membrane {
public:
	typedef std::pair<std::vector<double>,std::vector<int> > Mvc;
	Membrane(Point Start);

	// Top-level algorithm
	void composite();

	// For comparison
	void naiveComposite();

private:
	// Helper functions
	Mvc meanValueCoordinates(Point pt);

	std::vector<Point>	targetBoundary();
	std::vector<Pixel>	differenceBoundary();
	std::vector<size_t> refinedBoundary(Point pt);

	void computeBoundaryLevels();
	void computeThresholdLevels();
	void computeRefinedBoundary(Point pt, int index, int step, int level, std::vector<size_t>& refBoundary);

	int		wrap(int index);
	bool	refinedEnough(Point pt, Point vertex, Point next, Point prev, int level);
	double	boundaryWeight(Point pt, int localIndex, std::vector<size_t>& refBoundary);

	// Data members
	Point									start;
	size_t									boundaryLevels;
	std::vector<Mvc>						meanValues;
	std::vector<Point>*						boundary;
	std::vector<Point>*						interior;
	std::vector<std::vector<Pixel> >		history;
	std::vector<std::pair<double,double> >	thresholds;
};

#endif