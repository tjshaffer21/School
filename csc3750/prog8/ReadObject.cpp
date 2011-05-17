/** Class provided by Dr. Boshart. */
#include "ReadObject.h"

using namespace std;

char* getText(char* file)
{
   string str;
   char line[80];
   ifstream infile(file);

   infile.getline(line, 80);
   while (!infile.eof())
   {
      str = str + line + "\r\n";
      infile.getline(line, 80);
   }
   str = str + '\0';

   const char* text = str.c_str();
   int len = str.length();
   char* result = new char[len];
   strcpy(result, text);
   return result;
}

float* getColors(int numVertices, float r, float g, float b)
{
   float* colors = new float[numVertices*3];

   for (int i = 0; i < numVertices*3; i += 3)
   {
      colors[i] = r;
      colors[i + 1] = g;
      colors[i + 2] = b;
   }

   return colors;
}

int numVertices(char* ptr)
{
   int vcount = 0;
   string test;
   string* v = new string("v");
   ifstream infile(ptr);

   while (!infile.eof())
   {
      infile >> test;

      if (test == *v)  //counting number of vertices
      {
         vcount++;
      }
   }

   infile.close();
   delete v;

   return vcount;
}

int numFaces(char* ptr)
{
   int fcount = 0;
   string test;
   string* f = new string("f");
   ifstream infile(ptr);

   while (!infile.eof())
   {
      infile >> test;

      if (test == *f)  //counting number of faces
      {
         fcount++;
      }
   }

   infile.close();
   delete f;
   return fcount;
}

float* getTextureCoords(char* ptr, int vcount)
{
   float* texture = new float[vcount*2];

   float s, t;
   string test;
   string* vt = new string("vt");
   ifstream infile(ptr);
   vcount = 0;

   while (!infile.eof())
   {
      infile >> test;

      if (test == *vt)  //texture coordinates
      {
         infile >> s;
         infile >> t;
         texture[vcount] = s;
         texture[vcount + 1] = t;

         vcount = vcount + 2;
      }
   }

   infile.close();
   delete vt;
   return texture;
}

float* getNormals(char* ptr, int vcount)
{
   float* normals = new float[vcount*3];

   float x, y, z;
   string test;
   string* vn = new string("vn");
   ifstream infile(ptr);
   vcount = 0;

   while (!infile.eof())
   {
      infile >> test;

      if (test == *vn)  //normals
      {
         infile >> x;
         infile >> y;
         infile >> z;

         normals[vcount] = x;
         normals[vcount + 1] = y;
         normals[vcount + 2] = z;

         vcount = vcount + 3;
      }
   }

   infile.close();
   delete vn;
   return normals;
}

float* getVertices(char* ptr, int vcount)
{
   float* vertices = new float[vcount*3];

   float x, y, z;
   string test;
   string* v = new string("v");
   ifstream infile(ptr);
   vcount = 0;

   while (!infile.eof())
   {
      infile >> test;

      if (test == *v)  //vertices
      {
         infile >> x;
         infile >> y;
         infile >> z;

         vertices[vcount] = x;
         vertices[vcount + 1] = y;
         vertices[vcount + 2] = z;

         vcount = vcount + 3;
      }
   }

   infile.close();

   delete v;
   return vertices;
}

unsigned short* getFaces(char* ptr, int fcount)
{
   int val = fcount*3;
   unsigned short* indices = new unsigned short[val];    //assume each face requires 3 vertices to define it

   int index1, index2, index3;
   int temp;
   string str1, str2, str3, test;
   string* f = new string("f");
   ifstream infile(ptr);
   fcount = 0;

   while (!infile.eof())
   {
      infile >> test;

      if (test == *f)  //face definition
      {
         infile >> str1;
         infile >> str2;
         infile >> str3;

         temp = str1.find("/");
         str1 = str1.erase(temp, str1.size() - 1);
         temp = str2.find("/");
         str2 = str2.erase(temp, str2.size() - 1);
         temp = str3.find("/");
         str3 = str3.erase(temp, str3.size() - 1);

         char* cp;
         int len;

         cp = new char[10];
         len = str1.length();
         str1.copy(cp, len, 0);
         cp[len] = '\0';
         index1 = atoi(cp) - 1;  //subtract one off the index as opengl is 0-based
         delete[] cp;

         cp = new char[10];
         len = str2.length();
         str2.copy(cp, len, 0);
         cp[len] = '\0';
         index2 = atoi(cp) - 1;
         delete[] cp;

         cp = new char[10];
         len = str3.length();
         str3.copy(cp, len, 0);
         cp[len] = '\0';
         index3 = atoi(cp) - 1;
         delete[] cp;

         indices[fcount] = index1;
         indices[fcount + 1] = index2;
         indices[fcount + 2] = index3;

         fcount = fcount + 3;
      }
   }

   infile.close();
   delete f;
   return indices;
}
