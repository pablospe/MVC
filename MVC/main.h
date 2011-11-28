#ifndef MAIN_H
#define MAIN_H


#include "common.h"
#include "image.h"
#include "patch.h"

extern int windowWidthSrc;
extern int windowHeightSrc;
extern int windowWidthDst;
extern int windowHeightDst;

extern Image* currentSrcImage;
extern Image* originalSrcImage;

extern Image* currentDstImage;
extern Image* originalDstImage;

extern Patch srcPatch;
extern bool discreteCloningSrc;
extern bool contCloningSrc;

int  main (int argc, char** argv);
void display();
void unreshapeSrc (int width, int height);
void unreshapeDst (int width, int height);
void reshape (int width, int height, bool dst);



#endif // MAIN_H
