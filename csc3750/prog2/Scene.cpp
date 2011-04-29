/**
*    Thomas Shaffer
*    Scene
*    28 Septemeber 2009
*
*/
#include "Scene.h"

Scene::Scene( Matrix* wndm )
{
    scene = new List<InstanceObject>();
    wnd = wndm;
}

Scene::~Scene(){}

void Scene::addIO( InstanceObject* io )
{
    scene->add( io );
}

void Scene::render( Pixel* pix )
{
    ListIterator<InstanceObject>* iter = scene->iterator();
    while( iter->hasNext() ) {
        iter->next()->render( wnd, pix );
    }
}
