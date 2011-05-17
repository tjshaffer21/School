/**
*    InstanceObject
*    Thomas Shaffer
*    27 Nov. 2009
*
*    An object that contains the nonshared values of a BasicObject. It is used
*    to create multiple BasicObjects.
*/
#include "InstanceObject.h"
#include <iostream>
using namespace std;
/**
*    InstanceObject
*    Constructor
*    Preconditions:
*        A basic object.
*    Postconditions:
*        Creates a reference to a BasicObject.
*        Increments deleteObject's reference counter.
*/
InstanceObject::InstanceObject( BasicObject* obj ) : Node()
{
    // Add a basic object to the Instance Object.
    ref = obj;
    ref->addRef();

    material = new Color( 0.3, 0.3, 1.0 );
    shininess = 20;
    texture = NULL;
    normal = NULL;
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
    delete material;
}

/**
*    setColor
*    Sets the material color of the InstanceObject.
*    Preconditions:
*        Color pointer to the material color.
*    Postconditions:
*        The InstanceObject's old color is deleted and the new color is set.
*/
void InstanceObject::setColor( Color* diffuse )
{
    Color* temp = material;
    material = diffuse;
    delete temp;

    if ( texture != NULL ) { delete texture; }
    if ( normal != NULL ) { delete normal; }
}

/**
*    setTexture
*    Sets the texture of the InstanceObject.
*    Preconditions:
*        Texture pointer to the new texture.
*    Postconditions:
*       The old texture is deleted and the new texture is set.
*/
void InstanceObject::setTexture( Texture* tex )
{
    if ( texture == NULL )
    {
        texture = tex;
    } else {
        delete texture;
        texture = tex;
    }
}

/**
*    setNormal
*    Sets the Normal of the InstanceObject.
*    Preconditions:
*        Texture pointer to the new normal.
*    Postconditions:
*       The old normal is deleted and the new normal is set.
*/
void InstanceObject::setNormal( Texture* tex )
{
    if ( normal == NULL )
    {
        normal = tex;
    } else {
        delete normal;
        normal = tex;
    }
}

void InstanceObject::setShininess( float s ) { shininess = s; }

/**
*    readTexture
*    Reads the texture information.
*    Preconditions:
*        A Texture pointer to the texture.
*    Postconditions:
*        Returns a char pointer holding the texture data.
*/
unsigned char* InstanceObject::readTexture(Texture* tex)
{
   int width = tex->getWidth();
   int height = tex->getHeight();
   int numColors = width*height*3;
   char* textureFile = tex->getFileName();

   unsigned char* textureColors = new unsigned char[numColors];
   ifstream infile(textureFile, ios::binary);
   infile.read((char*)textureColors, numColors);
   infile.close();

   return textureColors;
}

/**
*    buildTransform
*    Written by Thomas Shaffer
*    Creates a new matrix for the Instance Object.
*    Preconditions:
*        A matrix.
*    Postconditions:
*        Creates a new instance matrix.
*/
void InstanceObject::buildTransform( Matrix* mtrx )
{
    Matrix* tmp =  mtrx->multiply( transform );
    delete transform;
    delete mtrx;
    transform = tmp;
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
void InstanceObject::render( Matrix* mtrx )
{
    Matrix* up_mtrx = mtrx->multiply( transform );
    GLfloat* diffuse = new GLfloat[4];

    if ( texture != NULL ) {
        unsigned char* textureColors = readTexture( texture );
        unsigned char* bumpColor = readTexture( normal );

        int w = texture->getWidth();
        int h = texture->getHeight();

        // Prepare GPU memory
        GLuint* tex_num = new GLuint[2];
        glGenTextures(2, tex_num );

        glEnable( GL_TEXTURE_2D );
        glActiveTexture( GL_TEXTURE0 );
        // create texture object.
        glBindTexture( GL_TEXTURE_2D, tex_num[0] );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
            GL_UNSIGNED_BYTE, textureColors );

        if ( normal != NULL ) {
            glActiveTexture( GL_TEXTURE1 );
            glBindTexture( GL_TEXTURE_2D, tex_num[1] );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
                GL_UNSIGNED_BYTE, bumpColor );
        }

        diffuse[0] = 1.0;
        diffuse[1] = 1.0;
        diffuse[2] = 1.0;
        diffuse[3] = 1.0;
    } else {
        glDisable( GL_TEXTURE_2D );
        diffuse[0] = material->getRed();
        diffuse[1] = material->getGreen();
        diffuse[2] = material->getBlue();
        diffuse[3] = 1.0;
    }

    // save state
    glPushMatrix();
    glPushAttrib( GL_CURRENT_BIT );

    const float* up_mtrx_array = up_mtrx->toArray();
    glMultMatrixf( up_mtrx_array );

    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse );
    glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, shininess );

    ref->render();

    // obtain state
    glPopAttrib();
    glPopMatrix();
    
    delete[] up_mtrx_array;
    delete up_mtrx;
    delete[] diffuse;
}


