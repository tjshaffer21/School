/**
*    Vector
*    Thomas Shaffer
*    27 Nov. 2009
*
*    A vector object is a 4x1 Matrix with the values (x,y,z,0).
*/
#include "Vector.h"

Vector::Vector(double x, double y, double z)
{
   vector = new Matrix(4, 1);
   vector->setElement(1, 1, x);
   vector->setElement(2, 1, y);
   vector->setElement(3, 1, z);
   vector->setElement(4, 1, 0.0);  //4th coordinate 0 for vectors
}

Vector::~Vector() { delete vector; }

// set methods - provided by Dr. Boshart
void Vector::setX(double x){ vector->setElement(1, 1, x); }
void Vector::setY(double y){ vector->setElement(2, 1, y); }
void Vector::setZ(double z){ vector->setElement(3, 1, z); }
 
// get methods - provided by Dr. Boshart
double Vector::getX() { return vector->getElement(1, 1); }
double Vector::getY() { return vector->getElement(2, 1); }
double Vector::getZ() { return vector->getElement(3, 1); }

void Vector::printVector() { vector->printMatrix(); }

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
*       The vector to perform the cross product on.
*    Postconditions:
*       Returns a Vector pointer to the resulting Vector.
*/
Vector* Vector::crossProduct( Vector* other )
{
    double x = (this->getY()*other->getZ()) - (this->getZ()*other->getY() );
    double y = (this->getZ()*other->getX()) - (this->getX()*other->getZ() );
    double z = (this->getX()*other->getY()) - (this->getY()*other->getX() );

    return (new Vector(x, y, z) );
}

/**
*    normalize - destructive method.
*    Normalizes the vector.
*    Preconditions:
*        None.
*    Postconditions:
*        Modified the vector by replacing the elements with their normalized
*        version.
*/
void Vector::normalize()
{
    double len = (this->getX()*this->getX()) + (this->getY()*this->getY()) +
        (this->getZ()*this->getZ());

    len = sqrt( len );

    this->setX( this->getX()/len );
    this->setY( this->getY()/len );
    this->setZ( this->getZ()/len );
}

/**
*    addition
*    Performs addition on two vectors to create a new vector.
*    Preconditions:
*        The vector to multiply with.
*    Postconditions:
*        Returns a Vector pointer to the new Vector that results from the
*        addition.
*/
Vector* Vector::addition( Vector* other )
{
    double x = (this->getX() + other->getX());
    double y = (this->getY() + other->getY());
    double z = (this->getZ() + other->getZ());
    
    return new Vector( x, y, z );
}

/**
*    division
*    Divides each element in the vector by the specified amount.
*    Preconditions:
*        A double that is to be divided by.
*    Postconditions:
*        Returns a Vector pointer to the new Vector from the result 
*        of the division.
*/
Vector* Vector::division( double amount )
{
    double x = this->getX()/amount;
    double y = this->getY()/amount;
    double z = this->getZ()/amount;

    return new Vector( x, y, z );
}

/**
*    multiply
*    Multiplies a vector with a matrix.
*    Preconditions:
*        A Matrix pointer to multiply the Vector by.
*    Postconditions:
*        Return a Vector pointer to the new Vector.
*/
Vector* Vector::multiply( Matrix* mtrx )
{
    Vector* vrt = new Vector( 0, 0, 0 );
    
    Matrix* inv = mtrx->inverse();
    Matrix* trans = inv->transpose();
    delete inv;
    
    Matrix* tmp = trans->multiply( vector );
    delete trans;
    
    vrt->setX( tmp->getElement(1, 1) );
    vrt->setY( tmp->getElement(2, 1) );
    vrt->setZ( tmp->getElement(3, 1) );

    delete tmp;
    return vrt;
}
