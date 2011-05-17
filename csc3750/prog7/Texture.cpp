#include "Texture.h"

Texture::Texture(char* fileName, int w, int h)
{
   textureFile = fileName;
   width = w;
   height = h;
}

Texture::~Texture() { delete[] textureFile;}

char* Texture::getFileName() { return textureFile; }
int Texture::getWidth() { return width; }
int Texture::getHeight() { return height; }
