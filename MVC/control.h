#ifndef CONTROL_H
#define CONTROL_H

#include "common.h"
#include "image.h"
#include "patch.h"
#include "geometry.h"

// init calls
int makeMenuSrc();
int makeMenuDst();

// glut callbacks
void menuFunc (int value);
void keyboardFunc (unsigned char key, int x, int y);
void mouseClickSrc (int button, int state, int x, int y);
void mouseClickDst (int button, int state, int x, int y);
void motionSrc(int x, int y);

// menu calls
void menuHelp ();
void runBatch();

void imageLoad(const char* filename, Window& w);
void imageSave(const char* filename, Window& w);
void imagePrint(Window& w);
void imageRevert(Window& w);

// Helper
bool checkSource();
void undoPoint();
void initDiscreteClone();
void initContinuousClone();

void welcomeMessage();

static inline void checkStream (const std::istream& in)
{
	if (in.fail())
	{
		std::cerr << "Fatal error: stream failed!" << std::endl;
		exit(-1);
	}
}

static inline void endProcess()
{
	std::cout << "done!" << std::endl;
}

#endif // CONTROL_H
