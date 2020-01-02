/*
Author : Karthikeyan Ganesan
Date Modified : 20th September 2019
Class : ECE 6122

Description: This header file manipulates a matrix by using operator overloading 
*/
#pragma once
#include<iostream>
using namespace std;
class ECE_Matrix
{	
	double **matrix;
	int nRows, nCols;
	
public:
	//constructors

	ECE_Matrix(); //Default constructor
	ECE_Matrix(int a, double b);
	ECE_Matrix(int a, int b, double val);
	ECE_Matrix(string a);
	//destructor
	~ECE_Matrix();
 
	// copy constructor
	ECE_Matrix(ECE_Matrix const &); 


	ECE_Matrix& operator=(ECE_Matrix const&);

	ECE_Matrix operator+(ECE_Matrix const& ) const;
	ECE_Matrix operator+ (double const & ) const;
	friend ECE_Matrix operator+(double const &,  ECE_Matrix const &  );
	ECE_Matrix operator+=( ECE_Matrix const &);


	ECE_Matrix operator-(ECE_Matrix const&) const;
	ECE_Matrix operator-(double const &) const;
	friend ECE_Matrix operator-(double const &,  ECE_Matrix const & );
	ECE_Matrix operator-=( ECE_Matrix const &);



	ECE_Matrix operator*(double const &) const;
	
	friend ECE_Matrix operator*(double const&,  ECE_Matrix const & );

	ECE_Matrix operator /(double const &) const;
	ECE_Matrix transpose() const;

	friend ostream& operator<<(ostream& , const ECE_Matrix& );






};