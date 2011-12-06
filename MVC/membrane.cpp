#include "membrane.h"
#include "main.h"

using namespace std;

Membrane::Membrane(Point Start)
:	start(Start)
{
	boundaryHierarchy();

	history.reserve(8);
	meanValues.reserve(source.patch.interior.size());
	for (size_t i = 0; i < source.patch.interior.size(); ++i)
		meanValues.push_back(meanValueCoordinates(source.patch.interior[i]));
}

void Membrane::composite()
{
	/*** Code for testing boundary levels
	for (size_t i = 0; i < hier.size(); ++i) {
		for (size_t j = 0; j < hier[i].size(); ++j) {
			cout << hier[i][j] << "\t";
			if (j%2 == 1)
				cout << endl;
		}
		cout << endl;
		cout << endl;
	}
	***/

	vector<Pixel> diff = boundaryDiff();

	vector<Pixel> membraneValues;
	membraneValues.reserve(source.patch.interior.size());
	
	for (size_t i = 0; i < source.patch.interior.size(); ++i) {
		Point sourcePoint = source.patch.interior[i];
		Point targetPoint = translate(source.patch.boundary[0],start,sourcePoint);

		assert(meanValues[i].size() ==  diff.size()); // .first.
		
		Pixel interpolant;
		for (size_t j = 0; j < diff.size(); ++j)		
			interpolant = interpolant + scale(diff[j], meanValues[i][j]); // .first.
		
		double timeWeight = 1;
		for (size_t j = 0; j < history.size(); ++ j) {
			int dT =  history.size() - j + 1;
			double weight = pow(double(dT), -0.75);
			timeWeight += weight;
			
			// j-index is history; i-index is point in interior
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

//MVC Membrane::meanValueCoordinates(Point pt)
vector<double> Membrane::meanValueCoordinates(Point pt)
{
	// MVC values;
	vector<double> values;
	double total = 0;
	for (size_t i = 0; i < source.patch.boundary.size(); ++i) {
		double weight = boundaryWeight(pt,i);
		total += weight;
		values.push_back(weight); //.first.
	}

	if (total == 0)
		total = 1;

	for (size_t i = 0; i < values.size(); ++i)
		values[i] = values[i] / total; // .first.

	return values;
}

double Membrane::boundaryWeight(Point pt, int index)
{
	Point current = source.patch.boundary[index];
	Point previous;
	Point next;

	// Wrap-arounds for boundary indices
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

vector<Point> Membrane::targetBoundary()
{
	vector<Point> bounds;
	for (size_t i = 0; i < source.patch.boundary.size(); ++i)
		bounds.push_back(translate(source.patch.boundary[0], start, source.patch.boundary[i]));
	return bounds;
}

vector<Pixel> Membrane::boundaryDiff()
{
	vector<Pixel> differences;
	vector<Point> targetBounds = targetBoundary();
	
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

void Membrane::boundaryHierarchy()
{
	hierarchy.clear();
	hierarchy.reserve(log(double(source.patch.boundary.size())) / log(double(2)));

	hierarchy.push_back(source.patch.boundary);

	while (hierarchy.back().size() >= 16) {
		vector<Point> sublevel;
		sublevel.reserve(log(double(hierarchy.back().size())) / log(double(2)));
		for (size_t i = 0; i < hierarchy.back().size(); i += 2)
			sublevel.push_back(hierarchy.back()[i]);

		hierarchy.push_back(sublevel);
	}
}