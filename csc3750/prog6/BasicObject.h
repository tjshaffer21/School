/**
*    BasicObject
*    Thomas Shaffer
*    16 September 2009
*    Basic object class handles the creation of basic objects (triangles) for
*    the purpose of creating more complex objects.
*/
#if !defined (BASICOBJECT_H)
#define BASICOBJECT_H

#include "Pixel.h"
#include "DeleteObject.h"
#include "List.h"
#include "Matrix.h"
#include "Face.h"

class Light;
class Color;
class Vertex;
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
        void render( Pixel* pix, Matrix* wnd, Matrix* trs, Matrix* zbuffer,
            Light* light, Vertex* eye, Color* material, Color* amb,
            double attenuation, double shininess );
};

#endif
