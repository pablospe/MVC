#include "batch.h"
#include "main.h"
#include "control.h"

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
	patch = p;
}

void Batch::run()
{
	
	++saveCounter;
}
