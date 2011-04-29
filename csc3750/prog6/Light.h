#if !defined (LIGHT_H)
#define LIGHT_H

#include "Vertex.h"

class Light
{
   private:
      Vertex* location;
      Color* color;

   public:
      Light();
      virtual ~Light();
      void setColor(Color* color);
      void setLocation(Vertex* vertex);
      Color* getColor();
      Vertex* getLocation();
};

#endif
