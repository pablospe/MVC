#include "mvc.h"
#include "main.h"

using namespace std;

void composite(Point start)
{
	vector<Pixel> diff = boundaryDiff(start);
	
	for (size_t i = 0; i < source.patch.interior.size(); ++i) {
		Point sourcePoint = source.patch.interior[i];
		Point targetPoint = translate(source.patch.boundary[0],start,sourcePoint);

		Pixel interpolant;
		vector<double> meanValues = meanValueCoordinates(sourcePoint);

		assert(meanValues.size() ==  diff.size());

		for (size_t j = 0; j < diff.size(); ++j)		
			interpolant = interpolant + scale(diff[j], meanValues[j]);

		Pixel result = interpolant + source.originalImg->getPixel_(sourcePoint);

		destination.currentImg->setPixel_(targetPoint, result);
	}
	glutPostRedisplay();
}

vector<double> meanValueCoordinates(Point pt)
{
	vector<double> values;
	double total = 0;
	for (size_t i = 0; i < source.patch.boundary.size(); ++i) {
		double weight = boundaryWeight(pt,i);
		total += weight;
		values.push_back(weight);
	}

	if (total == 0)
		total = 1;

	for (size_t i = 0; i < values.size(); ++i)
		values[i] = values[i] / total;

	return values;
}

double boundaryWeight(Point pt, int index)
{
	Point current = source.patch.boundary[index];
	Point previous;
	Point next;

	if (index == 0)
		previous = source.patch.boundary[source.patch.boundary.size() - 1];
	else
		previous = source.patch.boundary[index - 1];

	if (index == source.patch.boundary.size() - 1)
		next = source.patch.boundary[0];
	else
		next = source.patch.boundary[index + 1];

	double distance = dist(pt,current);

	double alpha0 = angle(pt, previous, current);
	double alpha1 = angle(pt, current, next);

	double tHalfAngle0 = tan(alpha0 / 2);
	double tHalfAngle1 = tan(alpha1 / 2);

	if (distance == 0)
		return 1;

	return ((tHalfAngle0 + tHalfAngle1) / (distance));
}

vector<Point> targetBoundary(Point start)
{
	vector<Point> bounds;
	for (size_t i = 0; i < source.patch.boundary.size(); ++i)
		bounds.push_back(translate(source.patch.boundary[0], start, source.patch.boundary[i]));
	return bounds;
}

vector<Pixel> boundaryDiff(Point start)
{
	vector<Pixel> differences;
	vector<Point> targetBounds = targetBoundary(start);
	
	assert(source.patch.boundary.size() == targetBounds.size());

	for (size_t i = 0; i < targetBounds.size(); ++i) {
		Point src = source.patch.boundary[i];
		Point dst = targetBounds[i];

		if (dst.x >= destination.width || dst.y >= destination.height)
			differences.push_back(Pixel());
		else
			differences.push_back(destination.originalImg->getPixel_(dst) - source.originalImg->getPixel_(src));
	}

	return differences;
}

Point translate(Point oldOrigin, Point newOrigin, Point pt)
{
	Point delta = pt - oldOrigin;

	return newOrigin + delta;
}