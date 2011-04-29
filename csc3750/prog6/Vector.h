#if !defined (VECTOR_H)
#define VECTOR_H
/**
*    Vector
*    Thomas Shaffer
*    16 October 2009
*    Modified: 9 Nov. 2009
*    Final:    ?? 2009
*
*    A vector object is a 4x1 Matrix with the values (x,y,z,0).
*/
#include <math.h>
#include "Matrix.h"

class Vector
{
   private:
      Matrix* vector;

   public:
      Vector(double x, double y, double z);
      ~Vector();
      void setX(double x);
      void setY(double y);
      void setZ(double z);
      double getX();
      double getY();
      double getZ();
      void printVector();
      void normalize( );
      double dotProduct( Vector* other );
      Vector* crossProduct( Vector* other );
      Vector* addition( Vector* other );
      Vector* division( double amount );
      Vector* multiply( Matrix* mtrx );
};

#endif
