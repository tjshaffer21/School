/**
*    Face
*    Thomas Shaffer
*    16 September 2009
*    Face class takes a select number of vertices (three) and connects them
*    using either DDA algorithm or Bresenham's algorithm.
*/
#include "Face.h"

/**
*   Face
*    Constructor that creates an empty face object.
*    Preconditions:
*        None.
*    Postconditions:
*        Instantiates private variables
*/
Face::Face()
{
    vertices = new List<Vertex>();
}

/**
*    ~Face
*    Deconstructor.
*    Preconditions:
*        None.
*    Postconditions:
*        Destroys the Face object.
*/
Face::~Face()
{
    ListIterator<Vertex>* iter = vertices->iterator();
    while( iter->hasNext() ) {
            delete iter->next();
    }
    delete iter;

    delete vertices;
}

/**
*    addVertex
*    Adds a vertex into the object's list of vertices.
*    Preconditions:
*        A pointer to the memory address of the vertex.
*    Postconditions:
*        The vertex is added into the list of vertices.
*/
void Face::addVertex( Vertex* vertex )
{
    vertices->add( vertex );
}

/**
*    getVertices
*    Returns a list of vertices.
*    Preconditions:
*        None.
*    Postconditions:
*        Returns a list of vertices.
*/
List<Vertex>* Face::getVertices()
{
    return vertices;
}

/**
*    render
*    Renders a face using either DDA or  Bresenham's Algorithm.
*    Preconditions:
*        A pointer to the face object.
*        A pointer to the pixel object.
*/
void Face::render( Matrix* wnd, Pixel* pix )
{
    // Transform vertices from local to window.
    Vertex* vrt_wnd = vertices->get(1)->multiply(wnd);
    Vertex* vrt_wnd2 = vertices->get(2)->multiply(wnd);
    Vertex* vrt_wnd3 = vertices->get(3)->multiply(wnd);

    // Render
    //renderDDA( vrt_wnd, vrt_wnd2, pix );
    //renderDDA( vrt_wnd2, vrt_wnd3, pix );
    //renderDDA( vrt_wnd, vrt_wnd3, pix );

    renderBresenham( vrt_wnd, vrt_wnd2, pix );
    renderBresenham( vrt_wnd2, vrt_wnd3, pix );
    renderBresenham( vrt_wnd, vrt_wnd3, pix );


    delete vrt_wnd;
    delete vrt_wnd2;
    delete vrt_wnd3;
}

/**
*    renderDDA
*    Renders a line using the DDA algorithm. 
*    Preconditions:
*        Two vertices.
*        A pixel object.
*    Postsconditions:
*        Connects two dots onto a screen.
*/
void Face::renderDDA(Vertex* v1, Vertex* v2, Pixel* pix )
{
    //Color color = Color( 1, 1, 1, 1 );
    srand( time(NULL) );

    // Obtain x and y for vertices.
    float y1 = v1->getY();
    float y2 = v2->getY();
    float x1 = v1->getX();
    float x2 = v2->getX();

    float dx = x2-x1;
    float dy = y2-y1;
    float m = dy/dx;  // Obtain slope.
    float b = y2 - (m*x2);

    int y_curr = 0;
    int y_end = 0;
    int x_curr = 0;
    int x_end = 0;

    // Check if absolute value of slope > 1 or if x1 & x2 are equal.
    if ( fabs(m) > 1 || x1 == x2 ) {
            m = dx/dy;
            b = x2 - (m*y2);
            
            // If y1 > y2 then y_curr is y2,
            // else y_curr is y1
            if ( y1 > y2 ) {
                    y_curr = (int)(y2+0.5);
                    y_end = (int)(y1+0.5);
            } else {
                    y_curr = (int)(y1+0.5);
                    y_end = (int)(y2+0.5);
            }

            // Loop over y.
            float x = (y_curr*m)+b;
            while( y_curr <= y_end ) {
                    Color color = Color( ((float)rand())/RAND_MAX, ((float)rand())/RAND_MAX,
                          ((float)rand())/RAND_MAX, 1 );
                    int xi = (int)(x+0.5);
                    if (xi < 0) {
                              xi = (int)(x-0.5);
                    }
                    pix->drawPixel( xi, y_curr, color );

                    y_curr++;
                    x += m;
            }
    } else {
        // If y1 > y2 then y_curr is y2,
        // else y_curr is y1
        if ( x1 > x2 ) {
          x_curr = (int)(x2+0.5);
          x_end = (int)(x1+0.5);
        } else {
          x_curr = (int)(x1+0.5);
          x_end = (int)(x2+0.5);
        }

        // Loop over x.
        float y = (m*x_curr)+b;
        while ( x_curr <= x_end ) {
            Color color = Color( ((float)rand())/RAND_MAX, ((float)rand())/RAND_MAX,
                  ((float)rand())/RAND_MAX, 1 );
            int yi = (int)(y+0.5);
            if ( yi < 0 ) {
                    yi = (int)(y-0.5);
            }
            pix->drawPixel( x_curr, yi, color );

            x_curr++;
            y += m;
        }
    }
}

/**
*    renderBresenham
*    Renders a line using the Bresenham algorithm.
*    Preconditions:
*        Two vertices.
*        A pixel object.
*    Postsconditions:
*        Connects two dots onto a screen.
*/
void Face::renderBresenham( Vertex* v1, Vertex* v2, Pixel* pix )
{
    Color color = Color( 1, 1, 1, 1 );

    // Obtain x and y values.
    double x1 = v1->getX();
    double y1 = v1->getY();
    double x2 = v2->getX();
    double y2 = v2->getY();

    int dx = ((int)(x1+0.5)) - ((int)(x2+0.5));
    int dy = ((int)(y2+0.5)) - ((int)(y1+0.5));

    // Incremental values.
    int incx = 1;
    int incy = 1;
    int inc1, inc2;

    int fraction;
    int x, y;


    if ( dx < 0 ) {
        dx = -dx;
    }

    if ( dy < 0 ) {
        dy = -dy;
    }

    // Negate step size if x2/y2 < x1/y1
    if ( x2 < x1 ) {
        incx = -1;
    }

    if  ( y2 < y1 ) {
        incy = -1;
    }

    // Round
    x=((int)(x1+0.5));
    y=((int)(y1+0.5));

    if ( dx > dy ) {        // Loop over x.
        pix->drawPixel( x, y, color );
        fraction = 2*dy-dx;
        inc1 = 2*(dy-dx);
        inc2 = 2*dy;

        for( int i = 0; i < dx; i++ ) {
            if ( fraction >= 0 ) {
                y += incy;
                fraction += inc1;
            } else {
                fraction += inc2;
            }

            x += incx;
            pix->drawPixel( x, y, color );
        }
    } else {        // Loop over y.
        pix->drawPixel( x, y, color );
        fraction = 2*dx-dy;
        inc1 = 2*(dx-dy);
        inc2 = 2*dx;

        for( int i = 0; i < dy; i++ ) {
            if ( fraction >= 0 ) {
                x += incx;
                fraction += inc1;
            } else {
                fraction += inc2;
            }

            y += incy;

            pix->drawPixel( x,y, color );
        }
    }
}
