#ifndef LESSON13_H
#define LESSON13_H

class lesson13
{
	public:
		void BuildFont();
		void KillFont();
		void glPrint(char *text);
		void InitGL(int Width, int Height);
		void ReSizeGLScene(int Width, int Height);
		void DrawGLScene();
		void keyPressed(unsigned char key, int x, int y);
};

#endif