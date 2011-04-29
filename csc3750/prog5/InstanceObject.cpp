/**
*    InstanceObject
*    Thomas Shaffer
*    16 September 2009
*
*    Object used to have multiple instances of an object
*    on the screen.
*/
#include "InstanceObject.h"

/**
*    InstanceObject
*    Constructor
*    Preconditions:
*        A basic object.
*    Postconditions:
*        Creates a reference to a BasicObject.
*        Increments deleteObject's reference counter.
*        creates a new instance matrix.
*/
InstanceObject::InstanceObject( BasicObject* obj )
{
    instance = new Matrix(4,4);
    instance->setElement(1,1,1);
    instance->setElement(1,2,0);
    instance->setElement(1,3,0);
    instance->setElement(1,4,0);
    instance->setElement(2,1,0);
    instance->setElement(2,2,1);
    instance->setElement(2,3,0);
    instance->setElement(2,4,0);
    instance->setElement(3,1,0);
    instance->setElement(3,2,0);
    instance->setElement(3,3,1);
    instance->setElement(3,4,0);
    instance->setElement(4,1,0);
    instance->setElement(4,2,0);
    instance->setElement(4,3,0);
    instance->setElement(4,4,1);

    ref = obj;
    ref->addRef();
    material = new Color( 1, 1, 1 );
}

/**
*    ~InstanceObject
*    Deconstructor
*    Preconditions:
*        None.
*    Postconditions:
*        Deletes the instance matrix.
*        Decrements deleteObject's counter.
*/
InstanceObject::~InstanceObject()
{
    ref->removeRef();
    delete instance;
    delete material;
}

void InstanceObject::setColor( double r, double g, double b )
{
    material->setRed( r );
    material->setGreen( g );
    material->setBlue( b );
}

/**
*    render
*    Renders the Instance object.
*    Preconditions:
*        The windowing transform matrix.
*        The pixel object.
*    Postconditions:
*        Renders the Instance Object.
*/
void InstanceObject::render( Pixel* pix, Matrix* wnd, Matrix* zbuffer )
{
    Matrix* tmp = wnd->multiply( instance );
    ref->render( tmp, pix, material, zbuffer );

    delete tmp;
}

/**
*    buildTransform
*        Creates a new matrix for the Instance Object.   
*    Preconditions:
*        A matrix.
*    Postconditions:
*        Creates a new instance matrix.
*/
void InstanceObject::buildTransform( Matrix* mtrx )
{
    Matrix* tmp =  mtrx->multiply( instance );
    delete instance;
    instance = tmp;
}
