#include "patch.h"
#include "main.h"

Patch::Patch()
:	img_width(0), img_height(0),
	lowX(0), lowY(0), highX(0), highY(0)
{
	boundary.clear();
}

Patch::Patch(int width, int height)
:	img_width(width), img_height(height),
	lowX(0), lowY(0), highX(0), highY(0)
{
	boundary.clear();
}

bool Patch::addPoint(Point& vertex)
{
	if ((boundary.size() > 3) && (close(vertex,boundary[0])))
		return true;
	else {
		cerr << "Adding vertex at: " << vertex.x << " " << vertex.y << endl;
		boundary.push_back(vertex);

		if (boundary.size() == 1) {
			lowX = vertex.x;
			lowY = vertex.y;
		}
		
		else  {
			// check bounding box
			if (vertex.x < lowX)
				lowX = vertex.x;
			else if (vertex.x > highX)
				highX = vertex.x;

			if (vertex.y < lowY)
				lowY = vertex.y;
			else if (vertex.y > highY)
				highY = vertex.y;
		}

		return false;
	}
	
}

void Patch::computeInterior()
{
	interior.reserve(pow(double(boundary.size()),2));
}

void Patch::fillBoundary()
{
	vector<Point> border;
	border.reserve(boundary.size() * 5);
	for (unsigned int i = 0; i < boundary.size(); ++i) {
		linearize(boundary[i], boundary[((i+1) % boundary.size())], border);
	}

	swap(border,boundary);
}

void Patch::linearize(Point pt1, Point pt2, vector<Point>& border)
{
	int x0 = pt1.x;
	int x1 = pt2.x;
	int y0 = pt1.y;
	int y1 = pt2.y;

	// reorient
	bool steep = (abs(y1 - y0) > abs(x1 - x0));
	if (steep) {
		swap(x0,y0); 
		swap(x1,y1);
	}
	if (x0 > x1) {
		swap(x0,x1);
		swap(y0,y1);
	}

	// actual difference
	int dX = x1 - x0;
	int dY = abs(y1 - y0);

	double cumErr = 0;
	double stepErr = double(dY) / double(dX); // error in each step

	int yStep = 1;
	if ((y0 - y1) > 0)
		yStep *= -1;

	for (; x0 <= x1; ++x0) {
		// add point
		if (steep)
			border.push_back(Point(y0,x0));
		else
			border.push_back(Point(x0,y0));

		// update error
		cumErr += stepErr;
		if (cumErr >= .5) {
			y0 += yStep;
			--cumErr;
		}
	}
}


void Patch::highLight(Image* img) const
{
	for (unsigned int i = 0; i < boundary.size(); ++i)
		for (int chn = RED; chn <= BLUE; ++chn)
			img->setPixel_(boundary[i].x,boundary[i].y,chn,1);
}


void Patch::clear()
{
	boundary.clear();
	interior.clear();
	lowX=lowY=highX=highY=0;
}