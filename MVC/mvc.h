#ifndef MVC_H
#define MVC_H

#include "common.h"
#include "image.h"
#include "patch.h"
#include "geometry.h"

// Top-level algorithm
void composite(Point start);

// Helper functions
std::vector<double> meanValueCoordinates(Point pt);
std::vector<Point> targetBoundary(Point start);
std::vector<Pixel> boundaryDiff(Point start);
std::vector<std::vector<Point> > boundaryHierarchy();
double boundaryWeight(Point pt, int index);

#endif