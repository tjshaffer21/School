#if !defined (AFFINETRANSFORMS_H)
#define AFFINETRANSFORMS_H

#include "Matrix.h"
#include "Vertex.h"
#include <math.h>

#define RADIANS 0.0174532925

class AffineTransforms
{
   private:

   public:
      static Matrix* scale(double x, double y, double z);
      static Matrix* translate( double ax, double ay, double az);
      static Matrix* rotateX( double degrees );
      static Matrix* rotateY( double degrees );
      static Matrix* rotateZ( double degrees );
      static Matrix* windowingTransform( double width, double height );
      static Matrix* aspectRatio( double width, double height, double fov );
      static Matrix* perspective( double z_max, double z_min );
      static Matrix* buildCamera( Vertex* eye, Vertex* at, Vector* up );
};

#endif
