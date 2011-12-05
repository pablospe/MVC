#include "control.h"
#include "main.h"
#include "batch.h"
#include <stdlib.h>

using namespace std;

enum {
	M_QUIT = 0,
	M_HELP = 1,

	M_SRC_OPEN = 2,
	M_SRC_SAVE = 3,
	M_SRC_INFO = 4,
	M_SRC_REVERT = 5,

	M_DST_OPEN = 6,
	M_DST_SAVE = 7,
	M_DST_INFO = 8,
	M_DST_REVERT = 9,

	M_SRC_CCLONE = 10,
	M_SRC_CLEAR = 11,
	M_SRC_DCLONE = 12,

	M_DST_PASTE = 13,

	M_BATCH_INIT = 14,

	M_LAST_ENUM
} MENU_ITEMS;


int makeMenuSrc ()
{
	int file = glutCreateMenu(menuFunc);
	glutAddMenuEntry( "Open...",		M_SRC_OPEN);
	glutAddMenuEntry( "Save...",		M_SRC_SAVE);
	glutAddMenuEntry( "Get Source Image Info",		M_SRC_INFO);
	glutAddMenuEntry( "Revert",		M_SRC_REVERT);

	int clone = glutCreateMenu(menuFunc);
	glutAddMenuEntry( "Outline Path", M_SRC_DCLONE);
	glutAddMenuEntry( "Trace Path", M_SRC_CCLONE);
	glutAddMenuEntry( "Clear Path", M_SRC_CLEAR);

	int batch = glutCreateMenu(menuFunc);
	glutAddMenuEntry( "Run", M_BATCH_INIT);

	int main = glutCreateMenu(menuFunc);
	glutAddSubMenu(   "File",		file);
	glutAddSubMenu(   "Clone",		clone);
	glutAddSubMenu(   "Batch",		batch);
	glutAddMenuEntry( "Help",		M_HELP);
	glutAddMenuEntry( "Quit",		M_QUIT);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	return main;
}

int makeMenuDst ()
{
	int file = glutCreateMenu(menuFunc);
	glutAddMenuEntry( "Open...",		M_DST_OPEN);
	glutAddMenuEntry( "Save...",		M_DST_SAVE);
	glutAddMenuEntry( "Get Image Info",		M_DST_INFO);
	glutAddMenuEntry( "Revert",		M_DST_REVERT);

	int clone = glutCreateMenu(menuFunc);
	glutAddMenuEntry( "Paste Patch", M_DST_PASTE);

	int batch = glutCreateMenu(menuFunc);
	glutAddMenuEntry( "Run", M_BATCH_INIT);

	int main = glutCreateMenu(menuFunc);
	glutAddSubMenu(   "File",		file);
	glutAddSubMenu(   "Clone",		clone);
	glutAddSubMenu(   "Batch",		batch);
	glutAddMenuEntry( "Help",		M_HELP);
	glutAddMenuEntry( "Quit",		M_QUIT);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	return main;
}


void menuFunc (int value)
{
	// variables used in the switch statement
	char filename[MAX_LINE];

	switch (value)
	{
	case M_QUIT:
		exit(0);
		break;
		;;

	case M_HELP:
		menuHelp();
		break;
		;;

	// General i/o
	case M_SRC_OPEN:
		cout << "Open file (string - no spaces) : ";
		cin  >> filename;
		checkStream(cin);
		imageLoad(filename, source);
		endProcess();
		break;

	case M_SRC_SAVE:
		cout << "Save as (string - no spaces) : ";
		cin  >> filename;
		checkStream(cin);
		imageSave(filename, source);
		endProcess();
		break;

	case M_SRC_INFO:
		imagePrint(source);
		endProcess();
		break;

	case M_SRC_REVERT:
		source.patch.clear();
		imageRevert(source);
		endProcess();
		break;

	case M_DST_OPEN:
		cout << "Open file (string - no spaces) : ";
		cin  >> filename;
		checkStream(cin);
		imageLoad(filename, destination);
		endProcess();
		break;

	case M_DST_SAVE:
		cout << "Save as (string - no spaces) : ";
		cin  >> filename;
		checkStream(cin);
		imageSave(filename, destination);
		endProcess();	
		break;

	case M_DST_INFO:
		imagePrint(destination);
		endProcess();
		break;

	case M_DST_REVERT:
		imageRevert(destination);
		endProcess();
		break;

	// Cloning
	case M_SRC_DCLONE:
		initDiscreteClone();
		break;

	case M_SRC_CCLONE:
		initContinuousClone();
		break;

	case M_SRC_CLEAR:
		source.patch.clear();
		endProcess();
		break;

	// Pasting
	case M_DST_PASTE:
		destination.paste = true;
		cout << "Click on the point that will correspond ";
		cout << "to first point in source patch" << endl;
		break;

	// Batch
	case M_BATCH_INIT:
		runBatch();
		endProcess();
		break;
	}
}


