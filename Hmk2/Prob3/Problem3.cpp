/*
Author Name : Karthikeyan Ganesan
Class : ECE6122
Last Date Modified : 20th September 2019

Description:
This file reads a triangle of a particular number of levels and calculates the maximum path sum in the triangle.
*/
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
using namespace std;
int check_next_level(int arr[], int curr_pos, int level)
{
	if (arr[curr_pos] + arr[curr_pos + level] > arr[curr_pos] + arr[curr_pos + level + 1])
		return(curr_pos + level);
	else
		return(curr_pos+level+1);
}
int main()
{
	int total_levels,a,level = 1,sum,curr_idx =0,next_level_idx;
	string s;
	ifstream infile;
	cout << "Enter the file name";
	cin >> s;
	infile.open(s);
	vector<int> ip_data;
	while (!infile.eof())
	{
		infile >> a;
		ip_data.push_back(a);
	}
	total_levels = ip_data[0];
	int triangle[1000];
	copy(++ip_data.begin(), ip_data.end(), triangle);
	sum = triangle[0];
	while (level < total_levels)
	{
		next_level_idx = check_next_level(triangle, curr_idx, level);
		sum += triangle[next_level_idx];
		curr_idx = next_level_idx;
		level = level + 1;
	}
	ofstream out_file;
	out_file.open("Output3.txt");
	out_file << sum;
	out_file.close();
	cout << "Maximum sum is" << sum;
}