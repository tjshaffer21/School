/**
*    Face
*    Thomas Shaffer
*    16 September 2009
*    Face class takes a select number of vertices (three) and connects them 
*    using either DDA algorithm or Bresenham's algorithm.
*/
#if !defined (FACE_H)
#define FACE_H

#include "Pixel.h"
#include "Vertex.h"
#include "List.h"
#include <algorithm>
#include <math.h>
#include <stdlib.h>

class Face
{
private:
    List<Vertex>* vertices;
    void renderBarycentric( Pixel* pix, Vertex* p, Vertex* r, Vertex* q,
        Matrix* zbuffer );
    int zbuffering( int x, int y, double z, Matrix* zbuffer );
    int backface( Vertex* p, Vertex* q, Vertex* r );
    
public:
    Face();
    virtual ~Face();
    void addVertex( Vertex* vertex );
    void render( Matrix* mtrx, Pixel *pix, Color* material, Matrix* zbuffer );

    List<Vertex>* getVertices();
};

#endif
