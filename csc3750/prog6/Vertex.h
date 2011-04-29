#if !defined (VERTEX_H)
#define VERTEX_H

#include "Color.h"
#include "Matrix.h"
#include "Vector.h"
#include "List.h"

class Vector;
class Face;
class Vertex
{
   private:
      Matrix* vertex;
      Color* color;
      List<Face>* faces;
     
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
      void addFace( Face* face );
      List<Face>* getFaces();
      Vertex* multiply( Matrix* mtrx );
      Vector* subtract( Vertex* v1 );
      void homogenize();
};

#endif
