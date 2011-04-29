#if !defined (NODE_H)
#define NODE_H

#include "Matrix.h"
#include "Pixel.h"
#include "DeleteObject.h"

class Light;
class Vertex;
class Node : public DeleteObject
{
   private:

   protected:
      Matrix* transform;

   public:
      Node();
      ~Node();
      virtual void render(Pixel* px, Matrix* matrix, Matrix* zbuffer,
          Light* light, Vertex* eye, Color* amb, double attenuation) = 0;
};

#endif
