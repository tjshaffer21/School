#include "Matrix.h"
#include "BasicMatrix.h"
#include <math.h>

Matrix::Matrix(int row, int column) : BasicMatrix(row, column)
{}

Matrix::~Matrix(){}

Matrix* Matrix::getIdentityMatrix(int size)
{
   Matrix* matrix = new Matrix(size, size);  //all zeroes
   for (int i = 0; i < size; i++)  //insert 1's on the diagonal
   {
      matrix->setElement(i, i, 1.0);
   }
   return matrix;
}

Matrix* Matrix::add(Matrix* otherMatrix)
{
   int rows = getNumRows();
   int cols = getNumColumns();

   if( getNumRows() != otherMatrix->getNumRows() || getNumColumns() != otherMatrix->getNumColumns())
   {
      return new Matrix(1, 1);
   }

   Matrix* result = new Matrix(rows, cols);    

   for (int i = 1; i <= rows; i++)
   {
      for (int j = 1; j <= cols; j++)
      {
         result->setElement(i, j, getElement(i, j) + otherMatrix->getElement(i, j));
      }
   }

   return result;  
}

Matrix* Matrix::multiply(Matrix* otherMatrix)
{
   if (getNumColumns() != otherMatrix->getNumRows())
   {
      return new Matrix(1, 1);
   }

   Matrix* result = new Matrix(getNumRows(), otherMatrix->getNumColumns());

   int rows = result->getNumRows();
   int cols = result->getNumColumns();

   double x;

   //loop over all elements of resulting matrix
   for (int i = 1; i <= rows; i++)
   {
      for (int j = 1; j <= cols; j++)
      {
         x = 0;
         //sum up multiplying matrices to obtain value placed in new matrix
         for (int k = 1; k <= getNumColumns(); k++)
         {
            x = x + getElement(i, k) * otherMatrix->getElement(k, j);
         }
         result->setElement(i, j, x);
      }
   }

   return result;
}

Matrix* Matrix::transpose()
{
   int rows = getNumRows();
   int cols = getNumColumns();
   Matrix* result = new Matrix(cols, rows);

   for (int i = 1; i <= rows; i++)
   {
      for (int j = 1; j <= cols; j++)
      {
         result->setElement(j, i, getElement(i, j));
      }
   }
    
   return result;
}

void Matrix::pivot(double** a, int* o, double* s, int& n, int& k)
{
   int p = k;

   double big = fabs(a[o[k - 1] - 1][k - 1]/s[o[k - 1] - 1]);
   double dummy;

   for (int i = k; i < n; i++)
   {
      dummy = fabs(a[o[i - 1] - 1][k - 1]/s[o[i - 1] - 1]);
      if (dummy > big)
      {
         big = dummy;
         p = i;
      }
   }

   int dum = o[p - 1];
   o[p - 1] = o[k - 1];
   o[k - 1] = dum;
}

void Matrix::substitute(double** a, int* o, int& n, double* b, double* x)
{
   double sum;
   double factor;

   for (int i = 2; i <= n; i++)
   {
      sum = b[o[i - 1] - 1];
      for (int j = 1; j <= i - 1; j++)
      {
         sum = sum - a[o[i - 1] - 1][j - 1] * b[o[j - 1] - 1];
      }
      b[o[i - 1] - 1] = sum;
   }

   x[n - 1] = b[o[n - 1] - 1]/(a[o[n - 1] - 1][n - 1]);
   for (int i = n - 1; i >= 1; i--)
   {
      sum = 0;
      for (int j = i + 1; j <= n; j++)
      {
         sum = sum + a[o[i - 1] - 1][j - 1] * x[j - 1];
      }
      x[i - 1] = (b[o[i - 1] - 1] - sum) / a[o[i - 1] - 1][i - 1];
   }
}

int Matrix::decompose(double** a, int& n, double& tol, int* o, double* s)
{

   for(int i = 1; i <= n; i++)
   {
      o[i - 1] = i;
      s[i - 1] = fabs(a[i - 1][1 - 1]);
      for (int j = 2; j <= n; j++)
      {
         if (fabs(a[i - 1][j - 1]) > s[i - 1])
         {
            s[i - 1] = fabs(a[i - 1][j - 1]);
         }
      }
   }

   for (int k = 1; k <= n - 1; k++)
   {
      pivot(a, o, s, n, k);

      if (fabs(a[o[k - 1] - 1][k - 1] / s[o[k - 1] - 1]) < tol)
      {
        return -1;
      }

      for (int i = k + 1; i <= n; i++)
      {
         double factor = a[o[i - 1] - 1][k - 1] / a[o[k - 1] - 1][k - 1];
         a[o[i - 1] - 1][k - 1] = factor;

         for (int j = k + 1; j <= n; j++)
         {
            a[o[i - 1] - 1][j - 1] = a[o[i - 1] - 1][j - 1] - factor * a[o[k - 1] - 1][j - 1];
         }

      }
   }

   if (fabs(a[o[n - 1] - 1][n - 1] / s[o[n - 1] - 1]) < tol)
   {
       return -1;
   }

   return 0;
}

double** Matrix::invert(double** a, int& length) 
{
   int n = length;
   double tol = .00000001;
   int er;
   int* o = new int[n];
   double* s = new double[n];
   double* b = new double[n];
   double* x = new double[n];
   double** ai = new double*[n];

   for (int x = 0; x < n; x++)
   {
      double* temp = new double[n];
      ai[x] = temp;
   }

   er = decompose(a, n, tol, o, s);

   if (er == -1)  //can't be inverted
   {
      return a;
   }

   else
   {
      for (int i = 1; i <= n; i++)
      {
         for(int j = 1; j <= n; j++)
         {
            if (i == j)
            {
               b[j - 1] = 1;
            }
            else
            {
               b[j - 1] = 0;
            }
         }

         substitute(a, o, n, b, x);

         for(int j = 1; j <= n; j++)
         {
            ai[j - 1][i - 1] = x[j - 1];
         }
      }
  
   }

   delete[] s;
   delete[] x;
   delete[] b;
   delete[] o;

   return ai;
}

Matrix* Matrix::toMatrix(double** array, int& rows, int& cols)
{
   Matrix* mat = new Matrix(rows, cols);

   for (int i = 1; i <= rows; i++)
   {
      for (int j = 1; j <= cols; j++)
      {
         mat->setElement(i, j, array[i - 1][j - 1]);
      }
   }

   return mat;
}

double** Matrix::toArray(Matrix* m)
{
   int rows = m->getNumRows();
   int cols = m->getNumColumns();

   double** array = new double*[rows];

   for (int x = 0; x < cols; x++)
   {
      double* temp = new double[cols];
      array[x] = temp;
   }

   for (int i = 1; i <= rows; i++)
   {
      for (int j = 1; j <= cols; j++)
      {
         array[i - 1][j - 1] = m->getElement(i, j);
      }
   }

   return array;
}

Matrix* Matrix::inverse()
{
   int rows = getNumRows();
   int cols = getNumColumns();

   if (rows != cols)
   {
      return new Matrix(1, 1);
   }

   double** a = toArray(this);
   double** ai = invert(a, rows);  //assumed to be square

   Matrix* inverse = toMatrix(ai, rows, cols);

   for (int x = 0; x < rows; x++)
   {
      delete[] a[x];
      delete[] ai[x];
   }

   delete[] a;
   delete[] ai;

   return inverse;
}
