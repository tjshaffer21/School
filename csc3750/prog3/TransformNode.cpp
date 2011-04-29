/**
*    Thomas Shaffer
*    Transform Node
*    28 Septemeber 2009
*    A Transform Node holds a base matrix that is
*    multiplied on all children nodes. The node is
*    used to create more complex objects.
*/
#include "TransformNode.h"

/**
*    Constructor
*    Creates a Transform Node so the render can create
*    complex objects.
*    Preconditions:
*        The base matrix to be carried out on all child
*        Nodes.
*    Postconditions:
*        Instantiates the list of children.
*        Sets the instance matrix to be the parameter.
*/
TransformNode::TransformNode( Matrix* mtrx ) : Node()
{
    trans = mtrx;
    cld = new List<Node>();
}

/**
*    ~TransformNode
*    Deconstructor
*    Preconditions:
*        None.
*    Postconditions:
*        Removes references from children.
*        deletes the instance matrix.
*/
TransformNode::~TransformNode()
{
    ListIterator<Node>* iter = cld->iterator();
    while ( iter->hasNext() ) {
        iter->next()->removeRef();
    }
    delete iter;

    delete cld;
    delete trans;
}

/**
*    addNode
*    Adds a node to the list of children.
*    Preconditions:
*        A Node object.
*    Postconditions:
*        The node is added to the list.
*/
void TransformNode::addNode( Node* child )
{
    child->addRef();
    cld->add( child );
}

void TransformNode::render( Pixel* pix, Matrix* mtrx )
{
    Matrix* temp = mtrx->multiply( trans );

    ListIterator<Node>* iter = cld->iterator();
    while( iter->hasNext() ) {
        iter->next()->render( pix, temp );
    }
    delete iter;
    delete temp;
}
