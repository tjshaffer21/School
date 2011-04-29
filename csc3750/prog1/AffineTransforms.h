#if !defined (AFFINETRANSFORMS_H)
#define AFFINETRANSFORMS_H

#include "Matrix.h"

class AffineTransforms
{
   private:

   public:;
      static Matrix* scale(double x, double y, double z);
      static Matrix* windowingTransform( double width, double height );
      static Matrix* aspectRatio( double width, double height );
};

#endif
