#ifndef MAIN_H
#define MAIN_H


#include "common.h"
#include "image.h"

extern int window_width;
extern int window_height;

extern Image* currentImage;
extern Image* originalImage;

int  main (int argc, char** argv);
void display ();
void unreshape (int width, int height);
void reshape (int width, int height);



#endif // MAIN_H
