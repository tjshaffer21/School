#if !defined (PICTURE_H)
#define PICTURE_H

#include <stdlib.h>
#include "math.h"
#include "Pixel.h"
#include "Scene.h"
#include "TransformNode.h"
#include "BasicObject.h"
#include "Vector.h"
#include "Light.h"
#include "AffineTransforms.h"
#include "Color.h"
#include "FileIO.h"
#include "Face.h"

#define    OBJ_FILE    "sphere.txt"
#define    TRS_FILE    "trs.txt"
#define    CMRA_FILE   "camera.txt"
#define    FOV_FILE    "fov.txt"
#define    SHADE_FILE  "shade.txt"
#define    LGHT_CLR    1,1,1
#define    AMB_CLR     1,0,0

class Picture
{
   public:
      static void render(Pixel* pixel);
      static BasicObject* readObject(char* fileName);
      static InstanceObject* buildInstanceObject(char* fileName,
          BasicObject* obj);
      static Matrix* buildCamera( char* filename, Vertex* eye );
      static void getZValues( char* fileName, double z[] );
      static void getShadingInfo( char* fileName, double s_array[] );
      
   private:
};

#endif
