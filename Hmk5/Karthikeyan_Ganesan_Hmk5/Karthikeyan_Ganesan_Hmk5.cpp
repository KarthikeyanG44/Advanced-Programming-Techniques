/*
Author Name : Karthikeyan Ganesan
Class : ECE6122
Last Date Modified : 5th November 2019

Description:
This file reads the position, speed and direction vectors of a docking station in free space and spacecrafts. The multi processes calculate the kinematics of each component of the system.
The main process broadcasts the status and location of the docking system and other spacecrafts. The other processes broadcast their own status,location and forces of themselves to the
main process
*/

//////Include the relevant header files///////
#include<iostream>
#include<stdio.h>
#include"mpi.h"
#include<fstream>
using namespace std;
#include<vector>
#include<math.h>
#include<sstream>
#include<string>
#include<stdlib.h>
#include<iomanip>
#include<map>

//////Function to read the input file///////
int k = 0;
void load_ip_file(float init[])
{
  ifstream file("in.dat");
  string S, T;
  while(getline(file,S))
  {
    istringstream  X(S);
    while(getline(X, T,' '))
    {
      char* pEnd;
      float val = strtof(T.c_str(),&pEnd);
      init[k] = val; ++k;
    }
  }
}

//////Function to calculate the kinematics of Buzzy///////
void calc_buzzy_kin(float pos[],float vel[])
{
  for (int i = 0; i < 3; i++)
  {
    pos[i] = pos[i] + vel[i];
  }
}

//////Function to calculate the kinematics of Yellow Jackets///////
void calc_jacket_kin(float pos[],float vel[],float acc[])
{
  for (int i = 0; i < 3 ; i++)
  {
    pos[i] = pos[i] + vel[i] + 0.5*acc[i];
    vel[i] = vel[i] + acc[i];
  }
}
//////Function to calculate the magnitude of a vector///////
float calcMagnitude(float arr[])
{
  float magnitude = 0;
  for (int i = 0; i < 3; i++)
  {
    magnitude += arr[i]*arr[i];
  }
  return(sqrt(magnitude));
}
//////Function to find unit vector///////
void normalize(float arr[])
{
  for (int i = 0; i < 3;i++)
  {
    arr[i] = arr[i]/calcMagnitude(arr);
  }
}
int main(int argc, char *argv[])
{
  // Define variables
  int numTasks,rank,rc;
  float posDiff[3],pos[3],dirVec[3],vel[3],speed;
  float acc[3],force[3];
  int status = 1;
  float sendArray[8];
  float init[500];
  double maxTime,maxForce,jacketMass = 10000;
  int root = 0;
// Initialize MPI infrastructure //
  rc = MPI_Init(&argc, &argv);
  if (rc != MPI_SUCCESS)
    {
        printf("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

  MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(rank == 0)
   {
     load_ip_file(init);
   }
MPI_Bcast(&init,500,MPI_FLOAT,root,MPI_COMM_WORLD);
float *receiveArray = (float*)malloc(numTasks*8*sizeof(float));

// store the initial values of location, speed and dir vec //
maxTime = init[0];
maxForce = init[1];
int offset = (rank*7) + 2;
int accessIdx = 0;
for (int j = offset; j < offset+7; j++)
{
  if ( accessIdx < 3 )
  {
    pos[accessIdx] = init[j];
    ++accessIdx;
  }
  else if( accessIdx == 3)
  {
    speed = init[j];
    ++accessIdx;
  }
  else
  {
    dirVec[accessIdx-4] = init[j];
    ++accessIdx;
  }
}

// calculate the direction of force, acceleration and velocities for initial instance //
for (int i = 0; i < 3; i++)
{
  posDiff[i] = pos[i] - init[i+2];
  vel[i] = speed * dirVec[i];
  acc[i] = 0;
  force[i] = 0;
}

// calculate the position for different timesteps //
for (int t = 0; t < maxTime ; t++)
{
  // Update the sending array each time //
  for (int j = 0 ; j < 8; j++)
  {
         if (j == 0) sendArray[j] = rank;
         else if (j == 1) sendArray[j] = status;
         else if ( j > 1 && j < 5) sendArray[j] = pos[j-2];
         else sendArray[j] = force[j - 5];
  }
  if (rank == 0)
  {
    calc_buzzy_kin(pos,vel);
    MPI_Allgather(&sendArray,8,MPI_FLOAT,receiveArray,8,MPI_FLOAT,MPI_COMM_WORLD);
    for (int j = 8; j < 64 ; j++)
    {
      if((j+1) % 8 != 0)
      {
        if (((j+1) % 8 == 0) || ((j+1) % 9 == 0))
        cout <<fixed<<int(receiveArray[j]) << "," ;
        else
        cout << setprecision(6) << scientific << receiveArray[j] << ",";
      }
      else
      cout << setprecision(6) << scientific << receiveArray[j] << endl;
    }
  }
  else
  {
    calc_jacket_kin(pos,vel,acc);
    MPI_Allgather(&sendArray,8,MPI_FLOAT,receiveArray,8,MPI_FLOAT,MPI_COMM_WORLD);
  }
}
MPI_Finalize();
return 0;
}
