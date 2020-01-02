/*
Author Name : Karthikeyan Ganesan
Class : ECE6122
Last Date Modified : 6th October 2019

Description:
This file reads a triangle of a particular number of levels and calculates the maximum path sum in the triangle using multi threading
*/
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<omp.h>
using namespace std;
// This function takes current position in the triangle and finds the neighbours closest to it that lead to a maximum sum
int check_next_level(int arr[], int curr_pos, int level)
{
	if (arr[curr_pos] + arr[curr_pos + level] > arr[curr_pos] + arr[curr_pos + level + 1])
		return(curr_pos + level);
	else
		return(curr_pos + level + 1);
}
int main(int argc, char* argv[])
{
  // Check the total number of input arguments
  if(argc < 2)
  {
    cout<<"Enter the input file name";
    return(0);
  }
	int total_levels, a, level = 1, sum = 0,curr_idx = 0, next_level_idx;
	//Read the specific input file and open it to read the triangle elements
  string s(argv[1]);
	ifstream infile;
	infile.open(s);
	vector<int> ip_data;
	while (!infile.eof())
	{
		infile >> a;
		ip_data.push_back(a);
	}
 //Read total number of levels and push into an array of triangle
	total_levels = ip_data[0];
	int triangle[100000];
	copy(++ip_data.begin(), ip_data.end(), triangle);
 // Use parallel processing to find sum with the maximum neighbours
#pragma parallel omp for reduction(+:sum)
	for (level =1 ;level< total_levels;level++)
	{
		curr_idx = check_next_level(triangle, curr_idx, level);
		sum += triangle[curr_idx];
	}
  sum+= triangle[0];
  //Once sum is found,display it and write it to a file
	ofstream out_file;
	out_file.open("Output3.txt");
	out_file << sum;
	out_file.close();
	cout << "Maximum sum is" << sum;
  return(0);
}