#include "control.h"
#include "main.h"
#include <stdlib.h>
#include "image.h"


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

	M_LAST_ENUM
} MENU_ITEMS;


int make_menuSrc ()
{
	int file = glutCreateMenu(menu_func);
	glutAddMenuEntry( "Open...",		M_SRC_OPEN);
	glutAddMenuEntry( "Get Source Image Info",		M_SRC_INFO);
	glutAddMenuEntry( "Revert",		M_DST_REVERT);

	int main = glutCreateMenu(menu_func);
	glutAddSubMenu(   "File",		file);
	glutAddMenuEntry( "Help",		M_HELP);
	glutAddMenuEntry( "Quit",		M_QUIT);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	return main;
}

int make_menuDst ()
{
	int file = glutCreateMenu(menu_func);
	glutAddMenuEntry( "Open...",		M_DST_OPEN);
	glutAddMenuEntry( "Save...",		M_DST_SAVE);
	glutAddMenuEntry( "Get Image Info",		M_DST_INFO);
	glutAddMenuEntry( "Revert",		M_DST_REVERT);

	int main = glutCreateMenu(menu_func);
	glutAddSubMenu(   "File",		file);
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


void menu_func (int value)
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
		menu_help();
		break;
		;;


	case M_SRC_OPEN:   // enum #2
		cerr << "Open file (string - no spaces) : ";
		cin  >> filename;
		checkStream(cin);
		image_loadSrc(filename);
		break;

	case M_SRC_INFO:
		image_print_info(false);
		break;

	case M_SRC_REVERT:
		image_revertSrc();
		break;

	case M_DST_OPEN:   // enum #2
		cerr << "Open file (string - no spaces) : ";
		cin  >> filename;
		checkStream(cin);
		image_loadDst(filename);
		break;

	case M_DST_SAVE:   // enum #3
		cerr << "Save as (string - no spaces) : ";
		cin  >> filename;
		checkStream(cin);
		image_save(filename);
		break;

	case M_DST_INFO:
		image_print_info(true);
		break;

	case M_DST_REVERT:
		image_revertDst();
		break;

	default:
		process_func(value);
	}
	return;
}

// all other menu functions
void process_func (int value)
{
	cerr << "in process_func" <<  endl;
	// variables used in the switch statement
	char filename[MAX_LINE];
	Image* resultImage = NULL;
	Pixel thePixel;

	if (resultImage != NULL)
	{
		/***
		delete currentImage;
		currentImage = resultImage;

		if (currentImage->getWidth()  != window_width    ||
			currentImage->getHeight() != window_height)
			reshape(currentImage->getWidth(), currentImage->getHeight());

		cerr << "done!" << endl;

		glutPostRedisplay();
		***/
	}
}

void keyboard_func (unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'H':
	case 'h':
		menu_help();
		break;
		;;

	case 'Q':
	case 'q':
		exit(0);
		break;
		;;
	}
}


void menu_help ()
{
	cerr << "not implemented yet" << endl;
}

void image_loadSrc (const char* filename)
{
	image_load(filename, currentSrcImage, false);
}

void image_loadDst (const char* filename)
{
	image_load(filename, currentDstImage, true);
}


void image_load (const char* filename, Image* curr, bool dst)
{
	if (curr)
		delete curr;
	//if (orig)
	//	delete orig;
	curr = NULL;
	//orig = NULL;

	///orig = new Image();
	//orig->read(filename);

	//f (orig->good())
	{  
	//	curr = new Image(*orig);
	//	reshape(curr->getWidth(), curr->getHeight(), dst);
	}
	//else
	{
	//	delete orig;  
	//	orig = NULL;
		cerr << "Couldn't load image " << filename << "!" << endl;
		return;
	}

	glutPostRedisplay();
	cerr << "done!" << endl;
}  


void image_save (const char* filename)
{
	if (currentDstImage)
	{
		if (currentDstImage->write(filename) == 0)
		{
			//delete originalImage;
			//originalImage = new Image(*currentImage);
		}
	}  
	else if (originalDstImage)
	{
		originalDstImage->write(filename);
	}
	else
	{
		cerr << "No image!" << endl;
		return;
	}

	cerr << "done!" << endl;
}


void image_print_info (bool dst)
{  
	if (currentSrcImage && !dst) {
		cerr << "width:    " << currentSrcImage->getWidth() << endl
			<< "height:   " << currentSrcImage->getHeight() << endl
			<< "bits:     " << currentSrcImage->getBits() << endl;
	}

	else if (currentDstImage && dst) {
		cerr << "width:    " << currentDstImage->getWidth() << endl
			<< "height:   " << currentDstImage->getHeight() << endl
			<< "bits:     " << currentDstImage->getBits() << endl;
	}

	cerr << "done!" << endl;
}


void image_revertSrc()
{
	image_revert(currentSrcImage, windowSrc_width, windowSrc_height, false);
}

void image_revertDst()
{
	image_revert(currentDstImage, windowDst_width, windowDst_height, true);
}

void image_revert (Image* curr, int width, int height, bool dst)
{
	if (curr)
		delete curr;

	//if (orig)
	{
	//	curr = new Image(*orig);

		if (width  != curr->getWidth() ||
			height != curr->getHeight())
			reshape(curr->getWidth(), curr->getHeight(), dst);
	}
	//else
	{
		cerr << "No image!" << endl;
		return;
	}

	glutPostRedisplay();

	cerr << "done!" << endl;
}  

