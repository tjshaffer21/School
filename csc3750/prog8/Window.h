/** Class provided by Dr. Boshart. */
#if !defined (WINDOW_H)
#define WINDOW_H

class Window
{

   public:
      Window(int argc, char** argv, int width, int height, int xLoc, int yLoc, char* title);
      ~Window();
      void show();
      static void reshape(int w,int h);
      static void display(int w, int h);
      static void staticDisplay();  

   private:

};

#endif
