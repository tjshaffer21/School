#include "Node.h"

Node::Node() : DeleteObject()
{
   transform = Matrix::getIdentityMatrix(4);
}

Node::~Node()
{
   delete transform;
}
