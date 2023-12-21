//
//  main.cpp
//  Fish-Fillet-Project
//
//  Created by Yehia Sharawy on 21/12/2023.
//

//Libraries
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION  //remove anu OPENGL Deprications
#include <GLUT/glut.h>
#else
#include<GL/glut.h>
#endif
#include <math.h>
#include <cstring>
#include<stdlib.h>

//Global Variables
int physicalWidth = 400;
int physicalHeight = 400;
int logicalWidth = 100;
int logicalHeight = 100;
int centerX = logicalWidth/2;
int centerY = logicalHeight/2;

void init2D(float r, float g, float b){
    glClearColor(r,g,b,0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,logicalWidth,0.0,logicalHeight);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.97,1.0,0.0);
    
    glutSwapBuffers();
    glFlush();
}


int main(int argc, char *argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(physicalWidth,physicalHeight);
    glutInitWindowPosition(600,300);
    glutCreateWindow("Revision 12th"); //Title of the window.
    init2D(0.0,0.0,1); //Background Color
    glutDisplayFunc(display);
    glutMainLoop();
}
