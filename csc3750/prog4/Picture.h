#if !defined (PICTURE_H)
#define PICTURE_H

#include <stdlib.h>
#include "Pixel.h"
#include "BasicObject.h"
#include "FileIO.h"
#include "AffineTransforms.h"
#include "Color.h"
#include "Scene.h"
#include "TransformNode.h"
#include "Vector.h"
#include "math.h"


class Picture
{
   public:
      static void render(Pixel* pixel);
      static BasicObject* readObject(char* fileName);
      static InstanceObject* buildInstanceObject(char* fileName, BasicObject* obj);
      static Matrix* buildCamera( char* filename );
      static void getZValues( char* fileName, double z[] );
      
   private:
};

#endif
