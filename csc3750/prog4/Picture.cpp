#include "Picture.h"

void Picture::render(Pixel* pix)
{
    // Get BasicObjects
    BasicObject* sphere = readObject( "sphere.txt" );

    //WNACI
    Matrix* wnd_trns = AffineTransforms::windowingTransform(pix->getWidth(),
        pix->getHeight());

    double z[3];
    getZValues( "fov.txt", z );

    // Aspect Ratio
    Matrix* aspct_ratio = AffineTransforms::aspectRatio(pix->getWidth(),
        pix->getHeight(), z[0]);    // Matrix for aspect ratio.

    // Normalization and Camera
    Matrix* camera = buildCamera( "camera.txt" );
    Matrix* normal = AffineTransforms::perspective( z[1], z[2] );

    Matrix* wnca = wnd_trns->multiply(normal)->multiply(aspct_ratio)->multiply(camera);

    delete wnd_trns;
    delete camera;
    delete normal;
    delete aspct_ratio;

    Scene* scene = new Scene(wnca);

    InstanceObject* sp = buildInstanceObject( "trs.txt", sphere );

    // Scene creation
    scene->addNode( sp );

//while(1){
     scene->render( pix );
//}

     delete scene;
}

BasicObject* Picture::readObject(char* fileName)
{
   BasicObject* obj = new BasicObject();
   FileIO* file = new FileIO(fileName, ' '); //assumed to be a read if a delimiter is specified

   List<Vertex> vertices;
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

         Vertex* vertex = new Vertex(x, y, z);
         vertices.add(vertex);
         obj->addVertex(vertex);
      }

      else if (*token == *f)  //face definition
      {
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

         Face* face = new Face();

         Vertex* vertex1 = vertices.get(index1);
         Vertex* vertex2 = vertices.get(index2);
         Vertex* vertex3 = vertices.get(index3);

         face->addVertex(vertex1);
         face->addVertex(vertex2);
         face->addVertex(vertex3);

         obj->addFace(face);

         delete str1;
         delete str2;
         delete str3;
      }

      //still need to delete the tokens even if the line is completely disregarded
      else
      {
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
   return obj;
}

InstanceObject* Picture::buildInstanceObject(char* fileName, BasicObject* obj)
{
    InstanceObject* io = new InstanceObject(obj);
    FileIO* inputFile = new FileIO(fileName, 1);  //for reading

    //scale transformation
    string line = inputFile->readLine();  //skip this line
    line = inputFile->readLine();
    double sx = atof(line.c_str());
    line = inputFile->readLine();
    double sy = atof(line.c_str());
    line = inputFile->readLine();
    double sz = atof(line.c_str());
    Matrix* scale = AffineTransforms::scale(sx, sy, sz);

    //rotation transformations
    line = inputFile->readLine();  //skip this line
    line = inputFile->readLine();
    double rx = atof(line.c_str());
    Matrix* rotX = AffineTransforms::rotateX(rx);
    line = inputFile->readLine();
    double ry = atof(line.c_str());
    Matrix* rotY = AffineTransforms::rotateY(ry);
    line = inputFile->readLine();
    double rz = atof(line.c_str());
    Matrix* rotZ = AffineTransforms::rotateZ(rz);

    //translation transformation
    line = inputFile->readLine();  //skip this line
    line = inputFile->readLine();
    double tx = atof(line.c_str());
    line = inputFile->readLine();
    double ty = atof(line.c_str());
    line = inputFile->readLine();
    double tz = atof(line.c_str());
    Matrix* translate = AffineTransforms::translate(tx, ty, tz);

    //standard TRS form
    io->buildTransform(scale);  //deletes the matrix when done
    io->buildTransform(rotX);
    io->buildTransform(rotY);
    io->buildTransform(rotZ);
    io->buildTransform(translate);

    delete scale;
    delete rotX;
    delete rotY;
    delete rotZ;
    delete translate;

    delete inputFile;
    return io;
}

Matrix* Picture::buildCamera(char* fileName)
{
    FileIO* inputFile = new FileIO(fileName, 1);  //for reading

    //eye vertex
    string line = inputFile->readLine();  //skip this line
    line = inputFile->readLine();
    double ex = atof(line.c_str());
    line = inputFile->readLine();
    double ey = atof(line.c_str());
    line = inputFile->readLine();
    double ez = atof(line.c_str());
    Vertex* eye = new Vertex(ex, ey, ez);

    //at vertex
    line = inputFile->readLine();  //skip this line
    line = inputFile->readLine();
    double ax = atof(line.c_str());
    line = inputFile->readLine();
    double ay = atof(line.c_str());
    line = inputFile->readLine();
    double az = atof(line.c_str());
    Vertex* at = new Vertex(ax, ay, az);

    //up vector
    line = inputFile->readLine();  //skip this line
    line = inputFile->readLine();
    double ux = atof(line.c_str());
    line = inputFile->readLine();
    double uy = atof(line.c_str());
    line = inputFile->readLine();
    double uz = atof(line.c_str());
    Vector* up = new Vector(ux, uy, uz);

    Matrix* camera = AffineTransforms::buildCamera( eye, at, up );

    delete eye;
    delete at;
    delete up;
    delete inputFile;
   
    return camera;
}

void Picture::getZValues( char* fileName, double zs[] )
{
   FileIO* inputFile = new FileIO(fileName, 1);  //for reading

   //eye vertex
   string line = inputFile->readLine();  //skip this line
   line = inputFile->readLine();
   double fov = atof(line.c_str());
   line = inputFile->readLine();
   line = inputFile->readLine();
   double zmax = atof(line.c_str());
   line = inputFile->readLine();
   line = inputFile->readLine();
   double zmin = atof(line.c_str());

   zs[0] = fov;
   zs[1] = zmax;
   zs[2] = zmin;

  delete inputFile;
}
