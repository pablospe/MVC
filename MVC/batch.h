#ifndef BATCH_H
#define BATCH_H

#include "common.h"
#include "image.h"

class Batch {
public:
	Batch();
	void init();
	void run();

	int counter;
	std::string srcFileBase;
	std::string dstFileBase;
	std::string saveFileBase;

};

#endif