#include "control.h"
#include "main.h"
#include "window.h"
#include <stdlib.h>

enum {
	M_QUIT = 0,
	M_HELP = 1,

	M_SRC_OPEN = 2,
	M_SRC_INFO = 3,
	M_SRC_REVERT = 4,

	M_DST_OPEN = 5,
	M_DST_SAVE = 6,
	M_DST_INFO = 7,
	M_DST_REVERT = 8,

	M_SRC_CCLONE = 9,
	M_SRC_CLEAR = 10,
	M_SRC_DCLONE = 11,

	M_DST_PASTE = 12,

	M_LAST_ENUM
} MENU_ITEMS;


int makeMenuSrc ()
{
	int file = glutCreateMenu(menuFunc);
	glutAddMenuEntry( "Open...",		M_SRC_OPEN);
	glutAddMenuEntry( "Get Source Image Info",		M_SRC_INFO);
	glutAddMenuEntry( "Revert",		M_SRC_REVERT);

	int clone = glutCreateMenu(menuFunc);
	glutAddMenuEntry( "Outline Path", M_SRC_DCLONE);
	glutAddMenuEntry( "Trace Path", M_SRC_CCLONE);
	glutAddMenuEntry( "Clear Path", M_SRC_CLEAR);

	int main = glutCreateMenu(menuFunc);
	glutAddSubMenu(   "File",		file);
	glutAddSubMenu(   "Clone",		clone);
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

	int main = glutCreateMenu(menuFunc);
	glutAddSubMenu(   "File",		file);
	glutAddSubMenu(   "Clone",		clone);
	glutAddMenuEntry( "Help",		M_HELP);
	glutAddMenuEntry( "Quit",		M_QUIT);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	return main;
}


static inline void checkStream (const istream& in)
{
	if (in.fail())
	{
		cerr << "Fatal error: stream failed!" << endl;
		exit(-1);
	}
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

	case M_SRC_OPEN:   // enum #2
		cerr << "Open file (string - no spaces) : ";
		cin  >> filename;
		checkStream(cin);
		imageLoadSrc(filename);
		break;

	case M_SRC_INFO:
		imagePrint(false);
		break;

	case M_SRC_REVERT:
		source.patch.clear();
		imageRevertSrc();
		break;

	case M_DST_OPEN:   // enum #2
		cerr << "Open file (string - no spaces) : ";
		cin  >> filename;
		checkStream(cin);
		imageLoadDst(filename);
		break;

	case M_DST_SAVE:   // enum #3
		cerr << "Save as (string - no spaces) : ";
		cin  >> filename;
		checkStream(cin);
		imageSave(filename);
		break;

	case M_DST_INFO:
		imagePrint(true);
		break;

	case M_DST_REVERT:
		imageRevertDst();
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
		break;

	// Pasting

	case M_DST_PASTE:
		destination.paste = true;
		break;
	
	}

	return;
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
	cout << "Close the patch by selecting a pixel close to the original pixel or press 'c'. ";
	cout << "Undo a point by pressing 'z'. \n" << endl;

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
	cout << "Close the patch by tracing back to the origin pixel or press 'c'. \n" << endl;

	source.patch.clear();
	source.cClone = true;
}

/***
void process_func (int value)
{
	cerr << "in process_func" <<  endl;
	// variables used in the switch statement
	char filename[MAX_LINE];
	Image* resultImage = NULL;
	Pixel thePixel;

	if (resultImage != NULL)
	{
		delete currentImage;
		currentImage = resultImage;

		if (currentImage->getWidth()  != window_width    ||
			currentImage->getHeight() != window_height)
			reshape(currentImage->getWidth(), currentImage->getHeight());

		cerr << "done!" << endl;

		glutPostRedisplay();
	}
}
***/

void keyboardFunc (unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'H':
	case 'h':
		menuHelp();
		break;
		;;

	case 'Q':
	case 'q':
		exit(0);
		break;
		;;

	case 'z':
		undoPoint();
		break;

	case 'c':
		source.patch.closed();
		break;

	}
}

void undoPoint()
{
	if (source.dClone) {
		int xLast = source.patch.boundary.back().x;
		int yLast = source.patch.boundary.back().y;
		source.currentImg->setPixel_(xLast,yLast,source.originalImg->getPixel_(xLast,yLast));
		source.patch.boundary.pop_back();
		cerr << "Removing point at: " << xLast << " " << yLast << endl;
		glutPostRedisplay();
	}
}

