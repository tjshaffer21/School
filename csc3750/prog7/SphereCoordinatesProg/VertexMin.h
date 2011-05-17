#if !defined (VERTEXMIN_H)
#define VERTEXMIN_H

#include "Matrix.h"
#include "List.h"

class VertexMin
{
   private:
      Matrix* vertex;
     
   public:
      VertexMin(double x, double y, double z);
      VertexMin( Matrix* mtrix );
      virtual ~VertexMin();
      void setX(double x);
      void setY(double y);
      void setZ(double z);
      void setH(double h);
      double getX();
      double getY();
      double getZ();
      double getH();
};

#endif
