/**
*    BasicObject
*    Thomas Shaffer
*    16 September 2009
*    Basic Object class handles the creation of basic objects (triangles) for
*    the purpose of creating more complex objects.
*/
#include "BasicObject.h"

/**
*   BasicObject
*   Constructor.
*   Preconditions:
*       A char pointer to the file that contains of the object information.
*   Postconditions:
*       Instantiates BasicObject's variables.
*/
BasicObject::BasicObject( char* file )
{
    num_vertices = numVertices( file );
    num_faces = numFaces( file );
    index = num_faces*3;

    vertices = getVertices( file, num_vertices );
    normals = getNormals( file, num_vertices );
    faces = getFaces( file, num_faces );
    tex_coords = getTextureCoords( file, num_vertices );
}

/**
*   ~BasicObject
*   Deconstuctor.
*   Preconditions:
*       None.
*   Postconditions:
*       BasicObject object is destroyed.
*/
BasicObject::~BasicObject()
{
    delete[] vertices;
    delete[] normals;
    delete[] faces;
    delete[] tex_coords;
}

/**
*    render
*    Renders the basic object.
*    Preconditions:
*        None.
*    Postconditions:
*        Renders the object.
*/
void BasicObject::render()
{
    // Enable capabilities.
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );
    glEnableClientState( GL_TEXTURE2 );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    // Set up information locations
    glVertexPointer( 3, GL_FLOAT, 0, vertices );
    glNormalPointer( GL_FLOAT, 0, normals );
    glTexCoordPointer( 2, GL_FLOAT, 0, tex_coords );

    // Render
    glDrawElements( GL_TRIANGLES, index, GL_UNSIGNED_SHORT, faces );
}
