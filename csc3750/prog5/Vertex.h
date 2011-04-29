#if !defined (VERTEX_H)
#define VERTEX_H

#include "Color.h"
#include "Matrix.h"
#include "Vector.h"

class Vector;

class Vertex
{
   private:
      Matrix* vertex;
      Color* color;
     
   public:
      Vertex(double x, double y, double z);
      Vertex( Matrix* mtrix );
      virtual ~Vertex();
      void setX(double x);
      void setY(double y);
      void setZ(double z);
      void setH(double h);
      double getX();
      double getY();
      double getZ();
      double getH();
      void setColor( Color* clr );
      void setRed(double x);
      void setGreen(double y);
      void setBlue(double z);
      double getRed();
      double getGreen();
      double getBlue();
      void printVertex();
      Vertex* multiply( Matrix* mtrx );
      Vector* subtract( Vertex* v1 );
      void homogenize();
};

#endif
