#if !defined (VECTOR_H)
#define VECTOR_H
/**
*    Vector
*    Thomas Shaffer
*    16 October 2009
*
*
*/
#include "Matrix.h"
#include "Vertex.h"
class Vertex;
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
      double dotProduct( Vector* other );
      Vector* crossProduct( Vector* other );
      void normalize( );

};

#endif
