#ifndef MAIN_H
#define MAIN_H

#include "control.h"
#include "common.h"
#include "image.h"
#include "patch.h"
#include "geometry.h"
#include "mvc.h"
#include "batch.h"

/***
 To Do
 - resolve problem with computing MVC using source patch,
 which will get cleared whenever a new image is loaded"
 ***/

extern Window source;
extern Window destination;

int  main (int argc, char** argv);
void display();
void unreshapeSrc (int width, int height);
void unreshapeDst (int width, int height);
void reshape (Window& w);


#endif // MAIN_H
