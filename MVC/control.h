#ifndef CONTROL_H
#define CONTROL_H

#include "common.h"
#include "image.h"


// init calls
int makeMenuSrc();
int makeMenuDst();

// glut callbacks
void menuFunc (int value);
void keyboardFunc (unsigned char key, int x, int y);
void mouseClickSrc (int button, int state, int x, int y);
void mouseClickDst (int button, int state, int x, int y);
void motionSrc(int x, int y);

void undoPoint();
bool checkSource();

/***
void mouse_move_func (int x, int y);
void idle_func ();
void process_func (int value);
***/

// menu calls
void menuHelp ();

void imageLoadSrc(const char* filename);
void imageLoadDst(const char* filename);
void imageLoad(const char* filename, Image*& orig, Image*& curr, bool dst);

void imageSave(const char* filename);
void imagePrint(bool dst);

void imageRevertSrc();
void imageRevertDst();
void imageRevert(Image*& orig, Image*& curr, int width, int height, bool dst);

void initDiscreteClone();
void initContinuousClone();

#endif // CONTROL_H
