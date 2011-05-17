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

    // Add a basic object to the Instance Object.
    ref = obj;
    ref->addRef();
    
    material = new Color( 1.0, 1.0, 1.0 );
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
    delete texture;
}

void InstanceObject::setColor( double r, double g, double b )
{
    material->setRed( r );
    material->setGreen( g );
    material->setBlue( b );
}

void InstanceObject::setShininess( double s ) { shininess = s; }
double InstanceObject::getShininess() { return shininess; }
void InstanceObject::setTexture( Texture* tex ) { texture = tex; }

/**
*    buildTransform
*    Creates a new matrix for the Instance Object.
*    Preconditions:
*        A matrix.
*    Postconditions:
*        Creates a new instance matrix.
*/
void InstanceObject::buildTransform( Matrix* mtrx )
{
    Matrix* tmp =  mtrx->multiply( instance );
    delete instance;
    delete mtrx;
    instance = tmp;
	//delete texture;
}

/**
*    readTexture
*
*    Preconditions:
*
*    Postconditions:
*
*/
unsigned char* InstanceObject::readTexture( Texture* tex )
{
    int width = tex->getWidth();
    int height = tex->getHeight();
    int numColors = width*height*3;
    char* textureFile = tex->getFileName();

    unsigned char* textureColors = new unsigned char[numColors];

    std::ifstream infile( textureFile, std::ios::binary );
    infile.read((char*)textureColors, numColors );
    infile.close();

    return textureColors;
}

/*
*    render
*    Renders the Instance object.
*    Preconditions:
*        The windowing transform matrix.
*        The pixel object.
*    Postconditions:
*        Renders the Instance Object.
*/
void InstanceObject::render( Pixel* pix, Matrix* wnd, Matrix* zbuffer,
    Light* light, Vertex* eye, Color* amb, double attenuation )
{
    Matrix* tmp = wnd->multiply( instance );
    unsigned char* tex = readTexture( texture );
    ref->render( pix, wnd, instance,zbuffer, light, eye, material, amb,
        attenuation, shininess, tex, texture->getWidth(), texture->getHeight() );
    delete tmp;
    delete tex;
}


