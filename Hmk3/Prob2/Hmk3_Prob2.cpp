/*
Author Name : Karthikeyan Ganesan
Class : ECE6122
Last Date Modified : 6th October 2019

Description:
This file reads a matrix of arbitrary size and returns the largest product of 4 adjacent numbers in the matrix. This program uses multi threading operations to acheive the same
*/
#include<iostream>
#include<omp.h>
#include<vector>
#include<fstream>
#include<algorithm>
#include<string>
using namespace std;
//This function takes in a matrix of size m*n and returns elements at location (i,j)
double access_matrix_element(vector<vector<int>> mat, int i, int j, int m, int n)
{

	if (j >=0  && j<n && i >=0  && i<m) 
	  return(mat[i][j]);
 else
	  return(0);
}
int main(int argc,char * argv[])
{
	//Declare variables
	vector<int> read_ip;
  vector<double> local_max;
	vector<vector<int>> matrix;
	int m, n, i,j,k=0,num_adj = 4;
	long double max_product = 0;
	ifstream file;
 
	//Check for number of arguments
	if (argc < 2)
	{
		cout << "Enter input file name!!";
   return(0);
   
	}
 
  
	// Assign input file name to variable and open file
  string file_name(argv[1]);
	file.open(file_name);
 
 
	//Read contents of file into vector
	while (!file.eof())
	{
		int a;
		file >> a;
		read_ip.push_back(a);
	}  
 
 
	// Define the size of the matrix after reading the file and read matrix elements
	m = read_ip[k];++k;
	n = read_ip[k];++k;
  matrix.resize(m,vector<int>(n));
	for (i=0;i<m;i++)
 {
		for (j = 0; j < n; j++)
			matrix[i][j] = read_ip[k++];
  }
   
   
// calculate the horizontal,vertical and diagonal products for each element in separate threads
#pragma omp for collapse(2) 
  for (i = 0; i < m; i++) 
  {
  		for (j = 0; j < n; j++) 
      {
  			double p1 = 1, p2 = 1, p3 = 1, p4 = 1;
  			for (int z = 0; z< num_adj; z++) 
        {  
          p1 *= access_matrix_element(matrix,i,j,m,n);
  				p2 *= access_matrix_element(matrix, i+z,j,m,n);
  				p3 *= access_matrix_element(matrix,i+z,j+z,m,n);
  				p4 *= access_matrix_element(matrix,i+z ,j-z,m,n);
  			}
        double max_prod =  max(p1, max(p2, max(p3, p4)));
        local_max.push_back(max_prod);                              //store the local maximum product of adjacent 4 elements in a vector 
  		}
    }

//Find the maximum of the local maximum products and save it to a text file
  max_product = *(max_element(local_max.begin(),local_max.end()));
  cout<<"Maximum Product"<<max_product;
  ofstream opfile;
	opfile.open("Output2.txt");
  opfile << max_product;
  opfile.close();
	return(0);
}