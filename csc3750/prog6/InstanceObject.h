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
        Color* material;
        double shininess;

    public:
        InstanceObject( BasicObject* obj );
        virtual ~InstanceObject();
        void setColor( double r, double b, double g );
        void render( Pixel* pix, Matrix* wnd, Matrix* zbuffer,
            Light* light, Vertex* eye, Color* amb, double attenuation );
        void buildTransform( Matrix* mtrx );
            Matrix* getInstance();
        void setShininess( double shininess );
        double getShininess();
};

#endif
