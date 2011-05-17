/** Class provided by Dr. Boshart. */
#ifndef READ_OBJECT_H
#define READ_OBJECT_H

#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>

   int numVertices(char* ptr);
   int numFaces(char* ptr);
   float* getVertices(char* ptr, int vcount);
   float* getNormals(char* ptr, int vcount);
   unsigned short* getFaces(char* ptr, int fcount);
   float* getColors(int numVertices, float r, float g, float b);
   float* getTextureCoords(char* ptr, int vcount);
   char* getText(char* file);

#endif
