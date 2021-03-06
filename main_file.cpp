#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glut.h>
#include "glm/glm.hpp"
#include <iostream>
#include <string>
#include <random>
#include "lib/EasyBMP.h"
#include "common/Model_OBJ.hpp"
#include "common/lesson13.h"
#include "common/DirectoryMonitor.h"

#define NUMBER_BMP_TEXTURES 2
#define SKY_TEXTURE 0
#define SKY_TEXTURE_SIZE 200
#define WALL_TEXTURE 1
#define WALL_TEXTURE_SIZE 200

GLuint textureName[NUMBER_BMP_TEXTURES];
GLubyte SkyTexture[SKY_TEXTURE_SIZE][SKY_TEXTURE_SIZE][3];
GLubyte WallTexture[WALL_TEXTURE_SIZE][WALL_TEXTURE_SIZE][3];

Model_OBJ obj;
lesson13 lesson;

DirectoryMonitor directoryMonitor;
int currentPos;
std::vector<std::string> dirContent;
std::string currentDir = "/", displayFile;
std::vector<float> xs, zs;
 
float rotationAngle = 0;

void LoadBMPTextures(void)
{
    BMP LoadedTextureFloor, LoadedTextureWall;

    LoadedTextureFloor.ReadFromFile("floor.bmp");
    LoadedTextureWall.ReadFromFile("wall.bmp");

    for(int i = 0; i < SKY_TEXTURE_SIZE; i++)
    {
       for(int j = 0; j < SKY_TEXTURE_SIZE; j++)
       {
          SkyTexture[i][j][0] = (GLubyte) LoadedTextureFloor(i,j)->Red;
          SkyTexture[i][j][1] = (GLubyte) LoadedTextureFloor(i,j)->Green;
          SkyTexture[i][j][2] = (GLubyte) LoadedTextureFloor(i,j)->Blue;
       }
    }

    for(int i = 0; i < WALL_TEXTURE_SIZE; i++)
    {
       for(int j = 0; j < WALL_TEXTURE_SIZE; j++)
       {
          WallTexture[i][j][0] = (GLubyte) LoadedTextureWall(i,j)->Red;
          WallTexture[i][j][1] = (GLubyte) LoadedTextureWall(i,j)->Green;
          WallTexture[i][j][2] = (GLubyte) LoadedTextureWall(i,j)->Blue;
       }
    }
}

void initBMPTextures()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    glGenTextures(NUMBER_BMP_TEXTURES, textureName);

    LoadBMPTextures();

    glBindTexture(GL_TEXTURE_2D, textureName[SKY_TEXTURE]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, SKY_TEXTURE_SIZE, SKY_TEXTURE_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, &SkyTexture[0][0][0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    
    glBindTexture(GL_TEXTURE_2D, textureName[WALL_TEXTURE]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, WALL_TEXTURE_SIZE, WALL_TEXTURE_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, &WallTexture[0][0][0]);                              
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
}

void enterToDir()
{
    printf("enter dir\n" );
    if(dirContent[currentPos].compare("RETURN") == 0)
    {
        currentDir = currentDir.substr(0, (int) currentDir.find_last_of("/"));
        directoryMonitor.getDir(currentDir, dirContent);
        xs.clear();
        zs.clear();

        if(currentDir.compare("/") != 0)
        {
            dirContent.push_back("RETURN");
        }
    }
    else
    {
        currentDir += "/" + dirContent[currentPos];
        directoryMonitor.getDir(currentDir, dirContent);
        dirContent.push_back("RETURN");
        xs.clear();
        zs.clear();
    }
}

void changePos(int val)
{
    if( (currentPos + val < dirContent.size()) && (val > 0) )
    {
        currentPos += val;
    }
    else if(val > 0)
    {
        currentPos = 0;
    }
    else if(currentPos == 0 && val < 0)
    {
        currentPos = dirContent.size() - 1;
    }
    else
    {
        currentPos += val;
    }
}

void drawObjects(float x, float y, float z)
{
    glPushMatrix();
    //glScalef(3.0f, 3.0f, 3.0f);
    glTranslatef(x, y, z);
    obj.Draw();
    glPopMatrix();
}

// Draw the floor
void drawFloor(float width, float length, float alpha)
{
    initBMPTextures();
    glBindTexture(GL_TEXTURE_2D, textureName[SKY_TEXTURE]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);    
    glVertex3f(-width / 2, 0.0f, length / 2);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-width / 2, 0.0f, -length / 2);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(width / 2, 0.0f, -length / 2);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(width / 2, 0.0f, length / 2);
    
    glEnd();
}

