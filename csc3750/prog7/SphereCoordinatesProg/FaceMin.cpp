/**
*    Face
*    Thomas Shaffer
*    28 October 2009
*    Face class takes a select number of vertices and renders them.
*/
#include "FaceMin.h"

FaceMin::FaceMin() { vertices = new List<VertexMin>(); }
FaceMin::~FaceMin() { delete vertices; }

/**
*    addVertex
*    Adds a vertex into the object's list of vertices.
*    Preconditions:
*        A pointer to the memory address of the vertex.
*    Postconditions:
*        The vertex is added into the list of vertices.
*/
void FaceMin::addVertex( VertexMin* vertex ) { vertices->add( vertex ); }

/**
*    getVertices
*    Returns a list of vertices.
*    Preconditions:
*        None.
*    Postconditions:
*        Returns a list of vertices.
*/
List<VertexMin>* FaceMin::getVertices() { return vertices; }

/**
*    getVertex
*    Returns a pointer to the specified vertex.
*    Preconditions:
*        The integer value of the vertex to return.
*    Postcondtion:
*        Returns a pointer to the vertex specified.
*/
VertexMin* FaceMin::getVertex( int i ) { return vertices->get(i); }
