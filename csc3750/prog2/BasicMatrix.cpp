#include "BasicMatrix.h"
#include <iostream>
using namespace std;

BasicMatrix::BasicMatrix(int row, int column)
{
   if (row < 1)
   {
      row = 1;
   }

   if (column < 1)
   {
      column = 1;
   }
 
   rows = row;
   columns = column;

   matrix = new double*[rows];  //array of pointers

   //initialize the matrix to contain all zeroes
   for (int i = 0; i < rows; i++)
   {
      double* temp = new double[columns];  //array of doubles
      matrix[i] = temp;
      for (int j = 0; j < columns; j++)
      {
         matrix[i][j] = 0;
      }
   }
}

BasicMatrix::~BasicMatrix()
{
   int rows = getNumRows();

   for (int i = 0; i < rows; i++)
   {
      delete[] matrix[i];
   }

   delete[] matrix;
}

int BasicMatrix::getNumRows()
{
   return rows;
}

int BasicMatrix::getNumColumns()
{
   return columns;
}

void BasicMatrix::setElement(int row, int column, double value)
{
   checkInput(row, column);
   matrix[row - 1][column - 1] = value;
}

double BasicMatrix::getElement(int row, int column)
{
   checkInput(row, column);
   return matrix[row - 1][column - 1];
}

void BasicMatrix::checkInput(int& row, int& column)
{
    if (row > rows || row < 1 || column > columns || column < 1)
    {
       row = rows;
       column = columns;
    }
}

void BasicMatrix::printMatrix()
{
   for (int i = 1; i <= rows; i++)
   {
      for (int j = 1; j <= columns; j++)
      {
         cout << getElement(i, j) << "   ";
      }
      cout << endl;
   }
}
