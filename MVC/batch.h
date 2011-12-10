#ifndef BATCH_H
#define BATCH_H

#include "common.h"
#include "image.h"
#include "patch.h"
#include "membrane.h"

class Batch {
public:
	void init(Patch p);
	void run(Membrane& membrane);

private:
	void prepareFiles(std::string& srcLoad, 
					  std::string& dstLoad, 
					  std::string& resultSave);

	// Data members;
	int srcCounter;
	int dstCounter;
	int saveCounter;
	std::string srcFileBase;
	std::string dstFileBase;
	std::string saveFileBase;

};

#endif