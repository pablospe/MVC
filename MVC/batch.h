#ifndef BATCH_H
#define BATCH_H

#include "common.h"
#include "image.h"
#include "patch.h"

class Batch {
public:
	void init();
	void runFirst();
	void run();

	int srcCounter;
	int dstCounter;
	int saveCounter;
	std::string srcFileBase;
	std::string dstFileBase;
	std::string saveFileBase;

};

#endif