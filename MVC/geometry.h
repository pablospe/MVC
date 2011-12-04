#ifndef GEOM_H
#define GEOM_H

#include "common.h"
#include "image.h"

double angle(Point ref, Point a, Point b);
double size(Point pt);
double dist(Point pt0, Point pt1);

Point translate(Point oldOrigin, Point newOrigin, Point pt);

#endif