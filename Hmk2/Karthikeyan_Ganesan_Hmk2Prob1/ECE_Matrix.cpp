/*
Author : Karthikeyan Ganesan
Date Modified : 20th September 2019
Class : ECE 6122

Description: This is the implementation file for the function descriptions in the header file "ECE_Matrix.h"
*/
#include<iostream>
#include<iomanip>
#include"ECE_Matrix.h"
#include<fstream>
#include<vector>
using namespace std;
ECE_Matrix::ECE_Matrix(ECE_Matrix const &in)
{
	this->nRows = in.nRows;
    this->nCols = in.nCols;
  	matrix = new double* [this->nRows];
	for (int i = 0; i < this->nRows; i++)
	{
		matrix[i] = new double[this->nCols]();
	}

    for(int i=0; i< this->nRows; i++)
    {
        for(int j=0; j< this->nCols; j++)
        {
            this->matrix[i][j] = in.matrix[i][j];
        }
    }
}

	
ECE_Matrix::ECE_Matrix()
{
	nRows=0;
	nCols=0;
	matrix = NULL;
}

ECE_Matrix::ECE_Matrix(int a, double b)
{
	int i, j;
	matrix = new double* [a];
	for (i = 0; i < a; i++)
	{
		matrix[i] = new double[a]();
	}
	for (i = 0;i<a;i++)
		for (j = 0; j < a; j++)
		{
			matrix[i][j] = b;
		}
	nRows = a;
	nCols = a;
}
ECE_Matrix::ECE_Matrix(int a, int b,double val)
{
	int i, j;
	matrix = new double* [a];
	for (i = 0; i < a; i++)
	{
		matrix[i] = new double[b]();
	}
	for (i = 0; i < a; i++)
		for (j = 0; j < b; j++)
		{
			matrix[i][j] = val;
		}
	nRows = a;
	nCols = b;
}
ECE_Matrix::ECE_Matrix(string s)
{
	ifstream file;
	file.open(s);
	vector<float> custom_matrix;
	while (!file.eof())
	{
		int read_data;
		file >> read_data;
		custom_matrix.push_back(read_data);
	}

	int k = 2;
	nRows = custom_matrix[0];
	nCols = custom_matrix[1];
	matrix = new double* [nRows];
	for (int i = 0; i < nRows; i++)
	{
		matrix[i] = new double[nCols]();
	}
	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
		{
			matrix[i][j] = custom_matrix[k];
			k = k + 1;
		}
				
}

ECE_Matrix::~ECE_Matrix()
{
    if(matrix != NULL)
	{
		for (int i = 0; i < nRows; i++)
		{
			delete[] matrix[i];
		}
		delete[] matrix;		
	}
        
}

ECE_Matrix& ECE_Matrix::operator=(const ECE_Matrix & M1)
{
	// Check to see if we are trying to assign ourself
    if (this == &M1)
        return *this;

    // Copy over everything
    this->nRows = M1.nRows;
    this->nCols = M1.nCols;
	matrix = new double* [nRows];
	for (int i = 0; i < nRows; i++)
	{
		matrix[i] = new double[nCols]();
	}

    for(int i=0; i< this->nRows; i++)
    {
        for(int j=0; j< this->nCols; j++)
        {
            this->matrix[i][j] = M1.matrix[i][j];
        }
    }
    // Return
    return *this;
}



ECE_Matrix ECE_Matrix::operator+(ECE_Matrix const &M1) const
{
	// first compare dimensions
	int newRow=0;
	int newCol = 0;
	if(this->nRows > M1.nRows)
	{
		newRow = this->nRows;
	}
	else
	{
		newRow = M1.nRows;
	}
	if(this->nCols > M1.nCols)
	{
		newCol = this->nCols;
	}
	else
	{
		newCol = M1.nCols;
	}

	ECE_Matrix M3(newRow, newCol, 0);



    for (int i=0; i < newRow; i++)
    {
        for (int j=0; j < newCol; j++)
        {
            if(i < this->nRows && j < this->nCols 
                && i < M1.nRows && j < M1.nCols)
            {
                M3.matrix[i][j]
                    = this->matrix[i][j]
                        + M1.matrix[i][j];
            }
            else if(i < this->nRows && j < this->nCols)
            {
                M3.matrix[i][j]
                    = this->matrix[i][j];
            }
            else if(i < M1.nRows && j < M1.nCols)
            {
                 M3.matrix[i][j]
                    = M1.matrix[i][j];
            }
        }
    }

	return(M3);
}

ECE_Matrix ECE_Matrix::operator+(double  const &n) const
{
	ECE_Matrix M3(*this);

	for (int i=0;i<nRows;i++)
		for (int j = 0; j < nCols; j++)
		{
			M3.matrix[i][j] += n;
		}
	return(M3);
	
}

ECE_Matrix operator+(double const &n,  ECE_Matrix const & M1)
{
	ECE_Matrix M3(M1);
	for (int i = 0; i < M3.nRows; i++)
		for (int j = 0; j < M3.nCols; j++)
		{
			M3.matrix[i][j] = n + M1.matrix[i][j];
		}
	return(M3);
	
}

