#include "main.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "control.h"

int  windowSrc_width  = 300;
int  windowSrc_height = 300;

int windowDst_width = 300;
int windowDst_height = 300;

Image* currentSrcImage = NULL;
Image* currentDstImage = NULL;
Image* originalDstImage = NULL;

int main (int argc, char** argv)
{
	// set up the window
	glutInit(&argc, &argv[0]); 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(windowSrc_width, windowSrc_height);

	// SOURCE WINDOW
	int windowSrc = glutCreateWindow("Source");

	// register call back functions
	glutDisplayFunc(display);
	glutReshapeFunc(unreshapeSrc);

	glClearColor(0.0,0.0,0.0,0.0);
	glDisable(GL_DEPTH_TEST);

	// setup main menu
	make_menuSrc();

	// register keyboard callback function
	glutKeyboardFunc(keyboard_func);
	// END SOURCE WINDOW

	// TARGET WINDOW
	glutInitWindowPosition(500,100);
	glutInitWindowSize(windowSrc_width, windowSrc_height);
	int windowDst = glutCreateWindow("Target");
	// register call back functions
	glutDisplayFunc(display);
	glutReshapeFunc(unreshapeDst);

	glClearColor(0.0,0.0,0.0,0.0);
	glDisable(GL_DEPTH_TEST);

	// setup main menu
	make_menuDst();

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
	if (currentSrcImage)
		currentSrcImage->glDrawPixelsWrapper();

	if (currentDstImage)
		currentDstImage->glDrawPixelsWrapper();

	// swap buffers
	glutSwapBuffers();  
}


void unreshapeSrc (int width, int height)
{
	reshape(windowSrc_width, windowSrc_height, false);
}

void unreshapeDst (int width, int height)
{
	reshape(windowDst_width, windowDst_height, true);
}

void reshape (int width, int height, bool dst) 
{
	int window_height, window_width;

	if (dst) {
		window_height = windowDst_height;
		window_width = windowDst_width;
	}

	else {
		window_height = windowSrc_height;
		window_width = windowSrc_width;
	}


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


	if (dst) {
		windowDst_height = window_height;
		windowDst_width = window_width;
	}

	else {
		windowSrc_height = window_height;
		windowSrc_width = window_width;
	}
}

