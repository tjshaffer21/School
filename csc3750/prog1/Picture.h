#if !defined (PICTURE_H)
#define PICTURE_H

#include <stdlib.h>
#include "Pixel.h"
#include "BasicObject.h"
#include "FileIO.h"
#include "AffineTransforms.h"
#include "Color.h"


class Picture
{
   public:
      static void render(Pixel* pixel);
      static BasicObject* readObject(const char* fileName);
      //static InstanceObject* buildInstanceObject(char* fileName, BasicObject* obj);

   private:
};

#endif
