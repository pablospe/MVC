#include "patch.h"
#include "main.h"

Patch::Patch(int width, int height)
:	img_width(width), img_height(height)
{
}

void Patch::addPoint(Point& vertex)
{
	boundary.push_back(vertex);
}

void Patch::computeInterior()
{
	interior.reserve(pow(double(boundary.size()),2));
}