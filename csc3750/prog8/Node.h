/**  Class provided by Dr. Boshart. */
#if !defined (NODE_H)
#define NODE_H

#include "Matrix.h"
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
      virtual void render(Matrix* mtrx ) = 0;
};

#endif
