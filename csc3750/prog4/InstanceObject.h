/**
*    InstanceObject
*    Thomas Shaffer
*    16 September 2009
*    
*    Object allows for multiple instances of an
*    object on the screen.
*/
#if !defined (INSTANCEOBJECT_H)
#define INSTANCEOBJECT_H

#include "Matrix.h"
#include "BasicObject.h"
#include "Node.h"

class InstanceObject : public Node
{
        private:
                Matrix* instance;       // TRS
                BasicObject* ref;

        public:
                InstanceObject( BasicObject* obj );
                virtual ~InstanceObject();
                void render( Pixel* pix, Matrix* wnd );
                void buildTransform( Matrix* mtrx );
                Matrix* getInstance();
};

#endif
