#include "main.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "control.h"

int  windowWidthSrc  = 300;
int  windowHeightSrc = 300;

int windowWidthDst = 300;
int windowHeightDst = 300;

Image* currentSrcImage = NULL;
Image* currentDstImage = NULL;
Image* originalDstImage = NULL;
Image* originalSrcImage = NULL;

Patch srcPatch;
bool cloningSrc = false;

int windowSrc;
int windowDst;


int main (int argc, char** argv)
{
	// set up the window
	glutInit(&argc, &argv[0]); 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	// SOURCE WINDOW
	glutInitWindowPosition(100,400);
	glutInitWindowSize(windowWidthSrc, windowHeightSrc);
	windowSrc = glutCreateWindow("Source");
	glutDisplayFunc(display);
	glutReshapeFunc(unreshapeSrc);
	glClearColor(0.0,0.0,0.0,0.0);
	glDisable(GL_DEPTH_TEST);
	makeMenuSrc();
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseClickSrc);
	// END SOURCE WINDOW

	// TARGET WINDOW
	glutInitWindowPosition(500,400);
	glutInitWindowSize(windowWidthSrc, windowHeightSrc);
	windowDst = glutCreateWindow("Target");
	glutDisplayFunc(display);
	glutReshapeFunc(unreshapeDst);
	glClearColor(0.0,0.0,0.0,0.0);
	glDisable(GL_DEPTH_TEST);
	makeMenuDst();
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseClickDst);
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
	glutSetWindow(windowSrc);
	glClear(GL_COLOR_BUFFER_BIT);

	glutSetWindow(windowDst);
	glClear(GL_COLOR_BUFFER_BIT);


	// draw the image
	if (currentSrcImage) {
		glutSetWindow(windowSrc);
		currentSrcImage->glDrawPixelsWrapper();
	}

	if (currentDstImage){ 
		glutSetWindow(windowDst);
		currentDstImage->glDrawPixelsWrapper();
	}

	// swap buffers
	glutSwapBuffers();


	// swap buffers
	glutSetWindow(windowSrc);
	glutSwapBuffers();
}


void unreshapeSrc (int width, int height)
{
	reshape(windowWidthSrc, windowHeightSrc, false);
}

void unreshapeDst (int width, int height)
{
	reshape(windowWidthDst, windowHeightDst, true);
}

void reshape (int width, int height, bool dst) 
{
	int window_height, window_width;

	if (dst) {
		window_height = windowHeightDst;
		window_width = windowWidthDst;
	}

	else {
		window_height = windowHeightSrc;
		window_width = windowWidthSrc;
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
		windowHeightDst = window_height;
		windowWidthDst = window_width;
	}

	else {
		windowHeightSrc = window_height;
		windowWidthSrc = window_width;
	}
}

