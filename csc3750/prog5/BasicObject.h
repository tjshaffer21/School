/**
*    BasicObject
*    Thomas Shaffer
*    16 September 2009
*    Basic object class handles the creation of basic objects (triangles) for
*    the purpose of creating more complex objects.
*/
#if !defined (BASICOBJECT_H)
#define BASICOBJECT_H

#include "Face.h"
#include "DeleteObject.h"

class BasicObject : public DeleteObject
{
    private:
        List<Face>* faces;
        List<Vertex>* vertices;

    public:
        BasicObject();
        virtual ~BasicObject();
        void addFace( Face* face );
        void addVertex( Vertex* vrt );
        void render( Matrix* wnd, Pixel* pix, Color* material, Matrix* zbuffer );
};

#endif
