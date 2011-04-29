#include "Picture.h"

void Picture::render(Pixel* pix)
{
//while(true){
    // Get BasicObjects
    BasicObject* obj = readObject( OBJ_FILE );

    double z[3];
    getZValues( FOV_FILE, z );

    //WNACI
    Matrix* wnd = AffineTransforms::windowingTransform(pix->getWidth(),
        pix->getHeight());
    Matrix* aspct = AffineTransforms::aspectRatio(pix->getWidth(),
        pix->getHeight(), z[0]);    // Matrix for aspect ratio.

    Vertex* eye = new Vertex( 0,0,0 );
    Matrix* camera = buildCamera( CMRA_FILE, eye );
    Matrix* normal = AffineTransforms::perspective( z[1], z[2] );

    Matrix* tmp = aspct->multiply(camera);
    delete camera;
    delete aspct;

    Matrix* tmp2 = normal->multiply( tmp );
    delete tmp;
    delete normal;

    Matrix* wnca = wnd->multiply(tmp2);
    delete tmp2;
    delete wnd;
    // END WNACI

    // Scene creation
    Scene* scene = new Scene(wnca);

    // Set up lighting
    double shading[8];
    getShadingInfo( SHADE_FILE, shading );

    Light* light = new Light();
    Vertex* light_loc = new Vertex( shading[0], shading[1], shading[2] );
    Color* light_clr = new Color( LGHT_CLR );
    
    light->setLocation( light_loc );
    light->setColor( light_clr );
    
    Color* amb = new Color( AMB_CLR );
    
    // Add objects to the scene.
    InstanceObject* sp = buildInstanceObject( TRS_FILE, obj );
    InstanceObject* sp2 = buildInstanceObject( TRS_FILE2, obj );
    sp->setShininess( shading[7] );
    sp2->setShininess( shading[7] );

    scene->addNode( sp );
    scene->addNode( sp2 );

    scene->render( pix, light, eye, amb, shading[6] );

    delete eye;
    delete amb;
    delete light;
    delete scene;
//}
}

/**
*    Reads a file and creates a BasicObject object.
*    Written by Dr. Boshart
*/
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

/**
*    Reads a file to build an InstanceObject object.
*    Written by Dr. Boshart
*/
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

    line = inputFile->readLine();
    line = inputFile->readLine();
    double red = atof(line.c_str());
    line = inputFile->readLine();
    double green = atof(line.c_str());
    line = inputFile->readLine();
    double blue = atof(line.c_str());
    io->setColor( red, green, blue );

    delete inputFile;
    return io;
}

/**
*    buildCamera
*    Obtain user information needed to create the camera.
*    Preconditions:
*        The filename to be read from.
*        The vertex for the eye point.
*    Postconditions:
*        Returns a Matrix pointer that holds the camera's info.
*        Modifies the eye point vertex to the eye point.
*/
Matrix* Picture::buildCamera(char* fileName, Vertex* eye)
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
    eye->setX( ex );
    eye->setY( ey );
    eye->setZ( ez );

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

    delete at;
    delete up;
    delete inputFile;

    return camera;
}

/**
*    getZValues
*    Obtain the values for fov and zmax/zmin.
*    Preconditions:
*        The filename to read from.
*        The array to store the information.
*    Postconditions:
*        The array is filled:
*            0 - fov
*            1 - zmax
*            2 - zmin
*/
void Picture::getZValues( char* fileName, double zs[] )
{
   FileIO* inputFile = new FileIO(fileName, 1);  //for reading

   //eye vertex
   string line = inputFile->readLine();  //skip this line
   line = inputFile->readLine();
   zs[0] = atof(line.c_str());
   line = inputFile->readLine();
   line = inputFile->readLine();
   zs[1] = atof(line.c_str());
   line = inputFile->readLine();
   line = inputFile->readLine();
   zs[2] = atof(line.c_str());
  
   delete inputFile;
}

/**
*    getShadingInfo
*    Obtains the user information necessary to perform shading on the
*    scene.
*    Preconditions:
*        The file to read from.
*        The array to store the information.
*    Postconditions:
*        The array is filled:
*           0-2 - Light position.
*           3-5 - Ambient Light Color
*           6   - Linear attenuation
*           7   - Shininess
*/
void Picture::getShadingInfo( char* fileName, double s_array[] )
{
   FileIO* inputFile = new FileIO(fileName, 1);  //for reading

   //eye vertex
   string line = inputFile->readLine();  //skip this line
   line = inputFile->readLine();
   s_array[0] = atof(line.c_str());
   line = inputFile->readLine();
   s_array[1] = atof(line.c_str());
   line = inputFile->readLine();
   s_array[2] = atof(line.c_str());
   line = inputFile->readLine();

   line = inputFile->readLine();
   s_array[3] = atof(line.c_str());
   line = inputFile->readLine();
   s_array[4] = atof(line.c_str());
   line = inputFile->readLine();
   s_array[5] = atof(line.c_str());

   line = inputFile->readLine();
   line = inputFile->readLine();
   s_array[6] = atof(line.c_str());

   line = inputFile->readLine();
   line = inputFile->readLine();
   s_array[7] = atof( line.c_str() );

   delete inputFile;
}
