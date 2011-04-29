#if !defined (TRANSFORMNODE_H)
#define TRANSFORMNODE_H

#include "Matrix.h"
#include "List.h"
#include "Node.h"

class TransformNode : public Node
{
    private:
        Matrix *trans;
        List<Node>*  cld;

    public:
        TransformNode( Matrix* trans );
        virtual ~TransformNode();
        void addNode( Node* child );
        void render( Pixel* pix, Matrix* trans );
};
#endif
