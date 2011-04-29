#include <GL/glut.h>
#include "Color.h"

double Color::clampColor(double c)
{
   if (c > 1.0)
   {
      c = 1.0;
   }
   else if (c<0.0)
   {
      c = 0.0;
   }

   return c;
}

void Color::clamp(double r, double g, double b, double a)
{
   red = clampColor(r);
   green = clampColor(g);
   blue = clampColor(b);
   alpha = clampColor(a);
}

Color::Color(double r, double g, double b, double a)
{
   clamp(r, g, b, a);
}

Color::Color(double r, double g, double b)
{
   clamp(r, g, b, 1.0);
}

Color::~Color()
{}

void Color::setRed(double r)
{
   red = clampColor(r);
}

void Color::setGreen(double g)
{
   green = clampColor(g);
}

void Color::setBlue(double b)
{
   blue = clampColor(b);
}

void Color::setAlpha(double a)
{
   alpha = clampColor(a);
}

double Color::getRed()
{
   return red;
}

double Color::getGreen()
{
   return green;
}

double Color::getBlue()
{
   return blue;
}

double Color::getAlpha()
{
   return alpha;
}

void Color::setColor()
{
   glColor3f(red, green, blue);
}
