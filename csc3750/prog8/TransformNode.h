#if !defined (TRANSFORMNODE_H)
#define TRANSFORMNODE_H

#include "Matrix.h"
#include "List.h"
#include "Node.h"

class TransformNode : public Node
{
    private:
        List<Node>*  cld;

    public:
        TransformNode( Matrix* trans );
        virtual ~TransformNode();
        void addNode( Node* child );
        void buildTransform( Matrix* mtrx );
        void render( Matrix* mtrx );
};
#endif
