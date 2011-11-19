#ifndef MAIN_H
#define MAIN_H

#include "common.h"
#include "image.h"

struct Window {
	int width;
	int height;
	Image* original;
	Image* current;
	bool dst;
};


extern int windowSrc_width;
extern int windowSrc_height;
extern int windowDst_width;
extern int windowDst_height;

extern Image* currentDstImage;
extern Image* originalDstImage;

extern Image* currentSrcImage;
extern Image* originalSrcImage;



int  main (int argc, char** argv);
void display();
void unreshapeSrc (int width, int height);
void unreshapeDst (int width, int height);
void reshape (int width, int height, bool dst);



#endif // MAIN_H
