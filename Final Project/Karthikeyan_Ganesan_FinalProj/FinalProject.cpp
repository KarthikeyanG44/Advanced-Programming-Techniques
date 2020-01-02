/*
Author Name : Karthikeyan Ganesan
Class : ECE 6122
Last Date Modified : 3rd December 2019

Description : This file simulates a UAV show where UAVs start moving towards a central point and start revolving around the central poin.
This simulation is carried out with a combination of the MPI infrastructure and the OpenGL graphics libraries.
The kinematics of each of the process is calculated by indiviudal processes in the MPI infrastructure. Initially, a distance dependent force of magnitude
less than 20N is applied. Once the bots reach the surface of the virtual sphere, the Hooke's law is used to bind the UAV to surface. The simulation ends
when the UAVs have flown for 60 seconds around the sphere.
*/

///////////////////////////////////////////////////
/////Include the relevant header files////////////
/////////////////////////////////////////////////

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "iomanip"
#include <cmath>
#include <math.h>
#include <cstdlib>
#include <GL/glut.h>
#include <chrono>
#include <thread>
#include "ECE_Bitmap.h"

///////////////////////////////////////////////////
/////Global Variables Declaration/////////////////
/////////////////////////////////////////////////

const int numElements = 6;  // x, y, z, vx, vy, vz

const int rcvSize = 16 * 6;  // (Main task + 15 UAVs) * numElements

double* rcvBuffer = new double[rcvSize];

double sendBuffer[numElements];

float eyeX = 60,eyeY = 0, eyeZ = 100;  //Define the coordinates of the centre of the camera and look at coordinates

float centerX = 0 ,centerY = 0, centerZ = 0;

float x,y,z,vx,vy,vz;   //UAV specific kinematic parameters

float widthCoord[3]={-22,0,22},lengthCoord[5] = {-45.0,-22.5,0,22.5,45.0};
float gravityForce[3] = {0.0,0.0,-10.0} , targetPos[3] = {0.0,0.0,50.0} ,dirVec[3] = {0.0,0.0,0.0};
float force[3] = {0.0,0.0,0.0}, acc[3] = {0.0,0.0,0.0};

float phi , theta = 0 ,thetaRad, phiRad,rho = 10,springConstant = 20,colorX,colorY,colorZ;

int widthIdx = 0 , lengthIdx = 0 , massUAV = 1;

#define PI 3.14159265

///////////////////////////////////////////////////
//Reshaping Function for viewing transformations//
/////////////////////////////////////////////////

void changeSize(int w, int h)
{
    float ratio = ((float)w) / ((float)h); // window aspect ratio
    glMatrixMode(GL_PROJECTION); // projection matrix is active
    glLoadIdentity(); // reset the projection
    gluPerspective(90.0, ratio, 0.1, 1000.0); // perspective transformation
    glMatrixMode(GL_MODELVIEW); // return to modelview mode
    glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
}

GLuint texture[1];

BMP inBitmap;

///////////////////////////////////////////////////
/////Function to define and enable texturing//////
/////////////////////////////////////////////////

void myinit(void)
{
    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);

    inBitmap.read("AmFBfield.bmp");


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Create Textures

    glGenTextures(1, texture);

    // Setup first texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture


    glTexImage2D(GL_TEXTURE_2D, 0, 3, inBitmap.bmp_info_header.width, inBitmap.bmp_info_header.height, 0,
        GL_BGR_EXT, GL_UNSIGNED_BYTE, &inBitmap.data[0]);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glEnable(GL_TEXTURE_2D);
}

///////////////////////////////////////////////////
//////Function to display football field//////////
/////////////////////////////////////////////////

void displayFootballField()
{
  glColor3f(0.0, 0.7, 0.0);
  glBegin(GL_QUADS);
      glTexCoord2f(1, 0);
      glVertex3f(-25.0, -55.0, 0.0);
      glTexCoord2f(0, 0);
      glVertex3f(-25.0, 55.0, 0.0);
      glTexCoord2f(0, 1);
      glVertex3f(25.0, 55.0, 0.0);
      glTexCoord2f(1, 1);
      glVertex3f(25.0, -55.0, 0.0);
  glEnd();
}

///////////////////////////////////////////////////
///////////Function to draw UAVs//////////////////
/////////////////////////////////////////////////

