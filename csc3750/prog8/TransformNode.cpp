/**
*    Thomas Shaffer
*    Transform Node
*    28 Septemeber 2009
*    A Transform Node holds a base matrix that is
*    multiplied on all children nodes. The node is
*    used to create more complex objects.
*/
#include "TransformNode.h"

TransformNode::TransformNode( Matrix* mtrx ) : Node()
{
    transform = mtrx;
    cld = new List<Node>();
}

TransformNode::~TransformNode()
{
    ListIterator<Node>* iter = cld->iterator();
    while ( iter->hasNext() ) {
        iter->next()->removeRef();
    }

    delete iter;
    delete cld;
    delete transform;
}

void TransformNode::addNode( Node* child )
{
    cld->add( child );
    child->addRef();
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
void TransformNode::buildTransform( Matrix* mtrx )
{
    Matrix* tmp =  mtrx->multiply( transform );
    delete transform;
    delete mtrx;
    transform = tmp;
}

/**
*    render
*    Renders the TransformNode object.
*    Preconditions:
*        A Matrix pointer to the matrix that is to be multiplied with the
*        transform matrix.
*    Postconditions:
*        Passes the matrix down the pipeline.
*/
void TransformNode::render( Matrix* mtrx )
{
    Matrix* temp = mtrx->multiply( transform );

    ListIterator<Node>* iter = cld->iterator();
    while( iter->hasNext() ) {
        iter->next()->render( temp );
    }

    delete iter;
    delete temp;
}
