#include "Light.h"

Light::Light()
{
   location = new Vertex(0, 0, 0);
   color = new Color(1.0, 1.0, 1.0);
}

Light::~Light()
{
   delete location;
   delete color;
}

void Light::setLocation(Vertex* l)
{
   Vertex* temp = location;
   location = l;
   delete temp;
}

void Light::setColor(Color* c)
{
   Color* temp = color;
   color = c;
   delete temp;
}

Vertex* Light::getLocation()
{
   return location;
}

Color* Light::getColor()
{
   return color;
}
