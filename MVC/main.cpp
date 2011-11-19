#include "main.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "control.h"

int  window_width  = 300;
int  window_height = 300;

int glass_width = 300;
int glass_height = 300;

Image* currentImage  = NULL;
Image* originalImage = NULL;

int main (int argc, char** argv)
{
    // set up the window
    glutInit(&argc, &argv[0]); 
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(window_width, window_height);

	// SOURCE WINDOW
    int window1 = glutCreateWindow("Source");
 
    // register call back functions
    glutDisplayFunc(display);
    glutReshapeFunc(unreshape);

    glClearColor(0.0,0.0,0.0,0.0);
    glDisable(GL_DEPTH_TEST);

    // setup main menu
    make_menu();

    // register keyboard callback function
    glutKeyboardFunc(keyboard_func);
	// END SOURCE WINDOW

	// TARGET WINDOW
	glutInitWindowPosition(500,100);
	int window2 = glutCreateWindow("Target");
	// register call back functions
    glutDisplayFunc(display);
    glutReshapeFunc(unreshape);

    glClearColor(0.0,0.0,0.0,0.0);
    glDisable(GL_DEPTH_TEST);

    // setup main menu
    make_menuR();

    // register keyboard callback function
    glutKeyboardFunc(keyboard_func);
	// END TARGET WINDOW

    // wait for something to happen
    glutMainLoop();
}


void display ()
{
  // check if there have been any openGL problems
  GLenum errCode = glGetError();
  if (errCode != GL_NO_ERROR) 
  {
      const GLubyte* errString = gluErrorString(errCode);
      cerr << "OpenGL error: " << errString << endl;
  }

  // clear the frame buffer
  glClear(GL_COLOR_BUFFER_BIT);

  // draw the image
  if (currentImage)
    currentImage->glDrawPixelsWrapper();

  // swap buffers
  glutSwapBuffers();  
}


void unreshape (int width, int height)
{
  // don't allow user to manually resize the window
  reshape(window_width, window_height);
}

void reshape (int width, int height) 
{
  // set window height and width
  window_width  = max(width,  64);
  window_height = max(height, 64); 

  // change the actual window's size
  glutReshapeWindow(window_width, window_height);

  // the lower left corner of the viewport is 0,0
  // the upper right corner is width, height
  glViewport(0, 0, (GLint) window_width, (GLint) window_height);  

  // setup orthographic projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, window_width, 0.0, window_height);

  // default mode should be modelview
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

