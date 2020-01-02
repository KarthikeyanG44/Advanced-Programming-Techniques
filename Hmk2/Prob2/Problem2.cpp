/*
Author Name : Karthikeyan Ganesan
Class : ECE6122
Last Date Modified : 20th September 2019

Description:
This file reads in a input text file consists of numbers in a MXN matrix and computes the largest product of the four consecutive numbers in horizontal/vertical/diagonal direction
*/
#include<iostream>
#include<algorithm>
#include<fstream>
#include<vector>
using namespace std;
double access_matrix_element(double **arr,int j,int i,int m,int n) {

	if (j >=0  && j<n && i >=0  && i<m) {
		return arr[j][i];
	}
	return (0);
}

double find_largest_product(double **arr, int k,int m,int n) {

	double max_product = 0;
	for (int j = 0; j < n; j++) {
		for (int i = 0; i < n; i++) {

			double p1 = 1, p2 = 1, p3 = 1, p4 = 1;

			for (int z = 0;  z< k; z++) {
				p1 *= access_matrix_element(arr, j, i + z,m,n);
				p2 *= access_matrix_element(arr, j+z,i,m,n);
				p3 *= access_matrix_element(arr, j+z,i+z,m,n);
				p4 *= access_matrix_element(arr, j+z , j-z,m,n);
			}
			max_product = max(max_product, max(p1, max(p2, max(p3, p4))));
		}
	}
	return max_product;
}
int main()
{
	double **matrix;
	int m, n,k=2,i,j;
	string s;
	cout << "Enter the file name";
	cin >> s;
	ifstream file;
	file.open(s);
	vector<int> numbers;
	while (!file.eof())
	{
		int a;
		file >> a;
		numbers.push_back(a);
	}
	file.close();
	m = numbers[0];
	n = numbers[1];
	matrix = new double* [m];
	for (int i = 0; i < m; i++)
		matrix[i] = new double[n];
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
		{
			matrix[i][j] = numbers[k++];
		}
	ofstream opfile;
	opfile.open("Output2.txt");
	opfile << find_largest_product(matrix,4,m, n);
	cout << find_largest_product(matrix, 4, m, n);
	opfile.close();
}