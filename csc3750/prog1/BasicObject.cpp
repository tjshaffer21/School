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
        // Delete Face objects.
        ListIterator<Face>* iter = faces->iterator(); 
        while ( iter->hasNext() ) {
                delete iter->next();
        }
        delete iter;
    
        // Delete Vertex objects
        ListIterator<Vertex>* iter1 = vertices->iterator();
        while( iter1->hasNext() ) {
                delete iter1->next();
        }
        delete iter1;
    
        delete faces;
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
void BasicObject::addFace( Face* face )
{
        faces->add( face );
    
        // Add Vertex objects into vertices list.
        ListIterator<Vertex>* iter = face->getVertices()->iterator();
        while ( iter->hasNext() ) {
            vertices->add( iter->next() );
        }
    
        delete iter;
}

/**
*    addVertex
*    Adds a new vertex to the object.
*    Preconditions:
*        The vertex to add to the object.
*    Postconditions:
*        The vertex is added to the vertices list.
*/
void BasicObject::addVertex( Vertex* vrt )
{
        vertices->add( vrt );
}

/**
*    render
*    Renders the basic object.
*    Preconditions:
*        The windowing transform matrix, the pixel object.
*    Postconditions:
*        Calls the next render method within the hierarchy.
*/
void BasicObject::render( Matrix* wnd, Pixel* pix )
{
        ListIterator<Face>* iter = faces->iterator();
        while( iter->hasNext() ) {
                iter->next()->render( wnd, pix );	
        }

        delete iter;
}
