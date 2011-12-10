#include "main.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

Window source;
Window destination;

int main(int argc, char** argv)
{
	welcomeMessage();
	cout << "Press 'h' for help\n\n";
	// set up the window
	glutInit(&argc, &argv[0]); 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	// source window
	glutInitWindowPosition(100,400);
	glutInitWindowSize(source.width, source.height);
	source.glNum = glutCreateWindow("Source");
	glutDisplayFunc(display);
	glutReshapeFunc(unreshapeSrc);
	glClearColor(0.0,0.0,0.0,0.0);
	glDisable(GL_DEPTH_TEST);
	makeMenuSrc();
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseClickSrc);
	glutMotionFunc(motionSrc);

	// target window
	glutInitWindowPosition(500,400);
	glutInitWindowSize(destination.width, destination.height);
	destination.glNum = glutCreateWindow("Target");
	glutDisplayFunc(display);
	glutReshapeFunc(unreshapeDst);
	glClearColor(0.0,0.0,0.0,0.0);
	glDisable(GL_DEPTH_TEST);
	makeMenuDst();
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseClickDst);

	imageLoad("../cat.bmp", source);
	imageLoad("../test.bmp", destination);

	glutMainLoop();
}


void display()
{
	// check if there have been any openGL problems
	GLenum errCode = glGetError();
	if (errCode != GL_NO_ERROR) 
	{
		const GLubyte* errString = gluErrorString(errCode);
		cerr << "OpenGL error: " << errString << endl;
	}

	// clear the frame buffer
	glutSetWindow(source.glNum);
	glClear(GL_COLOR_BUFFER_BIT);

	glutSetWindow(destination.glNum);
	glClear(GL_COLOR_BUFFER_BIT);

	// draw the image
	if (source.currentImg) {
		glutSetWindow(source.glNum);
		source.currentImg->glDrawPixelsWrapper();
	}

	if (destination.currentImg){ 
		glutSetWindow(destination.glNum);
		destination.currentImg->glDrawPixelsWrapper();
	}

	// swap buffers
	glutSwapBuffers();

	// swap buffers
	glutSetWindow(source.glNum);
	glutSwapBuffers();
}


void unreshapeSrc(int width, int height)
{
	reshape(source);
}

void unreshapeDst(int width, int height)
{
	reshape(destination);
}

void reshape(Window& w) 
{
	int window_height, window_width;

	// set window height and width
	window_width  = max(w.width,  64);
	window_height = max(w.height, 64); 

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

	w.height = window_height;
	w.width = window_width;
}

