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

#include <fstream>
#include "Matrix.h"
#include "BasicObject.h"
#include "Node.h"
#include "Texture.h"

class InstanceObject : public Node
{
    private:
        Matrix* instance;       // TRS
        BasicObject* ref;
        Color* material;
        Texture* texture;
        double shininess;

    public:
        InstanceObject( BasicObject* obj );
        virtual ~InstanceObject();
        void setColor( double r, double b, double g );
        void setShininess( double shininess );
        double getShininess();
        void setTexture( Texture* tex );
        void buildTransform( Matrix* mtrx );
            Matrix* getInstance();
        unsigned char* readTexture( Texture* tex );
        void render( Pixel* pix, Matrix* wnd, Matrix* zbuffer,
            Light* light, Vertex* eye, Color* amb, double attenuation );
};

#endif
