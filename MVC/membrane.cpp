#include "membrane.h"
#include "main.h"

using namespace std;

Membrane::Membrane(Point Start)
:	start(Start)
{
	boundary = &(source.patch.boundary);
	interior = &(source.patch.interior);
	computeBoundaryLevels();
	computeThresholdLevels();
	history.reserve(8);
	
	meanValues.reserve(interior->size());
	for (size_t i = 0; i < interior->size(); ++i)
		meanValues.push_back(meanValueCoordinates((*interior)[i]));
}

// Initialization Functions
void Membrane::computeBoundaryLevels()
{
	boundaryLevels = 0;
	int boundarySize = boundary->size();
	while (boundarySize > 16) {
		++boundaryLevels;
		boundarySize = ceil(double(boundarySize) / 2.0);
	}
}

void Membrane::computeThresholdLevels()
{
	thresholds.clear();
	for (size_t i = 0; i < boundaryLevels + 1; ++i)
		thresholds.push_back(pair<double,double>(double(boundary->size()) / double(16 * pow(2.5, int(i))),
												 .75 * pow(.8, int(i))));
}

// Top-level algorithm
void Membrane::composite()
{
	vector<Pixel> diff = differenceBoundary();
	vector<Pixel> membraneValues;
	membraneValues.reserve(interior->size());
	
	// Loop over interior
	for (size_t i = 0; i < interior->size(); ++i) {
		Point sourcePoint = (*interior)[i];
		Point targetPoint = translate((*boundary)[0],start,sourcePoint);

		Pixel interpolant;
		// Loop over boundary
		for (size_t j = 0; j < meanValues[i].second.size(); ++j)
			interpolant = interpolant + scale(diff[(meanValues[i].second[j])], meanValues[i].first[j]);
	
		double timeWeight = 1;
		// Loop over history (for batch)
		for (size_t j = 0; j < history.size(); ++ j) {
			int dT =  history.size() - j + 1;
			double weight = pow(double(dT), -0.75);
			timeWeight += weight;
			interpolant = interpolant + scale(history[j][i], weight);
		}
		interpolant = scale(interpolant, 1/timeWeight);

		Pixel result = interpolant + source.originalImg->getPixel_(sourcePoint);
		destination.currentImg->setPixel_(targetPoint, result);
		membraneValues.push_back(interpolant);
	}
	history.push_back(membraneValues);
	glutPostRedisplay();
}

// Helper functions
double Membrane::boundaryWeight(Point pt, int localIndex, vector<size_t>& refBoundary)
{
	Point current = (*boundary)[refBoundary[localIndex]];
	Point previous;
	Point next;

	// Wrap-arounds for boundary indices
	if (localIndex == 0)
		previous = (*boundary)[ refBoundary[refBoundary.size() - 1] ];
	else
		previous = (*boundary)[ refBoundary[localIndex - 1] ];

	if (localIndex == refBoundary.size() - 1)
		next = (*boundary)[ refBoundary[0] ];
	else
		next = (*boundary)[ refBoundary[localIndex + 1] ];

	double distance = dist(pt,current);

	double alpha0 = angle(pt, previous, current);
	double alpha1 = angle(pt, current, next);

	double tHalfAngle0 = tan(alpha0 / 2);
	double tHalfAngle1 = tan(alpha1 / 2);

	if (distance == 0)
		return 1;

	return ((tHalfAngle0 + tHalfAngle1) / (distance));
}

vector<Point> Membrane::targetBoundary()
{
	vector<Point> bounds;
	for (size_t i = 0; i < boundary->size(); ++i)
		bounds.push_back(translate((*boundary)[0], start, (*boundary)[i]));
	return bounds;
}

vector<Pixel> Membrane::differenceBoundary()
{
	vector<Pixel> differences;
	vector<Point> targetBounds = targetBoundary();
	
	assert(boundary->size() == targetBounds.size());

	for (size_t i = 0; i < targetBounds.size(); ++i) {
		Point src = (*boundary)[i];
		Point dst = targetBounds[i];

		if (dst.x >= destination.width || dst.y >= destination.height)
			differences.push_back(Pixel());
		else
			differences.push_back(destination.originalImg->getPixel_(dst) - source.originalImg->getPixel_(src));
	}

	return differences;
}

int Membrane::wrap(int index)
{
	if (index >= int(boundary->size()))
		return index - boundary->size();
	else if (index < 0)
		return index + boundary->size();
	else
		return index;
}

void Membrane::computeRefinedBoundary(Point pt, int index, int step, int level, vector<size_t>& refBoundary)
{
	if (step == 1)
		refBoundary.push_back(wrap(index));

	else if (refinedEnough(pt, (*boundary)[wrap(index)],(*boundary)[wrap(index + step)], (*boundary)[wrap(index - step)],level))
		refBoundary.push_back(wrap(index));
	
	else {
		step /= 2;
		computeRefinedBoundary(pt,index - step, step, level+1, refBoundary);
		computeRefinedBoundary(pt,index, step, level+1, refBoundary);
	}
}


vector<size_t> Membrane::refinedBoundary(Point pt)
{
	vector<size_t> refBoundary;
	int step = pow(double(2), double(boundaryLevels));

	for (size_t i = 0; i < boundary->size(); i+=step)
		computeRefinedBoundary(pt,i,step,0,refBoundary);

	return refBoundary;
}

bool Membrane::refinedEnough(Point pt, Point vertex, Point next, Point prev, int level)
{
	return (dist(pt, vertex)			> thresholds[level].first 
			&& angle(pt, prev, vertex)	< thresholds[level].second 
			&& angle(pt, next, vertex)	< thresholds[level].second);
}


Membrane::Mvc Membrane::meanValueCoordinates(Point pt)
{
	Mvc values;
	double total = 0;
	vector<size_t> refBoundary = refinedBoundary(pt);

	for (size_t i = 0; i < refBoundary.size(); ++i) {
		assert(refBoundary[i] < boundary->size());
		double weight = boundaryWeight(pt,i,refBoundary);
		total += weight;
		values.first.push_back(weight);
		values.second.push_back(refBoundary[i]);
	}

	assert(values.first.size() == values.second.size());

	if (total == 0)
		total = 1;

	for (size_t i = 0; i < values.first.size(); ++i)
		values.first[i] = values.first[i] / total; 

	return values;
}