/**
*    SphericalTextureCoord
*    Thomas Shaffer
*    23 Nov. 2009
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "List.h"
#include "FaceMin.h"
#include "VertexMin.h"
#include "FileIO.h"

#define     PI        3.14
#define     BUFFER    255

List<FaceMin>* faces;
List<VertexMin>* vertices;

void getST( VertexMin* vert, double* st )
{
    double x = vert->getX();
    double y = vert->getY();
    double z = vert->getZ();
	
    double theta;
    double phi;
    double s;
    double t;
	
    double r = x*x + y*y + z*z;
    double sqp = sqrt(r);

    theta = acos(y/sqp);
    phi = atan(x/z);

    if ( z <= 0 ) {
        s = (phi/(2.0*PI)) + 0.5;
    } else if ( x <= 0 ) {
        s = (phi/(2.0*PI)) + 1.0;
    } else {
        if ( z > 0 ) {
            s = (phi/(2.0*PI)) + 1.0;
        } else {
            s = (phi/(2.0*PI));
        }
    }

    t = 1-(theta/PI);

    st[0] = s;
    st[1] = t;
}

int main( int argv, char* argc[] )
{
    faces = new List<FaceMin>();
    vertices = new List<VertexMin>();

    FileIO* file = new FileIO(argc[1], ' '); //assumed to be a read if a delimiter is specified

    double x, y, z;
    string* token;
    int index1, index2, index3, temp;

    //parsing the complex index information for the triangular face
    //could include a vertex index, a vertex normal index, and texture coord information
    string* str1;
    string* str2;
    string* str3;
    string str11;
    string str22;
    string str33;

    string* v = new string("v");
    string* f = new string("f");
   
    while (!file->eof())
    {
        ListIterator<string>* iter = file->getTokens();
        token = iter->next();

        //all vertices are held in the vertices list so that the faces can be created
        if (*token == *v)  //vertices
        {
            delete token;

            token = iter->next();
            x = atof((*token).c_str());
            delete token;

            token = iter->next();
            y = atof((*token).c_str());
            delete token;

            token = iter->next();
            z = atof((*token).c_str());
            delete token;

            VertexMin* vertex = new VertexMin(x, y, z);
            vertices->add(vertex);
        } else if (*token == *f) {
            delete token;

            //get the complex index information for the triangle face
            str1 = iter->next();
            str2 = iter->next();
            str3 = iter->next();

            //parse the index information for the vertex index
            temp = str1->find("/");
            str11 = str1->erase(temp, str1->size() - 1);

            temp = str2->find("/");
            str22 = str2->erase(temp, str2->size() - 1);

            temp = str3->find("/");
            str33 = str3->erase(temp, str3->size() - 1);
            char* cp;
            int len;

            cp = new char[10];
            len = str11.length();
            str11.copy(cp, len, 0);
            cp[len] = '\0';
            index1 = atoi(cp);
            delete[] cp;

            cp = new char[10];
            len = str22.length();
            str22.copy(cp, len, 0);
            cp[len] = '\0';
            index2 = atoi(cp);
            delete[] cp;

            cp = new char[10];
            len = str33.length();
            str33.copy(cp, len, 0);
            cp[len] = '\0';
            index3 = atoi(cp);
            delete[] cp;

            VertexMin* vertex1 = vertices->get(index1);
            VertexMin* vertex2 = vertices->get(index2);
            VertexMin* vertex3 = vertices->get(index3);

            FaceMin *face = new FaceMin();
            face->addVertex(vertex1);
            face->addVertex(vertex2);
            face->addVertex(vertex3);

            faces->add(face);
            delete str1;
            delete str2;
            delete str3;
        }
        //still need to delete the tokens even if the line is completely disregarded
        else {
            delete token;
            while(iter->hasNext())
            {
                token = iter->next();
                delete token;
            }
        }
 
        delete iter;
    } 

    delete v;
    delete f;
    delete file;

    char buf[BUFFER];
    FILE* pFile;
    pFile = fopen( "spcoord.txt", "a+" );

    ListIterator<FaceMin>* i = faces->iterator();
    while( i->hasNext() ) {
        FaceMin* f = i->next();
        VertexMin* a = f->getVertex(1);
        VertexMin* c = f->getVertex(3);
        VertexMin* b = f->getVertex(2);

        double st1[2];
        double st2[2];
        double st3[2];
        getST( a, st1 );
        getST( b, st2 );
        getST( c, st3 );

        snprintf( buf, BUFFER, "vt %f %f %f %f %f %f\n", st1[0], st1[1], st2[0], st2[1], st3[0], st3[1] );
        fputs( buf, pFile );
    }
    fclose( pFile );
    return 0;
}
