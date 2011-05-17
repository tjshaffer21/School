#if !defined (FACEMIN_H)
#define FACEMIN_H

#include <stdlib.h>
#include "VertexMin.h"
#include "List.h"

class FaceMin
{
    private:
        List<VertexMin>* vertices;

    public:
        FaceMin();
        virtual ~FaceMin();
        void addVertex( VertexMin* vertex );
        List<VertexMin>* getVertices();
        VertexMin* getVertex( int i );
};
#endif
