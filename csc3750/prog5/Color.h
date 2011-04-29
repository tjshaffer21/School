#if !defined (COLOR_H)
#define COLOR_H

class Color
{

   private:
      double red;
      double green;
      double blue;
      double alpha;
      double clampColor(double c);
      void clamp(double r, double g, double b, double a);

   public:
      Color(double r,double g, double b, double a);
      Color(double r, double g, double b);
      ~Color();
      double getRed();
      double getGreen();
      double getBlue();
      double getAlpha();
      void setRed(double r);
      void setGreen(double g);
      void setBlue(double b);
      void setAlpha(double a);
      void setColor();  //sets the color of a single pixel using an OpenGL command
};

#endif
