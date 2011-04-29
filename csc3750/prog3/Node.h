#if !defined (NODE_H)
#define NODE_H

#include "Matrix.h"
#include "Pixel.h"
#include "DeleteObject.h"

class Node : public DeleteObject
{
   private:

   protected:
      Matrix* transform;

   public:
      Node();
      ~Node();
      virtual void render(Pixel* px, Matrix* matrix) = 0;
};

#endif
