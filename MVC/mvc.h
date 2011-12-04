#ifndef MVC_H
#define MVC_H

#include "common.h"
#include "image.h"
#include "patch.h"
#include "geometry.h"

void composite(Point start);

std::vector<double> meanValueCoordinates(Point pt);
std::vector<Point> targetBoundary(Point start);
std::vector<Pixel> boundaryDiff(Point start);

Point translate(Point oldOrigin, Point newOrigin, Point pt);

double boundaryWeight(Point pt, int index);

#endif