#if !defined (TEXTURE_H)
#define TEXTURE_H

class Texture
{
   private:
      char* textureFile;
      int width;
      int height;

   public:
      Texture(char* texFile, int w, int h);
      virtual ~Texture();
      char* getFileName();
      int getWidth();
      int getHeight();
};

#endif