ECE_Matrix ECE_Matrix::operator+=( ECE_Matrix const &M1)
{

	// first compare dimensions
	int newRow=0;
	int newCol = 0;
	if(this->nRows > M1.nRows)
	{
		newRow = this->nRows;
	}
	else
	{
		newRow = M1.nRows;
	}
	if(this->nCols > M1.nCols)
	{
		newCol = this->nCols;
	}
	else
	{
		newCol = M1.nCols;
	}

	double **temp = new double* [newRow];
	for (int i = 0; i < newRow; i++)
	{
		temp[i] = new double[newCol]();
	}

	for (int i=0; i < newRow; i++)
    {
        for (int j=0; j < newCol; j++)
        {
            if(i < this->nRows && j < this->nCols 
                && i < M1.nRows && j < M1.nCols)
            {
                temp[i][j]
                    = this->matrix[i][j]
                        + M1.matrix[i][j];
            }
            else if(i < this->nRows && j < this->nCols)
            {
                temp[i][j]
                    = this->matrix[i][j];
            }
            else if(i < M1.nRows && j < M1.nCols)
            {
                 temp[i][j]
                    = M1.matrix[i][j];
            }
        }
    }

	for (int i = 0; i < this->nRows; i++)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
	this->nRows = newRow;
	this->nCols = newCol;
	this->matrix = temp;
	return (*this);	
}

ECE_Matrix ECE_Matrix::operator-( ECE_Matrix const &M1) const
{
	// first compare dimensions
	int newRow=0;
	int newCol = 0;
	if(this->nRows > M1.nRows)
	{
		newRow = this->nRows;
	}
	else
	{
		newRow = M1.nRows;
	}
	if(this->nCols > M1.nCols)
	{
		newCol = this->nCols;
	}
	else
	{
		newCol = M1.nCols;
	}

	ECE_Matrix M3(newRow, newCol, 0);



    for (int i=0; i < newRow; i++)
    {
        for (int j=0; j < newCol; j++)
        {
            if(i < this->nRows && j < this->nCols 
                && i < M1.nRows && j < M1.nCols)
            {
                M3.matrix[i][j]
                    = this->matrix[i][j]
                        - M1.matrix[i][j];
            }
            else if(i < this->nRows && j < this->nCols)
            {
                M3.matrix[i][j]
                    = this->matrix[i][j];
            }
            else if(i < M1.nRows && j < M1.nCols)
            {
                 M3.matrix[i][j]
                    = -M1.matrix[i][j];
            }
        }
    }

	return(M3);
}

ECE_Matrix ECE_Matrix::operator-(double const &n) const
{
	ECE_Matrix M3(*this);
	
	for (int i=0;i<nRows;i++)
		for (int j = 0; j < nCols; j++)
		{
			M3.matrix[i][j] -= n;
		}
	return(M3);
}

ECE_Matrix operator-(double const &n,  ECE_Matrix const & M1)
{
	ECE_Matrix M3(M1);
	for (int i = 0; i < M3.nRows; i++)
		for (int j = 0; j < M3.nCols; j++)
		{
			M3.matrix[i][j] = n - M1.matrix[i][j];
		}
	return(M3);
}

ECE_Matrix ECE_Matrix::operator-=( ECE_Matrix const &M1)
{

	// first compare dimensions
	int newRow=0;
	int newCol = 0;
	if(this->nRows > M1.nRows)
	{
		newRow = this->nRows;
	}
	else
	{
		newRow = M1.nRows;
	}
	if(this->nCols > M1.nCols)
	{
		newCol = this->nCols;
	}
	else
	{
		newCol = M1.nCols;
	}

	double **temp = new double* [newRow];
	for (int i = 0; i < newRow; i++)
	{
		temp[i] = new double[newCol]();
	}

	for (int i=0; i < newRow; i++)
    {
        for (int j=0; j < newCol; j++)
        {
            if(i < this->nRows && j < this->nCols 
                && i < M1.nRows && j < M1.nCols)
            {
                temp[i][j]
                    = this->matrix[i][j]
                        - M1.matrix[i][j];
            }
            else if(i < this->nRows && j < this->nCols)
            {
                temp[i][j]
                    = this->matrix[i][j];
            }
            else if(i < M1.nRows && j < M1.nCols)
            {
                 temp[i][j]
                    = -M1.matrix[i][j];
            }
        }
    }

	for (int i = 0; i < this->nRows; i++)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
	this->nRows = newRow;
	this->nCols = newCol;
	this->matrix = temp;
	return (*this);	
}

ECE_Matrix ECE_Matrix::operator*(double const &n) const
{
	ECE_Matrix M3(*this);
	for(int i=0;i<M3.nRows;i++)
		for (int j = 0; j < M3.nCols; j++)
		{
			M3.matrix[i][j] *= n;
		}
	return(M3);
}

ECE_Matrix operator*(double const &n,  ECE_Matrix const & M1)
{
	ECE_Matrix M3(M1);
	for(int i=0;i<M3.nRows;i++)
		for (int j = 0; j < M3.nCols; j++)
		{
			M3.matrix[i][j] *= n;
		}
	return(M3);
}
ECE_Matrix ECE_Matrix::operator/(double const &n) const
{
	ECE_Matrix M3(*this);
	if (n == 0)
	{
		for (int i = 0; i < M3.nRows; i++)
			for (int j = 0; j < M3.nCols; j++)
			{
				M3.matrix[i][j] =0;
			}
	}
	else
	{
		for (int i = 0; i < M3.nRows; i++)
			for (int j = 0; j < M3.nCols; j++)
			{
				M3.matrix[i][j] /= n;
			}
	}
	return(M3);
}


ECE_Matrix ECE_Matrix::transpose() const
{
	ECE_Matrix  M3(this->nCols, this->nRows, 0);
	for (int i = 0; i < this->nCols; i++)
		for (int j = 0; j <this->nRows; j++)
			M3.matrix[i][j] = this->matrix[j][i];
	return(M3);
}
ostream& operator<<(ostream& disp, const ECE_Matrix& M1)
{
	for (int i = 0; i < M1.nRows; i++)
	{
		for (int j = 0; j < M1.nCols; j++)
			disp << scientific << setw(12) << setprecision(4) << M1.matrix[i][j];
		disp << endl;	
	}
	return(disp);
}
