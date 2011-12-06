#include "batch.h"
#include "main.h"
#include "control.h"
#include <sstream>

using namespace std;

void Batch::init(Patch p)
{
	char fileName[MAX_LINE];
	size_t counter;

	cout << "Enter base of file name without index or extension for source images: ";
	cin  >> fileName;
	checkStream(cin);
	srcFileBase = fileName;

	cout << "Enter base of file name without index or extension for target images: ";
	cin  >> fileName;
	checkStream(cin);
	dstFileBase = fileName;

	cout << "Enter base of file name to save resulting images: ";
	cin  >> fileName;
	checkStream(cin);
	saveFileBase = fileName;

	cout << "Enter the starting index of the source images: ";
	cin >> counter;
	checkStream(cin);
	srcCounter = counter;

	cout << "Enter the starting index of the target images: ";
	cin >> counter;
	checkStream(cin);
	dstCounter = counter;

	saveCounter = 0;
}

void Batch::prepareFiles(string& srcLoad, string& dstLoad, string& resultSave)
{
	stringstream index;
	index << srcCounter + saveCounter;
	srcLoad = srcFileBase;
	srcFileBase.append(index.str()).append(".bmp");
	swap(srcLoad, srcFileBase);
	index.str("");

	index << dstCounter + saveCounter;
	dstLoad = dstFileBase;
	dstFileBase.append(index.str()).append(".bmp");
	swap(dstLoad, dstFileBase);
	index.str("");

	index << saveCounter;
	resultSave = saveFileBase;
	saveFileBase.append(index.str()).append(".bmp");
	swap(resultSave, saveFileBase);
}

void Batch::run(Membrane& membrane)
{
	string srcLoad, dstLoad, resultSave;
	prepareFiles(srcLoad, dstLoad, resultSave);

	imageLoad(srcLoad.c_str(), source);
	imageLoad(dstLoad.c_str(), destination);
	membrane.composite();
	imageSave(resultSave.c_str(), destination);

	++saveCounter;
}
