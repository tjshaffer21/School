/**
*    Thomas Shaffer
*    Scene
*    27 Nov. 2009
*    Scene object holds the overall scene information.
*/
#if !defined  (SCENE_H)
#define SCENE_H

#include "FileIO.h"
#include "InstanceObject.h"
#include "List.h"
#include "AffineTransforms.h"
#include "Light.h"

#define    FOV_FILE    "fov.txt"
class Scene
{
    private:
        List<Node>* scene;

    public:
        Scene();
        virtual ~Scene();
        void addNode( Node* node );
        static void getZValues( char* fileName, double z[] );
        void render( int w, int h, Light* light, Vertex* eyepoint, 
            Vertex* atpoint, Color* ambient );
};
#endif
