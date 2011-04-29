/**
*    Face
*    Thomas Shaffer
*    28 October 2009
*    Face class takes a select number of vertices and renders them.
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
    vertices->removeAll();
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
List<Vertex>* Face::getVertices() { return vertices; }

/**
*    render
*    Renders a face using either DDA or  Bresenham's Algorithm.
*    Preconditions:
*        A pointer to the face object.
*        A pointer to the pixel object.
*/
void Face::render( Matrix* wnd, Pixel* pix, Color* material, Matrix* zbuffer )
{
    // Transform vertices from local to window.
    Vertex* p = vertices->get(1)->multiply(wnd);
    Vertex* q = vertices->get(2)->multiply(wnd);
    Vertex* r = vertices->get(3)->multiply(wnd);

    //p->setColor( material );
    //q->setColor( material );
    //r->setColor( material );

    Color *p_color = new Color( 1, 0, 0 );
    Color *q_color = new Color( 0, 1, 0 );
    Color *r_color = new Color( 0, 0, 1 );

    p->setColor( p_color );
    q->setColor( q_color );
    r->setColor( r_color );
    
    p->homogenize();
    q->homogenize();
    r->homogenize();

    if ( backface(p, q, r) == 1 ) {
        renderBarycentric( pix, p, q, r, zbuffer );
    }

    delete p;
    delete q;
    delete r;
    
    delete p_color;
    delete q_color;
    delete r_color;
}

/**
*    Renders the scene using barycentric coordinates.
*    Preconditions:
*        The pixel object used to draw scene.
*        The three vertices of the triangle.
*        The z-buffer matrix.
*    Postconditions:
*        The zbuffer is modified to handle z clipping.
*        The scene is clipped in x and y directions.
*        The scene is rendered and the color of the sphere is interpolated.
*/
void Face::renderBarycentric( Pixel* pix, Vertex* p, Vertex* r, Vertex* q,
    Matrix* zbuffer )
{
    double x_min = std::min( p->getX(), q->getX() );
    x_min = std::min( x_min, r->getX() );

    double x_max = std::max( p->getX(), q->getX() );
    x_max = std::max( x_max, r->getX() );

    double y_min = std::min( p->getY(), q->getY() );
    y_min = std::min( y_min, r->getY() );

    double y_max = std::max( p->getY(), q->getY() );
    y_max = std::max( y_max, r->getY() );

    if ( x_min < 0 ) { x_min = 0; }

    if ( x_max > zbuffer->getNumColumns() ) {
        x_max = zbuffer->getNumColumns(); 
    }

    if ( y_min < 0 ) { y_min = 0; }
    if ( y_max > zbuffer->getNumRows() ) { y_max = zbuffer->getNumRows(); }
    
    Vector* s = q->subtract( p );
    Vector* t = r->subtract( p );

    double px = ((int)x_min) - p->getX();
    double py = ((int)y_min) - p->getY();

    double denom = 1.0/((s->getX() * t->getY()) - (s->getY() * t->getX()));
    double alpha = ((px * t->getY()) - (py * t->getX())) * denom;
    double beta = ((py * s->getX()) - (px * s->getY())) * denom;
    
    double t_alpha;
    double t_beta;
    
    double alpha_incx = t->getY() * denom;
    double beta_incx = (-1.0 * (s->getY())) * denom;
    
    double alpha_incy = (-1.0 * (t->getX())) * denom;
    double beta_incy = s->getX() * denom;
    
    double z = 0;
    double red = 0;
    double green = 0;
    double blue = 0;

    for ( int y = ((int)y_min); y <= ((int)y_max); y++ ) {
         t_alpha = alpha;
         t_beta = beta;
         for ( int x = ((int)x_min); x <= ((int)x_max); x++ ) {
            if ( (alpha >= 0.0 && alpha <= 1.0 && beta >= 0.0 && beta <= 1.0 && 
                    (alpha+beta) <= 1.0) ) {
                z = (alpha * s->getZ()) + (beta * t->getZ()) + p->getZ();
            
                if ( zbuffering( x, y, z, zbuffer ) == 1 ) {
                    red = (alpha*(q->getRed() - p->getRed())+
                        (beta*(r->getRed() - p->getRed())) + p->getRed());
                    green = (alpha*(q->getGreen()-p->getGreen())+
                        (beta*(r->getGreen()-p->getGreen()))+p->getGreen());
                    blue = (alpha*(q->getBlue()-p->getBlue())+
                        (beta*(r->getBlue()-p->getBlue()))+p->getBlue());
                    Color* color = new Color(red, green, blue );
                    pix->drawPixel( x,y, *color );
                    delete color;
                }
            }
            alpha += alpha_incx;
            beta += beta_incx;
        }
        alpha = t_alpha;
        beta = t_beta;
        alpha += alpha_incy;
        beta += beta_incy;
    }

    delete s;
    delete t;
}

/**
*    Performs z-buffering on the pixels.
*    Preconditions:
*        The x,y,z values.
*        The zbuffer matrix.
*    Postconditions:
*        Updates the zbuffer.
*        Returns 1 if pixel is closer, else 0.
*/
int Face::zbuffering( int x, int y, double z, Matrix* zbuffer )
{
    double zvalue = zbuffer->getElement( y, x );
    if ( z <= 1.0 && z > zvalue ) {
            zbuffer->setElement( y, x, z );
            return 1;
    }

    return 0;
}

/**
*   Perfroms back-face culling on the object.
*   Preconditions:
*      The there vertices to perform culling on.
*   Postconditions:
*      Returns 1 if object is in the plane, else 0.
*/
int Face::backface( Vertex* p, Vertex* q, Vertex* r )
{
    Vector* e1 = r->subtract( p );
    Vector* e2 = r->subtract( q );

    Vector* n = e1->crossProduct( e2 );

    if ( n->getZ() <= 0 ) { 
        delete e1;
        delete e2;
        delete n;
        return 1;
    }

    delete e1;
    delete e2;
    delete n;
    return 0;
}
