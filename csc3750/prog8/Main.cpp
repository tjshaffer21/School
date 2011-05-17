#include "Window.h"

int main(int argc, char** argv)
{
   Window window(argc, argv, 800, 600, 450, 250, "Computer Graphics");
   window.show();
   return 0;
}
