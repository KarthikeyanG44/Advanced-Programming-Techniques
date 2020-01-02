/*
Author Name : Karthikeyan Ganesan
Class : ECE6122
Last Date Modified : 6th October 2019

Description:
This program reads in the dimensions of a lattice and calculates the maximum number of paths to reach the bottom of the lattice by only traversing in the
horizontal and vertical directions. Multi-threading is used here for the same purpose
*/
#include<iostream>
#include<algorithm>
#include<thread>
#include<cstdlib>
using namespace std;
//This function calculates the factorial of a given number
void factorial(long double &d)
{
	for(int i = d-1;i>0;--i)
   d*= i;
}

int main(int argc,char* argv[])
{
  //Check for the number of input arguments
	if (argc < 5)
	{
		cout << "Enter all required arguments";
    return(0);
	}

  //Read in the height and width of the lattice from the read text file
  //The number of paths is given by (h+w) choose w or (h+w) choose h
	int h = atoi(argv[2]);
	int w = atoi(argv[4]);
  long double numerator = h+w;
  long double denom_1 = h;
  long double denom_2 = w;
	long double num_paths;
  // Spawn separate threads to calculate the factorial of numerator and denominators
	thread num(factorial,ref(numerator));
  thread d1(factorial,ref(denom_1));
  thread d2(factorial,ref(denom_2));
  num.join();
  d1.join();
  d2.join();
  // After all threads are synchronized calculate the number of paths
	num_paths = numerator/(denom_1*denom_2);
	cout << "Total number of routes" << num_paths << endl;
  return(0);
}
