/**
*    BasicObject
*    Thomas Shaffer
*    27 Nov. 2009
*    Takes information about a basic object's smaller components and assembles
*    and renders them using OpenGL.
*/
#if !defined (BASICOBJECT_H)
#define BASICOBJECT_H

#include <GL/glew.h>
#include <GL/glut.h>
#include "ReadObject.h"
#include "DeleteObject.h"
#include "Color.h"

class Light;
class Color;
class Vertex;
class BasicObject : public DeleteObject
{
    private:
        int num_vertices;
        int num_faces;
        int index;
        unsigned short* faces;
        float* vertices;
        float* normals;
        float* tex_coords;

    public:
        BasicObject( char* file );
        virtual ~BasicObject();
        void render();
};

#endif
