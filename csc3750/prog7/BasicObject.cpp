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
*       None.
*   Postconditions:
*       Instantiates BasicObject's variables.
*/
BasicObject::BasicObject()
{
    faces = new List<Face>();
    vertices = new List<Vertex>();
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
    ListIterator<Face>* iter = faces->iterator();
    while ( iter->hasNext() ) {
        Face* face = iter->next();
        delete face;
    }
    
    ListIterator<Vertex>* iter1 = vertices->iterator();
    while( iter1->hasNext() ) {
        Vertex* vertex = iter1->next();
        delete vertex;
    }

    delete iter;
    delete faces;
    delete iter1;
    delete vertices;

}

/**
*    addFace
*    Adds a new face to the object.
*    Preconditions:
*        The face to add to the object.
*    Postconditions:
*        A new face is added to the faces list.
*        The vertices from the new face is added to the vertices list.
*/
void BasicObject::addFace( Face* face ) {
     faces->add( face );
    
     Vertex* vrt = face->getVertex(1);
     vrt->addFace( face );
     vrt = face->getVertex(2);
     vrt->addFace( face );
     vrt = face->getVertex(3);
     vrt->addFace( face );
}

/**
*    addVertex
*    Adds a new vertex to the object.
*    Preconditions:
*        The vertex to add to the object.
*    Postconditions:
*        The vertex is added to the vertices list.
*/
void BasicObject::addVertex( Vertex* vrt ) { vertices->add( vrt ); }

Face* BasicObject::getFace( int index ) { return faces->get( index ); }

/**
*    render
*    Renders the basic object.
*    Preconditions:
*        The windowing transform matrix, the pixel object.
*    Postconditions:
*        Calls the next render method within the hierarchy.
*/
void BasicObject::render( Pixel* pix, Matrix* wnd, Matrix* trs, Matrix* zbuffer,
    Light* light, Vertex* eye, Color* material, Color* amb, double attenuation,
    double shininess, unsigned char* tex, int tHeight, int tWidth )
{
    ListIterator<Face>* iter = faces->iterator();
    while( iter->hasNext() ) {
        iter->next()->render( pix, wnd, trs,zbuffer, light, eye, material, amb,
             attenuation, shininess, tex, tHeight, tWidth );
    }
    delete iter;
}
