/**
*    Vertex
*    27 Nov. 2009
*    Written by Dr. Boshart
*    Modified by Thomas Shaffer
*
*    Class provides methods for handling a vertex.
*/
#include "Vertex.h"

Vertex::Vertex(double x, double y, double z)
{
   color = new Color(1, 1, 1);
   vertex = new Matrix(4, 1);

   vertex->setElement(1, 1, x);
   vertex->setElement(2, 1, y);
   vertex->setElement(3, 1, z);
   vertex->setElement(4, 1, 1.0);  //4th coordinate 1 for vertices
}

Vertex::~Vertex()
{
    delete color;
    delete vertex;
}

// set - provide by Dr. Boshart
void Vertex::setX(double x) { vertex->setElement(1, 1, x); }
void Vertex::setY(double y) { vertex->setElement(2, 1, y); }
void Vertex::setZ(double z) { vertex->setElement(3, 1, z); }
void Vertex::setH(double h) { vertex->setElement(4, 1, h); }
void Vertex::setRed(double r) { color->setRed(r); }
void Vertex::setGreen(double g) { color->setGreen(g); }
void Vertex::setBlue(double b) { color->setBlue(b); }

// get - provided by Dr. Boshart.
double Vertex::getX() { return vertex->getElement(1, 1); }
double Vertex::getY() { return vertex->getElement(2, 1); }
double Vertex::getZ() { return vertex->getElement(3, 1); }
double Vertex::getH() { return vertex->getElement(4, 1); }
double Vertex::getRed() { return color->getRed(); }
double Vertex::getGreen() { return color->getGreen(); }
double Vertex::getBlue() { return color->getBlue(); }
void Vertex::printVertex() { vertex->printMatrix(); }

void Vertex::setColor( Color* clr )
{
    this->setRed( clr->getRed() );
    this->setGreen( clr->getGreen() );
    this->setBlue( clr->getBlue() );
}

/**
*    multiply
*    Converts vertices into screen coordinates.
*    Preconditions:
*        The Matrix that holds windowing coordinates.
*    Postconditions:
*        Returns the vertices in screen coordinates.
*/
Vertex* Vertex::multiply(Matrix* mtrx)
{
    Vertex* vrt = new Vertex( 0, 0, 0 );
    Matrix* tmp = mtrx->multiply( vertex );

    vrt->setX( tmp->getElement(1, 1) );
    vrt->setY( tmp->getElement(2, 1) );
    vrt->setZ( tmp->getElement(3, 1) );
    vrt->setH( tmp->getElement(4, 1) );
    vrt->setRed( this->getRed() );
    vrt->setGreen( this->getGreen() );
    vrt->setBlue( this->getBlue() );

    delete tmp;
    return vrt;
}

/**
*    subtract
*    Performs point-point subtract on two Vectors.
*    Preconditions:
*        The Vector the perform the subtraction on.
*    Postconditions:
*        Returns a vector resulted from the point-point subtraction.
*/
Vector* Vertex::subtract( Vertex* v1 )
{
    double x = this->getX() - v1->getX();
    double y = this->getY() - v1->getY();
    double z = this->getZ() - v1->getZ();

    Vector* v = new Vector( x,y,z );

    return v;
}

/**
*    homogenize - destructive method.
*    Performs perspective division.
*    Preconditions:
*        None.
*    Postconditions:
*        Sets x, y, and z to (orig_values/h).
*/
void Vertex::homogenize()
{
    this->setX(this->getX()/this->getH());
    this->setY(this->getY()/this->getH());
    this->setZ(this->getZ()/this->getH());
}
