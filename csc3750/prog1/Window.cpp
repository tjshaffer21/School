#include <GL/glut.h>
#include "Window.h"
#include "Picture.h"

void Window::staticDisplay()
{
   int width, height;
   width = glutGet(GLUT_WINDOW_WIDTH);
   height = glutGet(GLUT_WINDOW_HEIGHT);
   reshape(width, height);
}

void Window::display(Pixel* pixel)  //callback for window drawing
{
   //the main function for display
   glMatrixMode(GL_MODELVIEW);  //switch to model view mode for display
   glClear(GL_COLOR_BUFFER_BIT);  //clear the color buffer
   glEnable(GL_CULL_FACE);

   //the picture object contains the scene to be rendered (where virtually all of your work is done)
   //all rendering is done in screen coordinates
   //the pixel object is passed to it so that it can be rendered in OpenGL
   Picture picture;
   picture.render(pixel);

   glutSwapBuffers();  //for double buffering
   glFlush();  //forces completion of drawing
}

void Window::reshape(int w, int h)  //callback for window resizing
{
   //the pixel object will draw the requested pixel in screen coordinates
   Pixel pixel(w, h);

   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);  //set up the orthogonal projection
   
   glLoadIdentity();
   glOrtho(-1.0, 1.0, -1.0, 1.0, -100, 100);  //assumes all vertices are in the range -1 to 1 in x and y (accepts nearly any z)

   display(&pixel);  //need this here to make the callback work correctly
}

void Window::show()
{
   glutMainLoop();  //show everything
}

Window::~Window(){}

Window::Window(int argc, char** argv, int width, int height, int xLoc, int yLoc, const char* title)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  //use rgb color and double buffering 
   
   glutInitWindowSize(width, height);

   glutInitWindowPosition(xLoc, yLoc);  //place the window near the center of the screen
   glutCreateWindow(title);

   glClearColor(0.0, 0.0, 0.0, 0.0);  //background color is black

   glutDisplayFunc(Window::staticDisplay);  //set the repaint callback function (redraws after window has been resized)
   glutReshapeFunc(Window::reshape);        //set the reshape callback function (redraws while window is resized)
}
