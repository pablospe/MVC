#include "control.h"
#include "main.h"
#include <stdlib.h>

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
		imageLoad(filename, source);
		break;

	case M_SRC_SAVE:   // enum #3
		cerr << "Save as (string - no spaces) : ";
		cin  >> filename;
		checkStream(cin);
		imageSave(filename, source);
		break;

	case M_SRC_INFO:
		imagePrint(source);
		break;

	case M_SRC_REVERT:
		source.patch.clear();
		imageRevert(source);
		break;

	case M_DST_OPEN:   // enum #2
		cerr << "Open file (string - no spaces) : ";
		cin  >> filename;
		checkStream(cin);
		imageLoad(filename, destination);
		break;

	case M_DST_SAVE:   // enum #3
		cerr << "Save as (string - no spaces) : ";
		cin  >> filename;
		checkStream(cin);
		imageSave(filename, destination);
		break;

	case M_DST_INFO:
		imagePrint(destination);
		break;

	case M_DST_REVERT:
		imageRevert(destination);
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
	if (destination.paste) {
		composite(Point(x,y));
		destination.paste = false;
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
	cerr << "done!" << endl;
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

	cerr << "done!" << endl;
}


void imagePrint (Window& w)
{  
	if (w.currentImg) {
		// Note its possible the image size is not equal to the window
		// size if one dimension is smaller than 64)
		cerr << "width:    " << w.currentImg->getWidth() << endl
			 << "height:   " << w.currentImg->getHeight() << endl
			 << "bits:     " << w.currentImg->getBits() << endl;
	}

	else
		cerr << "No image!" << endl;

	cerr << "done!" << endl;
}

void imageRevert (Window& w)
{
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

	cerr << "done!" << endl;
}  

