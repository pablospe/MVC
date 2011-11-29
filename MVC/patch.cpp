#include "patch.h"
#include "main.h"
#include "window.h"

Patch::Patch()
:	origImg(NULL),currImg(NULL),
	img_width(0), img_height(0),
	lowX(0), lowY(0), highX(0), highY(0)
{
	boundary.clear();
	interior.clear();
	rows.clear();
}

void Patch::init(Image* imgOrig, Image* imgCurr)
{
	origImg = imgOrig ; currImg = imgCurr;
	img_width = imgOrig->getWidth();
	img_height = imgOrig->getHeight();
	lowX = 0; lowY = 0; highX = 0; highY = 0;
	boundary.clear();
	interior.clear();
	rows.clear();
}

bool Patch::addPoint(Point& vertex)
{
	if (boundary.size() > 3) {
		if (close(vertex,boundary[0]))
			return true;

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


void Patch::fillBoundary()
{
	vector<Point> border;
	border.reserve(pow(double(boundary.size()),2));
	for (unsigned int i = 0; i < boundary.size(); ++i) {
		fillLine(boundary[i], boundary[((i+1) % boundary.size())], border);
	}

	swap(border,boundary);
}


void Patch::fillLine(Point pt1, Point pt2, vector<Point>& border)
{
	vector<Point> tempBorder;
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
	bool reverse = false;
	if (x0 > x1) {
		reverse = true;
		swap(x0,x1);
		swap(y0,y1);
	}

	// actual difference
	int dX = x1 - x0;
	int dY = abs(y1 - y0);

	tempBorder.reserve(max(dX,dY));

	double cumErr = 0;
	double stepErr = double(dY) / double(dX); // error in each step

	int yStep = 1;
	if ((y0 - y1) > 0)
		yStep *= -1;

	for (; x0 <= x1; ++x0) {
		// add point
		if (steep)
			tempBorder.push_back(Point(y0,x0));
		else
			tempBorder.push_back(Point(x0,y0));

		// update error
		cumErr += stepErr;
		if (cumErr >= .5) {
			y0 += yStep;
			--cumErr;
		}
	}

	if (reverse)
		border.insert(border.end(), tempBorder.rbegin(), tempBorder.rend());
	else
		border.insert(border.end(), tempBorder.begin(), tempBorder.end());
}


void Patch::highLight()
{
	for (unsigned int i = 0; i < boundary.size(); ++i)
		for (int chn = RED; chn <= BLUE; ++chn)
			currImg->setPixel_(boundary[i].x,boundary[i].y,chn,1);
}


void Patch::clear()
{
	imageRevertSrc();
	boundary.clear();
	interior.clear();
	rows.clear();
	lowX=lowY=highX=highY=0;
	source.dClone = false;
	source.cClone = false;
	glutPostRedisplay();
}

void Patch::closed()
{
	cout << "Patch is closed" << endl;
	fillBoundary();
	computeInterior();
	color();
	source.cClone = false;
	source.dClone = false;
	glutPostRedisplay();
}


void Patch::computeRows()
{
	for (unsigned int i = 0; i < boundary.size(); ++i) {
		int x = boundary[i].x;
		int y = boundary[i].y;

		if (rows.count(x) == 0) {
			vector<int> yValues;
			yValues.push_back(y);
			rows[x] = yValues;
		}

		else
			rows[x].push_back(y);
	}
}

void Patch::computeInterior()
{
	computeRows();
	interior.reserve(pow(double(boundary.size()),2));

	for (int x = lowX; x <= highX; ++x) {
		for (int y = lowY; y <= highY; ++y) {
			if (interiorPoint(x,y))
				interior.push_back(Point(x,y));
			else
				currImg->setPixel_(x,y,Pixel());
		}
	}
	for (int x = 0; x < img_width; ++x)
		for (int y = 0; y < img_height; ++y)
			if (x < lowX || x > highX || y < lowY || y > highY)
				currImg->setPixel_(x,y,Pixel());
}

bool Patch::interiorPoint(int x, int y)
{
	if (rows.count(x) == 0)
		return false;
	else
		return checkRow(x,y,rows[x]);
}

bool Patch::checkAdjaceny(int x, int y, vector<int>& yBoundary)
{
	sort(yBoundary.begin(),yBoundary.end());

	bool adj = true;
	for (unsigned int i = 0; i < yBoundary.size() - 1; ++i)
		if (yBoundary[i] + 1 != yBoundary[i+1] && yBoundary[i] != yBoundary[i+1])
			adj = false;

	return adj;
}


bool Patch::checkRow(int x, int y, vector<int>& yBoundary)
{
	if (checkAdjaceny(x,y,yBoundary)) {
		for (unsigned int i = 0; i < yBoundary.size(); ++i)
			if (y == yBoundary[i])
				return true;
		return false;
	}

	int intersection = 0;
	vector<int> sub;

	for (unsigned int i = 0; i < rows[x].size(); ++i) {
		if (rows[x][i] > y) {
			bool adjacent = false;

			for (unsigned int j = 0; j < sub.size(); ++j)
				if (abs(sub[j] - rows[x][i]) <= 1) {
					adjacent = true;
					break;
				}
			
			sub.push_back(rows[x][i]);

			if (!adjacent)
				++intersection;
		}
	}
	
	return (intersection % 2 == 1);
}

void Patch::color()
{
	/***
	for (unsigned int i = 0; i < interior.size(); ++i)
		for (int chn = RED; chn <= BLUE; ++chn)
			currImg->setPixel_(interior[i].x,interior[i].y,chn,1);
	***/
}