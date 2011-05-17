#include "Picture.h"
#include <iostream>
void Picture::render(int w, int h )
{
//while(true){
    // Get BasicObjects
    BasicObject* obj = new BasicObject( OBJ_FILE );

    Vertex* eye = new Vertex( 0,0,0 );
    Vertex* at = new Vertex(0,0,0);
    Matrix* camera = buildCamera( CMRA_FILE, eye, at );

    // Scene creation
    Scene* scene = new Scene();

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
    sp->setShininess( shading[7] );

    scene->addNode( sp );

    scene->render( w, h, light, eye, at, amb );

    delete eye;
    delete at;
    delete amb;
    delete light;
    delete scene;
//}
}

/**
*    Reads a file to build an InstanceObject object.
*    Written by Dr. Boshart
*    Modified by Thomas Shaffer
*/
InstanceObject* Picture::buildInstanceObject(char* fileName, BasicObject* obj)
{
    InstanceObject* io = new InstanceObject(obj);
    FileIO* inputFile = new FileIO(fileName, 1);  //for reading
    char* f1 = (char*)malloc(255*sizeof(char));
    char* f2 = (char*)malloc(255*sizeof(char));

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

    // Material
    line = inputFile->readLine();
    line = inputFile->readLine();
    double red = atof(line.c_str());
    line = inputFile->readLine();
    double green = atof(line.c_str());
    line = inputFile->readLine();
    double blue = atof(line.c_str());
    Color* color = new Color( red, green, blue );
    io->setColor( color );

    // Texture
    string t_string = string("texture");
    line = inputFile->readLine();
    //if ( strcmp( line.c_str(), t_string.c_str() ) ) {
        line = inputFile->readLine();
        const char* file = line.c_str();
        strcpy( f1, file );
        line = inputFile->readLine();
        const char* file2 = line.c_str();
        strcpy( f2, file2 );
        line = inputFile->readLine();
        int width = atoi( line.c_str() );
        line = inputFile->readLine();
        int height = atoi( line.c_str() );
        Texture *tex = new Texture( f1, width, height );
        Texture *norm = new Texture( f2, width, height );
        io->setTexture( tex );
        io->setNormal( norm );
    //}

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
Matrix* Picture::buildCamera(char* fileName, Vertex* eye, Vertex* at)
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
    at->setX( ax );
    at->setY( ay );
    at->setZ( az );

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

    delete up;
    delete inputFile;

    return camera;
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
