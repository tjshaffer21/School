/**
*    Face
*    Thomas Shaffer
*    16 September 2009
*    Face class takes a select number of vertices (three) and connects them 
*    using either DDA algorithm or Bresenham's algorithm.
*/
#if !defined (FACE_H)
#define FACE_H

#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include "Pixel.h"
#include "Vertex.h"
#include "List.h"
#include "Light.h"

#define IA 0.1

class Vertex;
class Face
{
    private:
        List<Vertex>* vertices;
        int zbuffering( int x, int y, double z, Matrix* zbuffer );
        int backface( Vertex* p, Vertex* q, Vertex* r );
        Color* obtainShading( Vertex* m, Vector* n, Light* light, Color* amb,
            Vertex* eye, double attenuation, double shininess );
        void renderBarycentric( Pixel* pix, Vertex* p, Vertex* r, Vertex* q,
            Matrix* zbuffer );


    public:
        Face();
        virtual ~Face();
        void addVertex( Vertex* vertex );
        List<Vertex>* getVertices();
        Vertex* getVertex( int i );
        void render( Pixel* pix, Matrix* wnd, Matrix* trs, Matrix* zbuffer,
            Light* light, Vertex* eye, Color* material, Color* amb,
            double attenuation, double shininess );
        Vector* getNormal( Vertex* p, Vertex* q, Vertex* r );
        Vector* avgNormal( Vertex* m);
};

#endif
