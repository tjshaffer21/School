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

class InstanceObject
{
        private:
                Matrix* instance;       // TRS
                BasicObject* ref;

        public:
                InstanceObject( BasicObject* obj );
                virtual ~InstanceObject();
                void render( Matrix* wnd, Pixel* pix );
                void buildTransform( Matrix* mtrx );
};

#endif
