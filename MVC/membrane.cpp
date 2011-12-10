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
	/***
	for (size_t i = 0; i < hierarchy.size(); ++i) {
		cout << "Size: " << hierarchy[i].size() << endl;
		for (size_t j = 0; j < hierarchy[i].size(); ++j) {
			cout << hierarchy[i][j] << "\t";
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
		//cout << "interior in" << endl;
		Point sourcePoint = source.patch.interior[i];
		Point targetPoint = translate(source.patch.boundary[0],start,sourcePoint);
		
		Pixel interpolant;
		assert(meanValues[i].first.size() == meanValues[i].second.size());
		for (size_t j = 0; j < meanValues[i].second.size(); ++j) {
			int absBoundary= meanValues[i].second[j];
			assert(absBoundary < diff.size());
			interpolant = interpolant + scale(diff[absBoundary], meanValues[i].first[j]);
		}
	
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
		//cout << "interior out" << endl;
	}
	history.push_back(membraneValues);

	glutPostRedisplay();
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

////////////////////////////////////////////

int Membrane::wrap(int index)
{
	int result;
	if (index >= int(hierarchy[0].size()))
		result = index - hierarchy[0].size();
	else if (index < 0)
		result = index + hierarchy[0].size();
	else
		result = index;
	return result;
}

void Membrane::buildBoundary(Point pt, int index, int step, int level, vector<int>& refBoundary)
{
	//cout << "build in" << endl;
	if (step == 1) {
		refBoundary.push_back(wrap(index));
	}

	else if (refinedEnough(pt, hierarchy[0][wrap(index)], hierarchy[0][wrap(index + step)], hierarchy[0][wrap(index - step)],level)) {
		refBoundary.push_back(wrap(index));
		//cout << "enough" << endl;
	}
	
	else {
		step /= 2;
		buildBoundary(pt,index - step, step, level-1, refBoundary);
		buildBoundary(pt,index, step, level-1, refBoundary);
		//buildBoundary(pt,index + step, step, level-1, refBoundary);
	}
	//cout << "build out" << endl;
}

vector<int> Membrane::refinedBoundary(Point pt)
{
	//cout << "refined in" << endl;
	
	vector<int> refBoundary;
	int totalLevels = hierarchy.size();
	int step = pow(double(2), double(totalLevels - 1));

	for (size_t i = 0; i < hierarchy[0].size(); i+=step) {
		assert(i < hierarchy[0].size());
		buildBoundary(pt,i,step,0,refBoundary);
	}
	///cout << "refined out" << endl;
	return refBoundary;
}

bool Membrane::refinedEnough(Point pt, Point vertex, Point next, Point prev, int level)
{
	double epsilonDist = double(hierarchy[0].size()) / double(16 * pow(2.5, level));
	double epsilonAngle = .75 * pow(.8, level);

	//return false;

	return (dist(pt, vertex)			> epsilonDist 
			&& angle(pt, prev, vertex)	< epsilonAngle 
			&& angle(pt, next, vertex)	< epsilonAngle);
}

Membrane::MVC Membrane::meanValueCoordinates(Point pt)
{
	//cout << "mvc in" << endl;
	MVC values;
	double total = 0;
	vector<int> refBoundary = refinedBoundary(pt);

	for (size_t i = 0; i < refBoundary.size(); ++i) {
		assert(refBoundary[i] < hierarchy[0].size());
		double weight = boundaryWeight(pt,refBoundary[i]);
		total += weight;
		values.first.push_back(weight);
		values.second.push_back(refBoundary[i]);
	}

	assert(values.first.size() == values.second.size());

	if (total == 0)
		total = 1;

	for (size_t i = 0; i < values.first.size(); ++i)
		values.first[i] = values.first[i] / total; 

	//cout << "mvc out" << endl;
	return values;
}