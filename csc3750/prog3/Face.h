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
#include <math.h>
#include <stdlib.h>
#include <time.h>

class Face
{
private:
    List<Vertex>* vertices;
    void renderDDA(Vertex* v1, Vertex* v2, Pixel* pix);
    void renderBresenham(Vertex* v1, Vertex* v2, Pixel* pix);

public:
    Face();
    virtual ~Face();
    void addVertex( Vertex* vertex );
    void render( Matrix* mtrx, Pixel *pix );
    List<Vertex>* getVertices();
};

#endif
