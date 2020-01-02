/*
Author Name : Karthikeyan Ganesan
Class : ECE6122
Last Date Modified : 9/5/2019

Description:
 This file finds the arrangement of soliders (identified by an integer) around a round table such that the sum of the 
 adjacent identifiers of soliders sum to a Fibonacci number. Solution for Homework 1 Problem 3
*/

#include <iostream>
#include<algorithm>
#include<math.h>
using namespace std;
// This function checks whether a given number is a perfect square or not 
bool check_perf_square(int n)
{
	int root = sqrt(n);
	return(root * root == n);
}
// This function checks whether the input number is a Fibonacci number by checking whether (5n^2+4) or (5n^2-4) is a perfect square
bool check_fib_num(int n)
{
	return(check_perf_square(5 * pow(n,2) + 4) || check_perf_square(5 * pow(n,2) - 4));
}
// This function checks whether a given sequence of numbers satisfies the given condition 
bool check_seq_valid(int knights[], int size)
{
	for (int i = 0; i < size-1; i++)
	{
		int sum = knights[i] + knights[i + 1];
		if (check_fib_num(sum))
		{
			continue;
		}
		else
		{
			return(false);
		}
	}
	return(true);
}
// This function is used to calculate the factorial of  a given number 
double factorial(int n)
{
	if (n == 1)
	{
		return(1);
	}
	else
	{
		return(n * factorial(n - 1));
	}
}
int main()
{
	unsigned long N;
	int i;
	int combos_checked = 0;
	cout << "Enter the number of knights" << endl;
	cin >> N;
	int knight_numbers[100];
	for (i = 0; i < N; i++)
	{
		knight_numbers[i] = i + 1;        //Initialize the knights array with ascending order till number of soldiers
	}
	while (!check_seq_valid(knight_numbers, N))       //Loop over till the valid sequence of numbers is found
	{
		combos_checked++;
		if (next_permutation(knight_numbers,knight_numbers+N))
		{
			continue;													//if a higher combination is found check for validity 
		}
		else
		{
			break;
		}
	}
	if (combos_checked == factorial(N))
	{
		cout << "No solution found" << endl;					// if none of the combinations satisfy 
	}
	else
	{
		cout << "The knights should sit clockwise as follows:K";
		for (i = 0; i < N; i++)
		{
			cout << knight_numbers[i];			//display the least permutation that satisfies 
		}
	}
	
}