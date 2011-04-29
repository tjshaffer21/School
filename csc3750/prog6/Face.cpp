/**
*    Face
*    Thomas Shaffer
*    28 October 2009
*    Face class takes a select number of vertices and renders them.
*/
#include "Face.h"
#include <iostream>
Face::Face() { vertices = new List<Vertex>(); }
Face::~Face() { delete vertices; }

/**
*    addVertex
*    Adds a vertex into the object's list of vertices.
*    Preconditions:
*        A pointer to the memory address of the vertex.
*    Postconditions:
*        The vertex is added into the list of vertices.
*/
void Face::addVertex( Vertex* vertex ) { vertices->add( vertex ); }

/**
*    getVertices
*    Returns a list of vertices.
*    Preconditions:
*        None.
*    Postconditions:
*        Returns a list of vertices.
*/
List<Vertex>* Face::getVertices() { return vertices; }

Vertex* Face::getVertex( int i )
{
    return vertices->get(i);
}

/**
*    getNormal
*    Obtains the non-normalized normal of the three given vertices.
*    Preconditions:
*        The three vertices of the Face object.
*    Postconditions:
*        Returns the non-normalized normal of the face.
*/
Vector* Face::getNormal( Vertex* p, Vertex* q, Vertex* r )
{
    Vector* e1 = r->subtract( p );
    Vector* e2 = r->subtract( q );
    Vector* n = e1->crossProduct( e2 );

    delete e1;
    delete e2;

    return n;
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
int Face::backface( Vertex* p, Vertex* q, Vertex* r)
{
    Vector* n = getNormal( p, q, r );
    int ret = 1;

    if ( n->getZ() <= 0 ) { ret = 0; }

    delete n;
    return ret;
}

/**
*    obtainShading
*    Performs Interpolative shading on the Face object.
*    Preconditions:
*        The current Vertex that is being manipulated.
*        The normal vector.
*        The light object.
*        The ambient color.
*        The eye point.
*        The attenuation value.
*        The shininess level.
*    Postconditions:
*        Returns the color obtained using the algorithm.
*/
Color* Face::obtainShading( Vertex* m, Vector* n, Light* light, Color* amb,
    Vertex* eye, double attenuation, double shininess )
{
    Vector* l = light->getLocation()->subtract(m);    // Non-normalized
    Vector* l_n = light->getLocation()->subtract(m);  // Normalized
    l_n->normalize();

    // Obtain r.
    double r_dot = 2.0*(l_n->dotProduct( n ));
    double rx = (r_dot * n->getX()) - l_n->getX();
    double ry = (r_dot * n->getY()) - l_n->getY();
    double rz = (r_dot * n->getZ()) - l_n->getZ();
    Vector* rr = new Vector( rx, ry, rz );

    // Obtain v.
    Vector* v = eye->subtract( m );
    v->normalize();

    double lndot = l_n->dotProduct( n );
    if ( lndot < 0 ) { lndot = 0; }
    delete l_n;

    double rvdot = rr->dotProduct(v);
    if ( rvdot < 0 ) { rvdot = 0; }
    delete v;
    delete rr;

    // Falloff
    double bd = attenuation * sqrt( l->dotProduct(l) );
    delete l;

    // Obtain cr
    double c_diffr = m->getRed();
    double cr = (((c_diffr * light->getColor()->getRed() * lndot) +
        (light->getColor()->getRed() * pow( rvdot, shininess ))) / bd) +
        amb->getRed() * IA;

    // Obtain cb
    double c_diffb = m->getBlue();
    double cb = (((c_diffb * light->getColor()->getBlue() * lndot) +
        (light->getColor()->getBlue() * pow( rvdot, shininess ))) / bd) +
        amb->getBlue() * IA;

    // Obtain cg
    double c_diffg = m->getGreen();
    double cg = (((c_diffg * light->getColor()->getGreen() * lndot) +
        (light->getColor()->getGreen() * pow( rvdot, shininess ))) / bd) +
        amb->getGreen() * IA;

    Color* color = new Color( cr, cg, cb );
    return color;
}

/**
*    avgNormal
*    Obtain the average normal for a vertex that is within multiply Faces.
*    Preconditions:
*        The vertex that needs to be normalized.
*    Postconditions:
*        Returns a Vector pointer to the averaged normal.
*/
Vector* Face::avgNormal( Vertex* m )
{
    Vector* normal;

    List<Face>* f = m->getFaces();
    List<Vector>* nml = new List<Vector>();

    // Obtain all the normals for each face and add into a list.
    ListIterator<Face>* iter = f->iterator();
    int counter = 0;
    while ( iter->hasNext() ) {
        Face* fac = iter->next();
        nml->add( fac->getNormal( fac->getVertex(1), fac->getVertex(2),
            fac->getVertex(3) ) );
        counter++;
    }
    delete iter;


// MEMORY LEAK---------------------------------------------------------------------
    ListIterator<Vector>* iter2 = nml->iterator();
    Vector* tmp = iter2->next();
    while ( iter2->hasNext() ) {
        Vector* tmp2 = iter2->next();
        normal = tmp->addition( tmp2 );
        delete tmp;
        delete tmp2;
        tmp = normal;
    }
    delete iter2;

    tmp = normal->division( ((double)counter) );
    delete normal;
    normal = tmp;
// END MEMORY Leak=================================================================

    delete nml;
    return normal;
}

/**
*    render
*    Base render method that performs the calculations necessary before the
*    render equation is called.
*    Preconditions:
*        A pointer to the Pixel object that draws the pixels.
*        A pointer to the windows transform Matrix (WNAC).
*        A pointer to the Instance Matrix (TRS (I)).
*        A pointer to the zbuffer Matrix.
*        A pointer to the Light object.
*        A pointer to the eye point (Vertex).
*        A pointer to the Color of the object's material.
*        A pointer to the ambient light Color.
*        The attenuation value (double).
*        The shininess value (double).
*      
*    Postconditions:
*        Performs necessary preconditions before calling another render method
*        to perform the actual rendering equation.
*/
void Face::render( Pixel* pix, Matrix* wnd, Matrix* trs, Matrix* zbuffer,
    Light* light, Vertex* eye, Color* material, Color* amb, double attenuation,
    double shininess )
{
    // Obtain the average normal.
    Vector* tmp_np = avgNormal( vertices->get(1) );
    Vector* tmp_nq = avgNormal( vertices->get(2) );
    Vector* tmp_nr = avgNormal( vertices->get(3) );

    Vector* np = tmp_np->multiply( trs );
    Vector* nq = tmp_nq->multiply( trs );
    Vector* nr = tmp_nr->multiply( trs );
    
    np->normalize();
    nq->normalize();
    nr->normalize();
    
    delete tmp_np;
    delete tmp_nq;
    delete tmp_nr;
    
    Vertex* p = vertices->get(1)->multiply(trs);
    Vertex* q = vertices->get(2)->multiply(trs);
    Vertex* r = vertices->get(3)->multiply(trs);

    // Lighting Computations
    p->setColor( material );
    q->setColor( material );
    r->setColor( material );
    
    // Shading for vertex 1.
    Color* color = obtainShading(  p, np, light, amb, eye, attenuation,
        shininess );
    p->setRed( color->getRed() );
    p->setGreen( color->getGreen() );
    p->setBlue( color->getBlue() );
    delete color;

    // Shading for vertex 2.
    color = obtainShading( q, nq, light, amb, eye, attenuation,shininess );
    q->setRed( color->getRed() );
    q->setGreen( color->getGreen() );
    q->setBlue( color->getBlue() );
    delete color;

    // Shading for vertex 3.
    color = obtainShading( r, nr, light, amb, eye, attenuation,shininess );
    r->setRed( color->getRed() );
    r->setGreen( color->getGreen() );
    r->setBlue( color->getBlue() );
    delete color;

    Vertex* tmp = p->multiply( wnd );
    delete p;
    p = tmp;
    tmp = q->multiply( wnd );
    delete q;
    q = tmp;
    tmp = r->multiply( wnd );
    delete r;
    r = tmp;

    p->homogenize();
    q->homogenize();
    r->homogenize();

    if ( backface(p, q, r) == 0 ) {
        renderBarycentric( pix, p, q, r, zbuffer );
    }

    delete np;
    delete nq;
    delete nr;

    delete p;
    delete q;
    delete r;
}

/**
*    renderBarycentric
*    Renders the scene using barycentric coordinates.
*    Preconditions:
*        The Pixel object used to draw scene.
*        The three vertices of the triangle.
*        The z-buffer Matrix.
*    Postconditions:
*        The zbuffer is modified to handle z clipping.
*        The scene is clipped in x and y directions.
*        The scene is rendered and the color of the sphere is interpolated.
*/
void Face::renderBarycentric( Pixel* pix, Vertex* p, Vertex* r, Vertex* q,
    Matrix* zbuffer )
{
    // Bounding rectangle.
    double x_min = std::min( p->getX(), q->getX() );
    x_min = std::min( x_min, r->getX() );

    double x_max = std::max( p->getX(), q->getX() );
    x_max = std::max( x_max, r->getX() );

    double y_min = std::min( p->getY(), q->getY() );
    y_min = std::min( y_min, r->getY() );

    double y_max = std::max( p->getY(), q->getY() );
    y_max = std::max( y_max, r->getY() );

    // X & Y clipping
    double minX = -0.5;
    double minY = -0.5;
    double maxY = zbuffer->getNumRows() - 1 + 0.5;
    double maxX = zbuffer->getNumColumns() - 1 + 0.5;
 
    if ( x_min < minX ) { x_min = minX; }
    if ( x_max > maxX ) { x_max = maxX; }
    if ( y_min < minY ) { y_min = minY; }
    if ( y_max > maxY ) { y_max = maxY; }

    // Subtractions.
    double sub_red_qp = q->getRed() - p->getRed();
    double sub_red_rp = r->getRed() - p->getRed();
    double sub_blu_qp = q->getBlue() - p->getBlue();
    double sub_blu_rp = r->getBlue() - p->getBlue();
    double sub_grn_qp = q->getGreen() - p->getGreen();
    double sub_grn_rp = r->getGreen() - p->getGreen();
    double sub_z_qp = q->getZ() - p->getZ();
    double sub_z_rp = r->getZ() - p->getZ();
   
    Vector* s = q->subtract( p );
    Vector* t = r->subtract( p );

    // curr - specified point
    double px = ((int)x_min - p->getX());
    double py = ((int)y_min - p->getY());

    // Calculate alpha and beta.
    double denom = 1.0/((s->getX() * t->getY()) - (s->getY() * t->getX()));
    double alpha = ((px * t->getY()) - (py * t->getX())) * denom;
    double beta = ((py * s->getX()) - (px * s->getY())) * denom;

    // Render calcuations using alpha and beta.
    double red = (alpha * sub_red_qp) + (beta * sub_red_rp) + p->getRed();
    double grn = (alpha * sub_grn_qp) + (beta * sub_grn_rp) + p->getGreen();
    double blu = (alpha * sub_blu_qp) + (beta * sub_blu_rp) + p->getBlue();
    double z = (alpha * s->getZ()) + (beta * t->getZ()) + p->getZ();
    
    // Temporary variables
    double t_alpha;
    double t_beta;
    double t_r;
    double t_g;
    double t_b;
    double t_z;
    
    // Increments
    double alpha_incx = t->getY() * denom;
    double beta_incx = (-1.0 * (s->getY())) * denom;
    double r_incx = (alpha_incx * sub_red_qp) + (beta_incx * sub_red_rp); 
    double g_incx = (alpha_incx * sub_grn_qp) + (beta_incx * sub_grn_rp); 
    double b_incx = (alpha_incx * sub_blu_qp) + (beta_incx * sub_blu_rp);
    double z_incx = (alpha_incx * sub_z_qp) + (beta_incx * sub_z_rp);

    double alpha_incy = (-1.0 * (t->getX())) * denom;
    double beta_incy = s->getX() * denom;
    double r_incy = (alpha_incy * sub_red_qp) + (beta_incy * sub_red_rp);
    double g_incy = (alpha_incy * sub_grn_qp) + (beta_incy * sub_grn_rp);
    double b_incy = (alpha_incy * sub_blu_qp) + (beta_incy * sub_blu_rp);  
    double z_incy = (alpha_incy * sub_z_qp) + (beta_incy * sub_z_rp);

    // Iterate across the screen and see if the pixel will be drawn.
    for ( int y = ((int)y_min); y <= ((int)y_max); y++ ) {
         // Temp values := curr values
         t_alpha = alpha;
         t_beta = beta;
         
         t_r = red;
         t_g = grn;
         t_b = blu;
         
         t_z = z;

         for ( int x = ((int)x_min); x <= ((int)x_max); x++ ) {
            // 0.0 <= alpha <= 1.0; 0.0 <= beta <= 1.0; (alpha+beta) <= 1.0
            if ( (t_alpha >= 0.0 && t_alpha <= 1.0 && t_beta >= 0.0 &&
              t_beta <= 1.0 && (t_alpha+t_beta) <= 1.0) ) {
                if ( zbuffering( x, y, t_z, zbuffer ) == 1 ) {
                    Color color = Color(t_r, t_g, t_b );
                    pix->drawPixel( x,y, color );
                }
            }

            // Update temp values.
            t_alpha += alpha_incx;
            t_beta += beta_incx;
            
            t_r += r_incx;
            t_g += g_incx;
            t_b += b_incx;
            
            t_z += z_incx;
        }

        // Update the curr values.
        alpha += alpha_incy;
        beta += beta_incy;
        
        red += r_incy;
        grn += g_incy;
        blu += b_incy;
        
        z += z_incy;
    }

    delete s;
    delete t;
}
