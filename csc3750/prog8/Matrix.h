/** Class provided by Dr. Boshart. */
#if !defined (MATRIX_H)
#define MATRIX_H

#include "BasicMatrix.h"

class Matrix : public BasicMatrix
{
   private:
      static void substitute(double** a, int* o, int& n, double* b, double* x);
      static void pivot(double** a, int* o, double* s, int& n, int& k);
      static int decompose(double** a, int& n, double& tol, int* o, double* s);
      static double** invert(double** a, int& length);

   public:
      Matrix(int row, int column);
      virtual ~Matrix();
      static Matrix* getIdentityMatrix(int size);
      Matrix* add(Matrix* otherMatrix);
      Matrix* multiply(Matrix* otherMatrix);
      Matrix* transpose();
      static Matrix* toMatrix(double** array, int& rows, int& cols);
      Matrix* inverse();
      const float* toArray();  //return the Matrix as a column vector of floats for OpenGL
      double** toArray(Matrix* m);
};

#endif
