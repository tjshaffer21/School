#if !defined (WINDOW_H)
#define WINDOW_H

#include "Pixel.h"

class Window
{

   public:
      Window(int argc, char** argv, int width, int height, int xLoc, int yLoc, const char* title);
      ~Window();
      void show();
      static void reshape(int w,int h);
      static void display(Pixel* pixel);
      static void staticDisplay();  

   private:

};
#endif
