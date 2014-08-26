#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h> //Przydatne do wypisywania komunikatów na konsoli
#include <dirent.h>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Folder.h"
#include "variables.h"

using namespace std;

float speed_x=0; //60 stopni/s
float speed_y=0; //60 stopni/s
int lastTime=0;
float angle_x;
float angle_y;


void listFiles(const char* cPath)
{
	struct dirent * file;
    DIR * stream;
	string sNames[100];
	int i=0;
   
    if(( stream = opendir( cPath ) ) ) {
        while(( file = readdir( stream ) ) )
        {     
			sNames[i] = file->d_name;
			i++;
		}
       
        closedir( stream );
    }
    else
         printf( "! wywołując funkcję opendir(%s) pojawił się błąd otwarcia strumienia dla danej ścieżki, może nie istnieje, lub podano ścieżkę pustą\n", cPath );

	//return sNames;
}

void drawFloor(float width, float length, float alpha)
{
    // Begin drawing the floor
    glBegin(GL_QUADS);
         
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glNormal3f(0.0f, 1.0f, 0.0f);    
    glVertex3f(-width / 2, 0.0f, length / 2);
    glVertex3f(-width / 2, 0.0f, -length / 2);
    glVertex3f(width / 2, 0.0f, -length / 2);
    glVertex3f(width / 2, 0.0f, length / 2);
     
    glEnd();
}

void drawFolder()
{
	glPushMatrix();

	glTranslatef(2.0f,0.0f,5.0f);
	glRotatef(angle_y, 0.0f,1.0f,0.0f);

	glVertexPointer(3,GL_FLOAT,0,folderVertices);
	glColorPointer(3,GL_FLOAT,0,folderColours);	
	glDrawArrays(GL_TRIANGLE_FAN, 0, folderVertexCount);

}

void displayFrame(void) {
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
	glMatrixMode(GL_PROJECTION);
	//glLoadMatrixf(glm::value_ptr(P));
	glMatrixMode(GL_MODELVIEW);		

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glEnable(GL_BLEND);
	drawFloor(15.0f, 15.0f, 1.0f);
	glDisable(GL_BLEND);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);	
	
	glutSwapBuffers();
}


void nextFrame(void) {
	int actTime=glutGet(GLUT_ELAPSED_TIME);
	int interval=actTime-lastTime;
	lastTime=actTime;
	angle_x+=speed_x*interval/1000.0;
	angle_y+=speed_y*interval/1000.0;
	if (angle_x>360) angle_x-=360;
	if (angle_x>360) angle_x+=360;
	if (angle_y>360) angle_y-=360;
	if (angle_y>360) angle_y+=360;
	
	glutPostRedisplay();
}

void keyDown(int c, int x, int y) {
  switch (c) {
    case GLUT_KEY_LEFT: 
      speed_y=60;
      break;
    case GLUT_KEY_RIGHT:
      speed_y=-60;
      break;
    case GLUT_KEY_UP: 
      speed_x=60;
      break;
    case GLUT_KEY_DOWN:
      speed_x=-60;
      break;  
  }
}

void keyUp(int c, int x, int y) {
  switch (c) {
    case GLUT_KEY_LEFT: 
      speed_y=0;
      break;
    case GLUT_KEY_RIGHT:
      speed_y=-0;
      break;
    case GLUT_KEY_UP: 
      speed_x=0;
      break;
    case GLUT_KEY_DOWN:
      speed_x=-0;
      break;  
  }
}

int main(int argc, char* argv[]) 
{
	//string sNames[100] = listFiles( argv[1] );
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Program OpenGL");        
	glutDisplayFunc(displayFrame);
	glutIdleFunc(nextFrame);

	//Tutaj kod inicjujacy	
	glewInit();
	glutSpecialFunc(keyDown);
	glutSpecialUpFunc(keyUp);
	
	
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}
