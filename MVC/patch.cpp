#include "patch.h"
#include "main.h"

Patch::Patch()
:	img_width(0), img_height(0)
{
}

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

void Patch::highLight(Image* img)
{
	for (unsigned int i = 0; i < boundary.size(); ++i)
		for (int chn = RED; chn <= BLUE; ++chn)
			img->setPixel_(boundary[i].x,boundary[i].y,chn,1);
}