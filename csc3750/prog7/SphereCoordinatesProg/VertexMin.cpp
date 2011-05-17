#include "VertexMin.h"

VertexMin::VertexMin(double x, double y, double z)
{
   vertex = new Matrix(4, 1);

   vertex->setElement(1, 1, x);
   vertex->setElement(2, 1, y);
   vertex->setElement(3, 1, z);
   vertex->setElement(4, 1, 1.0);  //4th coordinate 1 for vertices
}

VertexMin::~VertexMin()
{
    delete vertex;
}

void VertexMin::setX(double x) { vertex->setElement(1, 1, x); }
void VertexMin::setY(double y) { vertex->setElement(2, 1, y); }
void VertexMin::setZ(double z) { vertex->setElement(3, 1, z); }
void VertexMin::setH(double h) { vertex->setElement(4, 1, h); }

double VertexMin::getX() { return vertex->getElement(1, 1); }
double VertexMin::getY() { return vertex->getElement(2, 1); }
double VertexMin::getZ() { return vertex->getElement(3, 1); }
double VertexMin::getH() { return vertex->getElement(4, 1); }
