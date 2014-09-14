//
// This code was created by Jeff Molofee '99 (ported to Linux/GLUT by Richard Campbell '99)
//
// If you've found this code useful, please let me know.
//
// Visit me at www.demonews.com/hosted/nehe 
// (email Richard Campbell at ulmont@bellsouth.net)
//
#include "lesson13.h"
#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <GL/glx.h>     // Header file fot the glx libraries.
#include <unistd.h>     // Header file for sleeping.
#include <math.h>       // Header file for sin/cos functions.
#include <stdio.h>      // Header file for printf debugging.
#include <string.h>

/* ascii code for the escape key */
#define ESCAPE 27
    int window; 
    GLuint base;            // base display list for the font set.
    GLfloat cnt1;           // 1st counter used to move text & for coloring.
    GLfloat cnt2; 

/* The number of our GLUT window */          // 2nd counter used to move text & for coloring.

void lesson13::BuildFont() 
{
    Display *dpy;
    XFontStruct *fontInfo;  // storage for our font.

    base = glGenLists(96);                      // storage for 96 characters.
    
    // load the font.  what fonts any of you have is going
    // to be system dependent, but on my system they are
    // in /usr/X11R6/lib/X11/fonts/*, with fonts.alias and
    // fonts.dir explaining what fonts the .pcf.gz files
    // are.  in any case, one of these 2 fonts should be
    // on your system...or you won't see any text.
    
    // get the current display.  This opens a second
    // connection to the display in the DISPLAY environment
    // value, and will be around only long enough to load 
    // the font. 
    dpy = XOpenDisplay(NULL); // default to DISPLAY env.   

    fontInfo = XLoadQueryFont(dpy, "-adobe-helvetica-medium-r-normal--18-*-*-*-p-*-iso8859-1");
    if (fontInfo == NULL) {
	fontInfo = XLoadQueryFont(dpy, "fixed");
	if (fontInfo == NULL) {
	    printf("no X font available?\n");
	}
    }

    // after loading this font info, this would probably be the time
    // to rotate, scale, or otherwise twink your fonts.  

    // start at character 32 (space), get 96 characters (a few characters past z), and
    // store them starting at base.
    glXUseXFont(fontInfo->fid, 32, 96, base);

    // free that font's info now that we've got the 
    // display lists.
    XFreeFont(dpy, fontInfo);

    // close down the 2nd display connection.
    XCloseDisplay(dpy);
}

void lesson13::KillFont()                         // delete the font.
{
    glDeleteLists(base, 96);                    // delete all 96 characters.
}

void lesson13::glPrint(char *text)                      // custom gl print routine.
{
    if (text == NULL) {                         // if there's no text, do nothing.
	return;
    }
    
    //glRasterPos2f(-0.2f+0.35f*((float)(cos(cnt1))), 0.35f*((float)(sin(cnt2))));
    glRasterPos3f(0.0f, 1.0f, 18.0f);
    //cnt1 += 0.01f;
    //cnt2 += 0.0081f;
    cnt1 = 25.0f;
    cnt2 = 25.0f;

    glPushAttrib(GL_LIST_BIT);                  // alert that we're about to offset the display lists with glListBase
    glListBase(base - 32);                      // sets the base character to 32.

    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); // draws the display list text.
    glPopAttrib();                              // undoes the glPushAttrib(GL_LIST_BIT);
}

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void lesson13::InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
  glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
  glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
  glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
  glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();				// Reset The Projection Matrix

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

  glMatrixMode(GL_MODELVIEW);

  BuildFont();  
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void lesson13::ReSizeGLScene(int Width, int Height)
{
  if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
    Height=1;

  glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void lesson13::DrawGLScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
  glLoadIdentity();				// Reset The View
  glTranslatef(0.0f, 0.0f, -1.0f);              // move 1 unit into the screen.
  
  // Pulsing Colors Based On Text Position
  glColor3f(1.0f*((float)(cos(cnt1))),1.0f*((float)(sin(cnt2))),1.0f-0.5f*((float)(cos(cnt1+cnt2))));

  // Position The Text On The Screen
  glRasterPos2f(-0.2f+0.35f*((float)(cos(cnt1))), 0.35f*((float)(sin(cnt2))));

  glPrint("OpenGL With NeHe");                  // print gl text to the screen.

  cnt1 += 0.01f;
  cnt2 += 0.0081f;

  // since this is double buffered, swap the buffers to display what just got drawn.
  glutSwapBuffers();
}

/* The function called whenever a key is pressed. */
void lesson13::keyPressed(unsigned char key, int x, int y) 
{
    /* avoid thrashing this procedure */
    usleep(100);

    /* If escape is pressed, kill everything. */
    if (key == ESCAPE) 
    { 
	/* get rid of font display lists */
	KillFont();

	/* shut down our window */
	glutDestroyWindow(window); 
	
	/* exit the program...normal termination. */
	exit(0);                   
    }
}


