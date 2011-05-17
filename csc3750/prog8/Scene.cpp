/**
*    Thomas Shaffer
*    Scene
*    27 Nov. 2009
*    Scene object holds the overall scene information.
*/
#include "Scene.h"

Scene::Scene()
{
    scene = new List<Node>();
}

Scene::~Scene()
{
    ListIterator<Node>* iter = scene->iterator();
    while ( iter->hasNext() ) {
        iter->next()->removeRef();
    }

     delete iter;
     delete scene;
}

void Scene::addNode( Node* io )
{
    scene->add( io );
    io->addRef();
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
void Scene::getZValues( char* fileName, double zs[] )
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
*    render
*    Renders the scene.
*    Preconditions:
*        An integer for the width of the window.
*        An integer for the height of the window.
*        A Light pointer to the light source.
*        A Vertex pointer to the eyepoint.
*        A Vertex pointer to the atpoint.
*        A Color pointer to the ambient color.
*        A double array that holds fov, zmax, and zmin.
*    Postconditions:
*        The scene is rendered when the final render method is reached.
*/
void Scene::render( int w, int h, Light* light, Vertex* eyepoint,
    Vertex* atpoint, Color* ambient )
{
    glClear( GL_DEPTH_BUFFER_BIT );
    glClear( GL_COLOR_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );

    glLoadIdentity();
    float ratio = (((double) h)/w);

    double z[3];
    getZValues( FOV_FILE, z );

    Matrix* perspective = AffineTransforms::perspectiveNorm( z[0], ratio, z[1],
        z[2] );
    const float* perspective_array = perspective->toArray();
    glMultMatrixf( perspective_array );
    delete perspective;

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( eyepoint->getX(), eyepoint->getY(), eyepoint->getZ(),
        atpoint->getX(), atpoint->getY(), atpoint->getZ(), 0, 1, 0 );

    Vertex* lightLoc = light->getLocation();
    GLfloat position0[] = {lightLoc->getX(), lightLoc->getY(), lightLoc->getZ(),
        1.0 };
    GLfloat specular0[] = {1.0,1.0,1.0,1.0};

    GLfloat ambientLight[] = {ambient->getRed(), ambient->getGreen(),
        ambient->getBlue(), 1.0 };
    GLfloat ambientMat[] = {1.0, 0, 0, 1.0};

    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambientLight );
    glLightfv( GL_LIGHT0, GL_POSITION, position0 );
    glLightfv( GL_LIGHT0, GL_SPECULAR, specular0 );

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, ambientMat );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specular0 );

    Matrix* identity = Matrix::getIdentityMatrix(4);

    ListIterator<Node>* iter = scene->iterator();
    while( iter->hasNext() ) {
        iter->next()->render( identity );
    }

     delete iter;
     delete identity;
}
