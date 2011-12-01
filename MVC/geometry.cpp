#include "geometry.h"

double dist(Point pt0, Point pt1)
{
	return sqrt( double(sqr(pt0.x - pt1.x) + sqr(pt0.y - pt1.y)) );
}

double size(Point pt)
{
	return dist(pt,Point(0,0));
}

double angle(Point ref, Point a, Point b)
{
	// Create two vectors
	Point refToA(a.x - ref.x, a.y - ref.y);
	Point refToB(b.x - ref.x, b.y - ref.y);

	// Find angle between them
	double dot = refToA.x * refToB.x + refToA.y * refToB.y;
	double sizeA = size(refToA);
	double sizeB = size(refToB);

	if (sizeA == 0 || sizeB == 0)
		return 0;

	return acos(clamp(dot / (sizeA * sizeB),-1,1));
}