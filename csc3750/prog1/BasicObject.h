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

class BasicObject
{
        private:
                List<Face>* faces;
                List<Vertex>* vertices;

        public:
                BasicObject();
                ~BasicObject();
                void addFace( Face* face );
		void addVertex( Vertex* vrt );
                void render( Matrix* wnd, Pixel* pix);
};

#endif
