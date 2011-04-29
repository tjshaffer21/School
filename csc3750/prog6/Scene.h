#if !defined  (SCENE_H)
#define SCENE_H
/**
*    Thomas Shaffer
*    Scene
*    28 Septemeber 2009
*
*/
#include "InstanceObject.h"

class Scene
{
    private:
        List<Node>* scene;
        Matrix* wnd;
	Matrix* zbuffer;

    public:
        Scene( Matrix* wnd );
        virtual ~Scene();
        void addNode( Node* node );
        void render( Pixel* pix, Light* light, Vertex* eye, Color* amb,
            double attenuation );
};
#endif