void drawWalls(float width, float length, float height)
{
    glBindTexture(GL_TEXTURE_2D, textureName[WALL_TEXTURE]);
    //1st wall
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-width/2, 0, length/2);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-width/2, height/2, length/2); 
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-width/2, height/2, -length/2); 
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-width/2, 0, -length/2); 
    glEnd();
    //2nd wall
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-width/2, 0, -length/2);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-width/2, height/2, -length/2); 
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(width/2, height/2, -length/2); 
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(width/2, 0, -length/2); 
    glEnd();
    //3rd wall
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(width/2, 0, -length/2);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(width/2, height/2, -length/2); 
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(width/2, height/2, length/2); 
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(width/2, 0, length/2); 
    glEnd();
}
 
// Handle keyboard input
void handleKeyPress(unsigned char key, int x, int y) 
{
    switch (key) 
    {
        case 27: 
            exit(0);
            break;
        case 120:
            changePos(1);
            break;
        case 122:
            changePos(-1);
            break;
        case 13:
            enterToDir();
            break;
        case 32:
            enterToDir();
            break;
    }
}
 
void initRendering() 
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHTING);                
    glEnable(GL_LIGHT0);
                 
    // Setup light
    GLfloat lightAmbient[] = {0.5f, 0.5f ,0.5f, 1.0f}; 
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
 
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse); 
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
     
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    obj.Load("cube.obj");

    glShadeModel(GL_SMOOTH);
    lesson.BuildFont();

    directoryMonitor.getDir("/", dirContent);
}
 
void handleResize(int w, int h) 
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(55.0, (double)w / (double)h, 1.0, 1000.0);
}
 
void drawScene() 
{
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
     
    glTranslatef(0.0f, 0.0f, -45.0f);
    glRotatef(45, 1.0f, 0.0f, 0.0f);
         
    // Set light position
    GLfloat lightPosition[] = {0.0f, 30.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    // Enable drawing colors to the screen
    glColorMask(1, 1, 1, 1); 
    // Blend the floor onto the screen
    glEnable(GL_BLEND);    
    drawFloor(30.0f, 30.f, 0.8f);
    drawWalls(30.0f, 30.0f, 30.0f);
    glDisable(GL_TEXTURE_2D);

    if(xs.empty() || zs.empty())
    {
        for(int j=0; j<dirContent.size(); j++)
        {
            xs.push_back( (float) (0 + (rand() % (int)(28 - 0 + 1)) -14) );
            zs.push_back( (float) (0 + (rand() % (int)(28 - 0 + 1)) -14) );
        }
    }

    for(int i=0; i<dirContent.size(); i++)
    {
        if(currentPos == i)
        {
            drawObjects(xs[i], 10.0f, zs[i]);
        }
        else
        {
            drawObjects(xs[i], 2.0f, zs[i]);
        }
    }
    //printf("currentPos: %d\n", currentPos);
    char *c = (char*) dirContent[currentPos].c_str();
    lesson.glPrint(c);
    glDisable(GL_BLEND);
     
    glutSwapBuffers();    
}
 
void update(int value) 
{
    rotationAngle += 1.0f;
    if (rotationAngle > 360) 
        rotationAngle = 0;        
     
    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}
 
 
int main(int argc, char *argv[])
{
    // Initialize Glut
    glutInit(&argc, argv);
    // Initialize display mode and use stencil
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_ALPHA);
    // Initialize a Glut window
    glutInitWindowSize(640, 480);
    // Create glut window
    glutCreateWindow("OpenGL GLUT reflection example");
        // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    // Initialize rendering
    initRendering();
    // Set display function
    glutDisplayFunc(drawScene);
    // Set timer function
    glutTimerFunc(25, update, 0);
    // Set keyPress function
    glutKeyboardFunc(handleKeyPress);
    // Set resize function 
    glutReshapeFunc(handleResize);    
    // Start OpenGL loop
    glutMainLoop();
     
    return 0;
}