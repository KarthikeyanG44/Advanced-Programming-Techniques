/*
Author Name : Karthikeyan Ganesan
Class : ECE6122
Last Date Modified : 9/5/2019

Description:
This file takes in a positive integer number and approximates it to the nearest multiple of 7 by adding appropriate number of ones before dividing it by 
7. This process repeats itself till the end result is 1. Solution for Homework 1 Problem 1
*/

#include<iostream>
using namespace std;
// This recursive function counts the number of ones to be added to a given number to bring it up to the nearest multiple of 7. This function also updates the total number
//of ones that have been added so far
int count_ones(int num, int num_ones)
{
	int add_near_multiple;
	if (num != 1)
	{
		if (num % 7 == 0)
		{
			num /= 7;
			count_ones(num, num_ones);
		}
		else
		{
			add_near_multiple = 7 - (num % 7);
			num_ones += add_near_multiple;
			num += add_near_multiple;
			count_ones(num, num_ones);
		}
	}
	else
	{
		return(num_ones);
	}
}

int main()
{
	int input, init_ones = 0, total_ones;
	cout << "Enter a input number" << endl;
	cin >> input;
	if (input <= 0)
	{
		cout << "Enter a valid number greater than zero" << endl;		// This statement checks for any invalid inputs
	}
	else if (input == 1)
	{
		cout << "Input matches end of desired sequence and process terminating";  //If the end of the sequence is the input, the program does not execute 
	}
	else
	{
		total_ones = count_ones(input, init_ones);
		cout << "The sequence has" << total_ones << "instances of the number 1 being added" << endl;	//Prints the total number of ones added to end the process
	}
	return 0;
}
