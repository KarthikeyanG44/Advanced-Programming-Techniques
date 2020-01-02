/*
Author Name : Karthikeyan Ganesan
Class : ECE 6122
Last Date Modified : 24th November 2019

Description : This file uses the OpenGL library to display a chess board and the associated chess pieces
Further viewing manipulations like vertical movements and rotation are also performed. Finally, chess piece movements coresponding
to various key presses are also diaplyed*/

///////////////////////////////////////////////////
/////Include the relevant header files////////////
/////////////////////////////////////////////////

#include <iostream>
#include <GL/freeglut.h> // Include the GLUT header file
#include <stdio.h>
#include <math.h>
#include <stdlib.h> // standard definitions

///////////////////////////////////////////////////
/////Global Variables Declaration/////////////////
/////////////////////////////////////////////////

#define PI 3.14159265
#define ESC 27
bool lightZero = true;
bool lightOne = true;
float x = 4 , y = -5 , z = 5;
float lx = 4 ,ly = 4, lz = 0;
int side = 0, PawnNumber = 20,KnightNumber = 20,transSideP = 3, transSideK = 3;
int zRotate = 0;
bool transPawn = false, transKnight = false;

///////////////////////////////////////////////////
//Reshaping Function for viewing transformations//
/////////////////////////////////////////////////

void changeSize(int w, int h)
{
    float ratio = ((float)w) / ((float)h); // window aspect ratio
    glMatrixMode(GL_PROJECTION); // projection matrix is active
    glLoadIdentity(); // reset the projection
    gluPerspective(45.0, ratio, 0.1, 100.0); // perspective transformation
    glMatrixMode(GL_MODELVIEW); // return to modelview mode
    glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
}

///////////////////////////////////////////////////
/////Function to define and orient lighting //////
/////////////////////////////////////////////////