void mouseClickSrc (int button, int state, int x, int y)
{
	if (source.currentImg && source.dClone && !source.cClone) {

		if (button == GLUT_DOWN) {
			static Point lastDPoint(0,0);
			Point vertex(x,y);
			if (lastDPoint != vertex && source.patch.addPoint(vertex))
				source.patch.closed();

			else
				for (int chn = RED; chn <= BLUE; ++chn)
					source.currentImg->setPixel_(x,y,chn, 1);

			lastDPoint = vertex;
		}

		glutPostRedisplay();
	}
}

void mouseClickDst (int button, int state, int x, int y)
{
	cerr << x << " " << y << endl;

	if (destination.paste) {
		// pasting code here!

	}
}

void motionSrc(int x, int y)
{
	if (source.currentImg != NULL && source.currentImg->good() && source.cClone) {

		static Point lastCPoint(0,0);
		Point vertex(x,y);

		if (vertex != lastCPoint && source.patch.boundary.size() > 10 && source.patch.addPoint(vertex))
			source.patch.closed();

		else if (vertex != lastCPoint)
			source.patch.addPoint(vertex);

		for (int chn = RED; chn <= BLUE; ++chn)
			source.currentImg->setPixel_(x,y,chn, 0);

		lastCPoint = vertex;
		glutPostRedisplay();
	}
}

void menuHelp ()
{
	cerr << "not implemented yet" << endl;
}

void imageLoadSrc (const char* filename)
{
	imageLoad(filename, source.originalImg, source.currentImg, false);

	source.patch.init(source.originalImg, source.currentImg);
}

void imageLoadDst (const char* filename)
{
	imageLoad(filename, destination.originalImg, destination.currentImg, true);
}


void imageLoad (const char* filename, Image*& orig, Image*& curr, bool dst)
{
	if (curr)
		delete curr;
	if (orig)
		delete orig;
	curr = NULL;
	orig = NULL;

	orig = new Image();
	orig->read(filename);

	if (orig->good())
	{  
		curr = new Image(*orig);
		reshape(curr->getWidth(), curr->getHeight(), dst);
	}
	else
	{
		delete orig;  
		orig = NULL;
		cerr << "Couldn't load image " << filename << "!" << endl;
		return;
	}

	glutPostRedisplay();
	cerr << "done!" << endl;
}  


void imageSave (const char* filename)
{
	if (destination.currentImg)
	{
		if (destination.currentImg->write(filename) == 0)
		{
			//delete originalImage;
			//originalImage = new Image(*currentImage);
		}
	}  
	else if (destination.originalImg)
	{
		destination.originalImg->write(filename);
	}
	else
	{
		cerr << "No image!" << endl;
		return;
	}

	cerr << "done!" << endl;
}


void imagePrint (bool dst)
{  
	if (source.currentImg && !dst) {
		cerr << "width:    " << source.currentImg->getWidth() << endl
			<< "height:   " << source.currentImg->getHeight() << endl
			<< "bits:     " << source.currentImg->getBits() << endl;
	}

	else if (destination.currentImg && dst) {
		cerr << "width:    " << destination.currentImg->getWidth() << endl
			<< "height:   " << destination.currentImg->getHeight() << endl
			<< "bits:     " << destination.currentImg->getBits() << endl;
	}

	cerr << "done!" << endl;
}


void imageRevertSrc()
{
	imageRevert(source.originalImg, source.currentImg, source.height, source.height, false);
}

void imageRevertDst()
{
	imageRevert(destination.originalImg, destination.currentImg, destination.width, destination.height, true);
}

void imageRevert (Image*& orig, Image*& curr, int width, int height, bool dst)
{
	
	if (curr)
		delete curr;

	if (orig)
	{
		curr = new Image(*orig);

		if (width  != curr->getWidth() ||
			height != curr->getHeight())
			reshape(curr->getWidth(), curr->getHeight(), dst);
	}
	else
	{
		cerr << "No image!" << endl;
		return;
	}

	glutPostRedisplay();

	cerr << "done!" << endl;
}  