void drawUAV()
{
  float colorA = (float) colorX/255.0;
  float colorB = (float) colorY/255.0;
  float colorC = (float) colorZ/255.0;
  glColor3f(colorA,colorB,colorC);
  glPushMatrix();
    glTranslatef(0.0,0.0,0.5);
    glScalef(1.0,1.0,1.0);
    glutSolidSphere(0.5, 10, 10);
  glPopMatrix();
}

///////////////////////////////////////////////////
////////Function to draw Virtual Sphere///////////
/////////////////////////////////////////////////

void drawSphereCentre()
{
  glColor3f(0.5,0.5,0.5);
  glPushMatrix();
    glTranslatef(0.0,0.0,50);
    glScalef(1.0,1.0,1.0);
    glutWireSphere(10, 20, 20);
  glPopMatrix();
}

///////////////////////////////////////////////////
/////Function to render the entire scene//////////
/////////////////////////////////////////////////

void renderScene()
{

    // Clear color and depth buffers
    glClearColor(0.0, 0.7, 1.0, 1.0); // background color to green??
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();


    gluLookAt(eyeX, eyeY, eyeZ,
              centerX, centerY, centerZ,
              0.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);

    glBindTexture(GL_TEXTURE_2D, texture[0]);

    displayFootballField();

    glBindTexture(GL_TEXTURE_2D,0);

    for (int i = 0; i < 15 ; i++)
    {
      int offset = (i+1)*6;
      float transX = rcvBuffer[offset];
      float transY = rcvBuffer[offset+1];
      float transZ = rcvBuffer[offset+2];
      glPushMatrix();
        glTranslatef(transX,transY,transZ);
        drawUAV();
      glPopMatrix();
    }

    glPushMatrix();
      drawSphereCentre();
    glPopMatrix();

    glutSwapBuffers(); // Make it all visible

    MPI_Allgather(sendBuffer, numElements, MPI_DOUBLE, rcvBuffer, numElements, MPI_DOUBLE, MPI_COMM_WORLD);
}

///////////////////////////////////////////////////
////Timer function that is called every 100msec///
////to update the scene with new UAV locations///
/////////////////////////////////////////////////

void timerFunction(int id)
{
    glutPostRedisplay();
    glutTimerFunc(100, timerFunction, 0);
}

///////////////////////////////////////////////////
/////Function to process keyboard inputs//////////
/////////////////////////////////////////////////

void processNormalKeys(unsigned char key, int xx, int yy)
{
    if (key == 'd' || key == 'D')
    {
        eyeZ = eyeZ - 5 ;
        glutPostRedisplay();
    }
    else if (key == 'u' || key == 'U')
    {
        eyeZ = eyeZ + 5 ;
        glutPostRedisplay();
    }
    else if (key == 'r' || key == 'R')
    {
        eyeY = eyeY + 5 ;
        glutPostRedisplay();
    }
    else if (key == 'l' || key == 'L')
    {
        eyeY = eyeY - 5 ;
        glutPostRedisplay();
    }
    else if ( key == 'q' || key == 'Q')
    {
      exit(0);
    }
}

///////////////////////////////////////////////////
//Function to repeatedly process GLUT functions///
/////////////////////////////////////////////////

void mainOpenGL(int argc, char**argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);

    glutCreateWindow(argv[0]);
    glEnable(GL_DEPTH_TEST);
    myinit();
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(processNormalKeys);
    glutTimerFunc(100, timerFunction, 0);
    glutMainLoop();
}

///////////////////////////////////////////////////
//Function to calculate the magnitude of vector///
/////////////////////////////////////////////////

float calcMagnitude(const float a[])
{
  float mag = 0 ;
  for (int i = 0; i < 3; i++)
    mag+= a[i]*a[i];

  return(sqrt(mag));
}

///////////////////////////////////////////////////
//Function to find a unit vector in direction ////
//////////////of a given vector///////////////////
/////////////////////////////////////////////////

void normalizeVector(float a[])
{
    float mag = calcMagnitude(a);
    for (int i = 0 ; i < 3 ; i++)
    {
      a[i]/= mag;
    }
}

///////////////////////////////////////////////////
//Function to calculate the kinematics of UAVs////
/////////////////////////////////////////////////

