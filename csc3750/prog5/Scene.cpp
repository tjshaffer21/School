/**
*    Thomas Shaffer
*    Scene
*    28 Septemeber 2009
*    Creates a scene for the application.
*/
#include "Scene.h"

Scene::Scene( Matrix* wndm )
{
    scene = new List<Node>();
    wnd = wndm;
}

Scene::~Scene()
{
    ListIterator<Node>* iter = scene->iterator();
    while ( iter->hasNext() ) {
        iter->next()->removeRef();
    }
    delete iter;

    delete scene;
    delete wnd;
}


/**
*    addNode
*    Adds an Instance Object to the Scene.
*    Preconditions:
*        An InstanceObject.
*    Postconditions:
*        None.
*/
void Scene::addNode( Node* io )
{
    io->addRef();
    scene->add( io );
}

/**
*    render
*    Renders the scene.
*    Preconditions:
*        The pixel object.
*    Postconditions:
*        The scene is rendered when the final render method is reached.
*/
void Scene::render( Pixel* pix )
{
    Matrix* zbuffer = new Matrix( pix->getHeight(), pix->getWidth() );
    for ( int i = 0; i < pix->getHeight(); i++ ) {
         for ( int j = 0; j < pix->getWidth(); j++ ) {
             zbuffer->setElement( i, j, -1 );
         }
    }
    
    ListIterator<Node>* iter = scene->iterator();
    while( iter->hasNext() ) {
        iter->next()->render( pix, wnd, zbuffer );
    }
    delete iter;
    delete zbuffer;
}
