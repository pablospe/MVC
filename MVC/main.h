#ifndef MAIN_H
#define MAIN_H

#include "common.h"
#include "image.h"
#include "patch.h"
#include "window.h"

extern Window source;
extern Window destination;

int  main (int argc, char** argv);
void display();
void unreshapeSrc (int width, int height);
void unreshapeDst (int width, int height);
void reshape (int width, int height, bool dst);



#endif // MAIN_H
