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

void runBatch();
void undoPoint();
bool checkSource();

static inline void checkStream (const istream& in)
{
	if (in.fail())
	{
		cerr << "Fatal error: stream failed!" << endl;
		exit(-1);
	}
}
/***
void idle_func ();
void process_func (int value);
***/

// menu calls
void menuHelp ();

void imageLoad(const char* filename, Window& w);
void imageSave(const char* filename, Window& w);
void imagePrint(Window& w);
void imageRevert(Window& w);

void initDiscreteClone();
void initContinuousClone();

#endif // CONTROL_H
