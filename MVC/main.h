#ifndef MAIN_H
#define MAIN_H

#include "control.h"
#include "common.h"
#include "image.h"
#include "patch.h"
#include "geometry.h"
#include "membrane.h"
#include "batch.h"

/***
 To DO
 - incorporate boundary hierarchy
   in mvc calculation
***/

extern Window source;
extern Window destination;

int  main (int argc, char** argv);
void display();
void unreshapeSrc (int width, int height);
void unreshapeDst (int width, int height);
void reshape (Window& w);


#endif // MAIN_H