void lightInit(void)
{
    // material properties
    GLfloat matShininess[] = {50.0};
    GLfloat matSpecular[] = {0.5, 0.5 , 0.5 , 1.0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    // lighting positions
    GLfloat light0Diffuse[] = {0.0, 0.0, 0.0, 0.0};
    GLfloat light0Ambient[] = {0.2, 0.2, 0.2, 1.0 };
    GLfloat light0Specular[] = {0.0, 0.0, 0.0, 0.0};

    GLfloat light1Position[] = {5.0, 5.0, 8.0, 0.0};
    GLfloat light1Diffuse[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat light1Specular[] = {0.3, 0.3, 0.3, 1.0};
    GLfloat light1Ambient[] = {0.0, 0.0, 0.0, 0.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);

    glLightfv(GL_LIGHT1, GL_AMBIENT, light1Ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1Specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1Position);


    // glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
}

///////////////////////////////////////////////////
////Function to draw each of the chess pieces/////
/////////////////////////////////////////////////

void drawPawn()
{
  if (side == 0) glColor3f(1.0,1.0,0.94);
  else glColor3f(0.58,0.3,0);

  glPushMatrix();
          glTranslatef(0.0, 0.0, 0.75/2);
          glScalef(0.75,0.75,1.0);
          glutSolidSphere(0.75/2, 20, 20);
  glPopMatrix();

}
void drawRook()
{
  if (side == 0) glColor3f(1.0,1.0,0.94);
  else glColor3f(0.58,0.3,0);

  glPushMatrix();
          glTranslatef(0.0, 0.0, 0.75/2);
          glScalef(0.75,0.75,1.0);
          glutSolidCube(0.75);
  glPopMatrix();

}
void drawKnight()
{
  if (side == 0) glColor3f(1.0,1.0,0.94);
  else glColor3f(0.58,0.3,0);

  glPushMatrix();
          glTranslatef(0.0, 0.0,0.40);
          glRotatef(90, 1, 0, 0);
          glScalef(0.75,0.75,1.0);
          glutSolidTeapot(0.6);
  glPopMatrix();

}
void drawBishop()
{
  if (side == 0) glColor3f(1.0,1.0,0.94);
  else glColor3f(0.58,0.3,0);

  glPushMatrix();
          glTranslatef(0.0, 0.0, 0.0);
          glScalef(0.75,0.75,1.0);
          glutSolidCone(0.375,1.0,20,20);
  glPopMatrix();

}
void drawQueen()
{
  if (side == 0) glColor3f(1.0,1.0,0.94);
  else glColor3f(0.58,0.3,0);

  glPushMatrix();
          glTranslatef(0.0, 0.0,0.3);
          glRotatef(90, 1, 0, 0);
          glScalef(0.55,0.55,0.55);
          glutSolidTetrahedron();
  glPopMatrix();

}
void drawKing()
{
  if (side == 0) glColor3f(1.0,1.0,0.94);
  else glColor3f(0.58,0.3,0);

  glPushMatrix();
          glTranslatef(0.0, 0.0,0.53);
          glScalef(0.50,0.50,0.50);
          glutSolidOctahedron();
  glPopMatrix();
}

///////////////////////////////////////////////////
///Function to Render the chess board and piexes//
/////////////////////////////////////////////////

void renderScene(void)
{
    int i, j;
    bool isWhite = false;
    // Clear color and depth buffers
    glClearColor(0.5, 0.5, 0.5, 1.0); // background color is grey
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();


    // Set the camera centered at (x,y,1) and looking along directional
    // vector (lx, ly, 0), with the z-axis pointing up
    gluLookAt(
        x , y, z,
        lx , ly , lz,
        0.0, 0.0, 1.0);
    // Draw chess board
    for (int i = 0 ; i < 8 ; i ++)   // i->x
    {
        if (i % 2 == 0)
  		{
  			isWhite = false;
  		}
  		else
  		{
  			isWhite = true;
  		}
      for (int j = 0; j < 8; j++)   // j ->y
      {
        if (isWhite == true)
        {
          glPushMatrix();
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_POLYGON);
                glVertex3f(i,j, 0.0);
                glVertex3f(i,j+1,0.0);
                glVertex3f(i+1,j+1 , 0.0);
                glVertex3f(i+1, j, 0.0);
            glEnd();
          glPopMatrix();
          isWhite = false;
        }

        else
        {
          glPushMatrix();
          glColor3f(0.0, 0.0, 0.0);
          glBegin(GL_POLYGON);
              glVertex3f(i,j, 0.0);
              glVertex3f(i,j+1,0.0);
              glVertex3f(i+1,j+1 , 0.0);
                glVertex3f(i+1, j, 0.0);
          glEnd();
          glPopMatrix();
          isWhite = true;
        }
      }
    }


    float transX = 0.5;
    float transY = 0.5;
    // Draw the chess pieces
    for (int i = 0; i < 8; i++)
    {
        transX = transX + i;
        for (int j = 0; j < 8; j++)
        {
          transY = transY + j;
          if ( j == 0 )
          {
            side = 0;
            switch (i)
            {
              case 0:
              glPushMatrix();
                glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
                drawRook();
              glPopMatrix();
              break;

              case 1:
              if (transKnight && (transSideK == 0) && (KnightNumber == 0)){transY = transY + 2; transX = transX + 1;transKnight = false;}
              glPushMatrix();
                glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
                drawKnight();
              glPopMatrix();
              break;

              case 2:
              glPushMatrix();
                glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
                drawBishop();
              glPopMatrix();
              break;

              case 3:
              glPushMatrix();
                glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
                drawQueen();
              glPopMatrix();
              break;

              case 4:
              glPushMatrix();
                glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
                drawKing();
              glPopMatrix();
              break;

              case 5:
              glPushMatrix();
                glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
                drawBishop();
              glPopMatrix();
              break;

              case 6:
              if (transKnight && (transSideK == 0) && (KnightNumber == 1)){transY = transY + 2; transX = transX - 1;transKnight = false;}
              glPushMatrix();
                glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
                drawKnight();
              glPopMatrix();
              break;

              case 7:
              glPushMatrix();
                glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
                drawRook();
              glPopMatrix();
              break;
            }
          }

          else if ( j == 7 )
          {
            side = 1;
            switch (i)
            {
              case 0:
              glPushMatrix();
                glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
                drawRook();
              glPopMatrix();
              break;

              case 1:
              if (transKnight && (transSideK == 1) && (KnightNumber == 2)){transY = transY - 2; transX = transX + 1;transKnight = false;}
              glPushMatrix();
                glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
                drawKnight();
              glPopMatrix();
              break;

              case 2:
              glPushMatrix();
                glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
                drawBishop();
              glPopMatrix();
              break;

              case 3:
              glPushMatrix();
                glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
                drawQueen();
              glPopMatrix();
              break;

              case 4:
              glPushMatrix();
                glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
                drawKing();
              glPopMatrix();
              break;

              case 5:
              glPushMatrix();
                glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
                drawBishop();
              glPopMatrix();
              break;

              case 6:
              if (transKnight && (transSideK == 1) && (KnightNumber == 3)){transY = transY - 2; transX = transX - 1;transKnight = false;}
              glPushMatrix();
                glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
                drawKnight();
              glPopMatrix();
              break;

              case 7:
              glPushMatrix();
                glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
                drawRook();
              glPopMatrix();
              break;
            }
          }
          else if (j == 1)
          {
            if (transPawn && (transSideP == 0) && (i == PawnNumber)){transY = transY + 1; transPawn = false;}
            side = 0;
            glPushMatrix();
              glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
              drawPawn();
            glPopMatrix();
          }
          else if (j == 6)
          {
            if (transPawn && (transSideP == 1) && (i == PawnNumber)) {transY = transY - 1; transPawn = false;}
            side = 1;
            glPushMatrix();
              glTranslatef(transX,transY,0.0);   //shift origin of drawing objects //
              drawPawn();
            glPopMatrix();
          }
          transY = 0.5;
        }
        transX = 0.5;
    }
    glutSwapBuffers(); // Make it all visible
}

///////////////////////////////////////////////////
////Function to process key presses//////////////
/////////////////////////////////////////////////

void processNormalKeys(unsigned char key, int xx, int yy)
{
    if (key == '0')
    {
      if (lightZero) { glEnable(GL_LIGHT0); lightZero = false;glutPostRedisplay();}
      else { glDisable(GL_LIGHT0); lightZero = true; glutPostRedisplay();}


    }
    else if (key == '1')
    {
      if (lightOne){glEnable(GL_LIGHT1); lightOne = false; glutPostRedisplay();}
      else { glDisable(GL_LIGHT1);lightOne = true; glutPostRedisplay();}
    }

    else if (key == 'r' || key == 'R')
    {
        zRotate = zRotate + 10;
        x = -9 * sin(zRotate*PI/180) + 4;
        y = -9 * cos(zRotate*PI/180) + 4;
        glutPostRedisplay();
    }

    else if (key == 'd' || key == 'D')
    {
        z = z- 0.25 ;
        glutPostRedisplay();
    }
    else if (key == 'u' || key == 'U')
    {
        z = z + 0.25 ;
        glutPostRedisplay();
    }
    else if ( key == 'p' || key == 'P')
    {
      PawnNumber = rand() % 8;
      transPawn = true;
      transSideP = rand() % 2;
      glutPostRedisplay();
    }
    else if ( key == 'k' || key == 'K')
    {
      KnightNumber = rand() % 4;
      transKnight = true;
      transSideK = rand() % 2;
      glutPostRedisplay();
    }
    else if ( key == 'q' || key == 'Q')
    {
      exit(0);
    }
}

///////////////////////////////////////////////////
//////////////Main Function///////////////////////
/////////////////////////////////////////////////

int main(int argc, char **argv)
{
    printf("\n\
-----------------------------------------------------------------------\n\
  OpenGL Program to display chess game:\n\
  - Press p or P to move Pawn\n\
  - Press k or K to move Knight\n\
  - 1 to toggle Diffuse Lights\n\
  - 0 to toggle Ambient Lights\n\
  - q or Q to quit\n\
-----------------------------------------------------------------------\n");

    // Initializations
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Chess Board");

    // register callbacks
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(processNormalKeys);
    lightInit();
    srand(0);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}
