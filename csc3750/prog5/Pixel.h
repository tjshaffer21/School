#if !defined (PIXEL_H)
#define PIXEL_H

#include "Color.h"

class Pixel
{
   public:
      Pixel(int width, int height);
      ~Pixel();
      int getWidth();
      int getHeight();
      void setWidth(int width);
      void setHeight(int height);
      void drawPixel(int xPixel, int yPixel, Color color);

   private:
      int xWindow;
      int yWindow;
};

#endif
