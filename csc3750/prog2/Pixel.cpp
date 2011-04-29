#include "GL/glut.h"
#include "Pixel.h"

Pixel::Pixel(int width, int height)
{
   xWindow = width;
   yWindow = height;
}

Pixel::~Pixel(){}

void Pixel::setWidth(int width)
{
   xWindow = width;
}

void Pixel::setHeight(int height)
{
   yWindow = height;
}

int Pixel::getWidth()
{
   return xWindow;
}

int Pixel::getHeight()
{
   return yWindow;
}

void Pixel::drawPixel(int xPixel, int yPixel, Color color)
{
   double xCanonical;
   double yCanonical;

   //convert from screen coordinates to projected world coordinates and plot one pixel
   //the calculation below is the "inverse" transform from screen coords to world coords that OpenGL wants
   xCanonical = (xPixel*2.0)/xWindow + (-1.0*(xWindow - 1))/xWindow;
   yCanonical = (-1.0*yPixel*2.0)/yWindow + (1.0*(yWindow - 1))/yWindow;

   color.setColor();  //makes an opengl call for me

   //plot a point
   glBegin(GL_POINTS);

      glVertex2f((float) xCanonical, (float) yCanonical);

   glEnd();
}