void runBatch()
{
	if (source.currentImg == NULL || source.patch.empty() || !destination.pastePoint.valid()) {
		cerr << "You must first select a patch in the current source image and a point to paste in the current target image" << endl;
		return;
	}

	source.batch = true;
	destination.batch = true;

	Batch bat;
	bat.init(source.patch);

	size_t frames;
	cout << "Enter the number of frames to clone over: ";
	cin >> frames;
	checkStream(cin);

	MVC membrane(destination.pastePoint);

	for (size_t i = 0; i < frames; ++i)
		bat.run(membrane);

	source.batch = false;
	destination.batch = false;

	// Load last frame
	glutPostRedisplay();
}

bool checkSource()
{
	return (source.originalImg == NULL || source.currentImg == NULL ||
			source.originalImg->bad()  || source.currentImg->bad());
}

void initDiscreteClone()
{
	if (checkSource()) {
		cerr << "No image!" << endl; return;
	}

	cout << "Select vertices of patch. ";
	cout << "The polygon must not intersect itself. ";
	cout << "Close patch by selecting a pixel close to the original pixel or press 'c'. ";
	cout << "Undo a point by pressing 'z'." << endl;

	source.patch.clear();
	source.dClone = true;
}

void initContinuousClone()
{
	if (checkSource()) {
		cerr << "No image!" << endl; return; 
	}

	cout << "Click and hold mouse to trace patch. ";
	cout << "The polygon must not intersect itself. ";
	cout << "Close the patch by tracing back to the origin pixel or press 'c'." << endl;

	source.patch.clear();
	source.cClone = true;
}

void keyboardFunc (unsigned char key, int x, int y)
{
	switch (tolower(key))
	{
	case 'h':
		menuHelp();
		break;
		;;

	case 'q':
		exit(0);
		break;
		;;

	case 'z':
		undoPoint();
		break;

	case 'c':
		if (source.patch.boundary.size() >= 3)
			source.patch.closed();
		break;

	}
}

void undoPoint()
{
	if (source.dClone) {
		Point last = source.patch.boundary.back();
		source.currentImg->setPixel_(last,source.originalImg->getPixel_(last));
		source.patch.boundary.pop_back();
		cout << "Removing point at: " << last << endl;
		glutPostRedisplay();
	}
}

void mouseClickSrc (int button, int state, int x, int y)
{
	if (source.currentImg && source.dClone && !source.cClone) {

		if (button == GLUT_DOWN) {
			Point vertex(x,y);
			if (source.patch.addPoint(vertex))
				source.patch.closed();

			else
				for (int chn = RED; chn <= BLUE; ++chn)
					source.currentImg->setPixel_(vertex,chn, 1);
		}

		glutPostRedisplay();
	}
}

void mouseClickDst (int button, int state, int x, int y)
{
	if (destination.paste) {
		destination.pastePoint = Point(x,y);
		MVC membrane(Point(x,y));
		membrane.composite();
		destination.paste = false;
		endProcess();
	}
}

void motionSrc(int x, int y)
{
	if (source.currentImg != NULL && source.currentImg->good() && source.cClone) {

		Point vertex(x,y);

		if (source.patch.boundary.size() > 10 && source.patch.addPoint(vertex))
			source.patch.closed();

		else
			source.patch.addPoint(vertex);

		for (int chn = RED; chn <= BLUE; ++chn)
			source.currentImg->setPixel_(vertex,chn, 0);

		glutPostRedisplay();
	}
}

void menuHelp ()
{
	cout << "not implemented yet" << endl;
}


void imageLoad (const char* filename, Window& w)
{
	if (w.currentImg)
		delete w.currentImg;
	if (w.originalImg)
		delete w.originalImg;

	w.currentImg = NULL;
	w.originalImg = NULL;

	w.originalImg = new Image();
	w.originalImg->read(filename);

	if (w.originalImg->good())
	{  
		w.currentImg = new Image(*w.originalImg);
		w.update();
		reshape(w);
	}
	else
	{
		delete w.originalImg;  
		w.originalImg = NULL;
		cerr << "Couldn't load image " << filename << "!" << endl;
		return;
	}

	glutPostRedisplay();
}  


void imageSave (const char* filename, Window& w)
{
	if (w.currentImg)
	{
		if (w.currentImg->write(filename) == 0)
		{
			//delete originalImage;
			//originalImage = new Image(*currentImage);
		}
	}  
	else if (w.originalImg)
	{
		w.originalImg->write(filename);
	}
	else
	{
		cerr << "No image!" << endl;
		return;
	}
}


void imagePrint (Window& w)
{  
	if (w.currentImg) {
		// Note its possible the image size is not equal to the window
		// size if one dimension is smaller than 64)
		cout << "width:    " << w.currentImg->getWidth() << endl
			 << "height:   " << w.currentImg->getHeight() << endl
			 << "bits:     " << w.currentImg->getBits() << endl;
	}

	else
		cerr << "No image!" << endl;
}

void imageRevert (Window& w)
{
	if (w.patch.boundary.size() != 0)
		w.patch.clear();

	if (w.currentImg)
		delete w.currentImg;

	if (w.originalImg)
	{
		w.currentImg = new Image(*w.originalImg);

		if (w.width  != w.currentImg->getWidth() || w.height != w.currentImg->getHeight()) {
			w.update();		
			reshape(w);
		}
	}
	else
	{
		cerr << "No image!" << endl;
		return;
	}

	glutPostRedisplay();
}  

