/**
*    InstanceObject
*    Thomas Shaffer
*    27 Nov. 2009
*
*    An object that contains the nonshared values of a BasicObject. It is used
*    to create multiple BasicObjects.
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
        BasicObject* ref;
        Color* material;
        Texture* texture;
        Texture* normal;
        float shininess;

    public:
        InstanceObject( BasicObject* obj );
        InstanceObject( BasicObject* obj, Color* diffuse,
            int shine, Texture* tex );
        virtual ~InstanceObject();
        void setColor( Color* diffuse );
        void setShininess( float shininess );
        void setTexture( Texture* tex );
        void setNormal( Texture* tex );
        unsigned char* readTexture( Texture* tex );
        void buildTransform( Matrix* mtrx );
        void render(Matrix* mtrx );
};

#endif
