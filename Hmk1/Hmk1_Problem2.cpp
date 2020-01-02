/*
Author Name : Karthikeyan Ganesan
Class : ECE6122
Last Date Modified : 9/5/2019

Description:
This problem implements numerical integration of a function using the midpoint integration rule. It takes the limits of the integral 
and the number of subdivisions ( thereby getting the step size ) and evaluvates the integral. Solution for Homework 1 Problem 2
*/

#include<iostream>
#include<math.h>
using namespace std;
float numerical_integration(double beta, unsigned long n)
{
	float  del_x = beta / n;
	float integral_value = 0;
	for (int i = 0; i < n; i++)
	{
		float present_x_2 = pow(i * del_x,2);
		integral_value += (4 / (1 + present_x_2)) * del_x;
	}
	return(integral_value);

}
int main()
{
	double beta; unsigned long N;
	cout << "Enter the limits of the integral and the total number of subdivisons" << endl;   
	cin >> beta >> N;
	if (beta < 0 || N < beta)
	{
		cout << "Enter valid inputs";				//check for input validity
	}
	else
	{
		float integral_evaluation = numerical_integration(beta, N);
		cout << "The value of the integral is " << integral_evaluation<<endl;		//Evaluate integral and display it
	}
	return 0;
}