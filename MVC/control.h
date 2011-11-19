#ifndef CONTROL_H
#define CONTROL_H

#include "common.h"
#include "image.h"


// init calls
int make_menuSrc();
int make_menuDst();

// glut callbacks
void menu_func (int value);
void keyboard_func (unsigned char key, int x, int y);

/*** unused
void mouse_click_func (int button, int state, int x, int y);
void mouse_move_func (int x, int y);
void idle_func ();
***/

// menu calls
void menu_help ();

void image_loadSrc (const char* filename);
void image_loadDst (const char* filename);
void image_load (const char* filename, Image*& orig, Image*& curr, bool dst);

void image_save (const char* filename);
void image_print_info (bool dst);

void image_revertSrc ();
void image_revertDst ();
void image_revert (Image*& orig, Image*& curr, int width, int height, bool dst);
void process_func (int value);


#endif // CONTROL_H
