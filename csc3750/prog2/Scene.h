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
        List<InstanceObject>* scene;
        Matrix* wnd;

    public:
        Scene( Matrix* wnd );
        virtual ~Scene();
        void addIO( InstanceObject* io );
        void render( Pixel* pix );
};
#endif