void CalcualteUAVsLocation(int rank)
{
  int offset = rank * 6;

  for (int j = 0; j < 3; j++)
  {
    dirVec[j] = (targetPos[j] - rcvBuffer[offset + j]);
  }

if (calcMagnitude(dirVec) > 20)                                               // Give full thrust only if the UAV if far away from the centre
{
    normalizeVector(dirVec);
    for (int j = 0; j < 3 ; j++)
    {
      force[j] = 15*dirVec[j] - gravityForce[j];
    }
    if (calcMagnitude(force) > 20)
    {
      for(int j = 0 ; j < 3; j++)
      {
        force[j] = 20*dirVec[j];
      }
    }
    for (int j = 0; j < 6; j++)
    {
      if ( j < 3 )
      {
        acc[j] = force[j];
        sendBuffer[j] = rcvBuffer[offset + j] + rcvBuffer[offset + j + 3]*0.1 + 0.5 * acc[j] * 0.1 * 0.1 ;
      }
      else
      {
        sendBuffer[j] = rcvBuffer[offset + j] + acc[j-3]*0.1;
        if (sendBuffer[j] > 2) sendBuffer [j] = 2;
      }
    }
}
else
{
    float springForce = springConstant*(calcMagnitude(dirVec)-10);
    normalizeVector(dirVec);
    for (int j = 0; j < 3 ; j++)
    {
      force[j] = springForce*(dirVec[j]) - gravityForce[j];
    }
    if (calcMagnitude(force) > 20)
    {
      for(int j = 0 ; j < 3; j++)
      {
        force[j] = 20 * dirVec[j];
      }
    }
    for (int j = 0; j < 6; j++)
    {
      if ( j < 3 )
      {
        acc[j] = force[j];
        sendBuffer[j] = rcvBuffer[offset + j] + rcvBuffer[offset + j + 3]*0.1 + 0.5 * acc[j] * 0.1 * 0.1 ;
      }
      else
      {
        sendBuffer[j] = rcvBuffer[offset + j] + acc[j-3]*0.1;
        if (sendBuffer[j] > 10) sendBuffer[j] = 10;
      }
    }
}
}

///////////////////////////////////////////////////
//Main function which indicates begining of every//
//////////////individual process/////////////////
/////////////////////////////////////////////////

int main(int argc, char**argv)
{
    int numTasks, rank , cycle = 0;

    int rc = MPI_Init(&argc, &argv);

    if (rc != MPI_SUCCESS)
    {
        printf("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int gsize = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &gsize);

    colorX = 255.0;
    colorY = 0.0;
    colorY = 0.0;

    if (rank > 0 && rank <= 5)
    {
      lengthIdx = lengthIdx + (rank-1);
      widthIdx = widthIdx;
      x = widthCoord[widthIdx];
      y = lengthCoord[lengthIdx];
      z = 0.0 ;
      vx = 0.0;
      vy = 0.0;
      vz = 0.0;
    }

    else if (rank > 5 && rank <= 10)
    {
      lengthIdx = lengthIdx + (rank % 5);
      widthIdx = widthIdx + 1;
      x = widthCoord[widthIdx];
      y = lengthCoord[lengthIdx];
      z = 0.0 ;
      vx = 0.0;
      vy = 0.0;
      vz = 0.0;
    }

    else if (rank > 10 && rank <= 15)
    {
      lengthIdx = lengthIdx + (rank % 5);
      widthIdx = widthIdx + 2;
      x = widthCoord[widthIdx];
      y = lengthCoord[lengthIdx];
      z = 0.0 ;
      vx = 0.0;
      vy = 0.0;
      vz = 0.0;
    }

    sendBuffer[0] = x;
    sendBuffer[1] = y;
    sendBuffer[2] = z;
    sendBuffer[3] = vx;
    sendBuffer[4] = vy;
    sendBuffer[5] = vz;

    MPI_Allgather(sendBuffer, numElements, MPI_DOUBLE, rcvBuffer, numElements, MPI_DOUBLE, MPI_COMM_WORLD);

    if (rank == 0)
    {
        mainOpenGL(argc, argv);
    }

    else
    {
        // Sleep for 5 seconds
        std::this_thread::sleep_for(std::chrono::seconds(5));
        for (int ii = 0; ii < 1200 ; ii++)
        {
            CalcualteUAVsLocation(rank);
            MPI_Allgather(sendBuffer, numElements, MPI_DOUBLE, rcvBuffer, numElements, MPI_DOUBLE, MPI_COMM_WORLD);
            if (cycle%2!=0) {colorX += 1; if (colorX >= 255) cycle += 1;}
            else if (cycle % 2) {colorX -= 1; if (colorX <= 40) cycle += 1;}
        }
    }

    return 0;
}
