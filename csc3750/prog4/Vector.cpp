#include "Vector.h"
#include <math.h>

Vector::Vector(double x, double y, double z)
{
   vector = new Matrix(4, 1);
   vector->setElement(1, 1, x);
   vector->setElement(2, 1, y);
   vector->setElement(3, 1, z);
   vector->setElement(4, 1, 0.0);  //4th coordinate 0 for vectors
}

Vector::~Vector()
{
   delete vector;
}

void Vector::setX(double x)
{
   vector->setElement(1, 1, x);
}

void Vector::setY(double y)
{
   vector->setElement(2, 1, y);
}

void Vector::setZ(double z)
{
   vector->setElement(3, 1, z);
}

double Vector::getX()
{
   return vector->getElement(1, 1);
}

double Vector::getY()
{
   return vector->getElement(2, 1);
}

double Vector::getZ()
{
   return vector->getElement(3, 1);
}

void Vector::printVector()
{
   vector->printMatrix();
}

/**
*    dotProduct
*    Performs dot product on the two Vertices.
*    Preconditions:
*        The vector to perform the manipulation on.
*    Postconditions:
*        Returns the result of the dot product.
*/
double Vector::dotProduct( Vector* other )
{
    double x = this->getX() * other->getX();
    double y = this->getY() * other->getY();
    double z = this->getZ() * other->getZ();

    
    return (x+y+z);
}

/**
*    crossProduct
*    Performs cross product on two Vectors.
*    Preconditions:
*       The vector the perform the cross product on.
*    Postconditions:
*       Returns the result of the cross product.
*/
Vector* Vector::crossProduct( Vector* other )
{
    double x = (this->getY()*other->getZ()) - (this->getZ()*other->getY() );
    double y = (this->getZ()*other->getX()) - (this->getX()*other->getZ() );
    double z = (this->getX()*other->getY()) - (this->getY()*other->getX() );

    return (new Vector(x, y, z) );
}

/**
*    normalize
*
*    Preconditions:
*
*    Postconditions:
*
*/
void Vector::normalize()
{
    double len = (this->getX()*this->getX()) + 
        (this->getY()*this->getY()) + 
        (this->getZ()*this->getZ());

    len = sqrt( len );

    this->setX( this->getX()/len );
    this->setY( this->getY()/len );
    this->setZ( this->getZ()/len );
}


