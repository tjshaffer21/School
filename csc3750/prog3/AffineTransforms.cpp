#include "AffineTransforms.h"

Matrix* AffineTransforms::scale(double x, double y, double z)
{
   Matrix* S = new Matrix(4, 4);

   S->setElement(1,1,x);
   S->setElement(1,2,0);
   S->setElement(1,3,0);
   S->setElement(1,4,0);

   S->setElement(2,1,0);
   S->setElement(2,2,y);
   S->setElement(2,3,0);
   S->setElement(2,4,0);

   S->setElement(3,1,0);
   S->setElement(3,2,0);
   S->setElement(3,3,z);
   S->setElement(3,4,0);

   S->setElement(4,1,0);
   S->setElement(4,2,0);
   S->setElement(4,3,0);
   S->setElement(4,4,1);

   return S;
}

/**
*    windowingTransform
*    Creates a matrix that holds the values for the windowing transform,
*    so that we can convert local coordinates to world coordinates. 
*    Preconditions:
*        A double called getWidth which is the width of the window.
*        A double called getHeight which is the height of the window.
*    Postconditions:
*        Returns a pointer to the matrix.
*/
Matrix* AffineTransforms::windowingTransform( double width, double height )
{
    Matrix* wndw_mtrx = new Matrix(4,4);

    // Change necessary elements for the method.
    wndw_mtrx->setElement( 1, 1, width/2.0 );
    wndw_mtrx->setElement( 2, 2, (-1.0*height)/2.0 );
    wndw_mtrx->setElement( 3, 3, 1 );
    wndw_mtrx->setElement( 4, 4, 1 );
    wndw_mtrx->setElement( 1, 4, (width-1.0)/2.0 );
    wndw_mtrx->setElement( 2, 4, (height-1.0)/2.0 );

    return wndw_mtrx;
}

/**
*    aspectRatio
*    Creates a matrix that holds the values for the aspect ratio.
*    Preconditions:
*        The width of the screen.
*        The height of the screen.
*    Postconditions:
*        Returns a pointer to a matrix.
*/
Matrix* AffineTransforms::aspectRatio( double width, double height )
{

    Matrix* aspc = new Matrix( 4, 4 );
    double a = height/width;

    aspc->setElement( 1, 1, 2.0/2.0 );
    aspc->setElement( 2, 2, 2.0/(2.0*a));
    aspc->setElement( 3, 3, 1 );
    aspc->setElement( 4, 4, 1 );

    return aspc;
}

/**
*    translate
*    Translate the object based on the values given by the user.
*    Preconditions:
*        Three doubles.
*    Postconditions:
*        Returns a matrix that is used in I matrix.
*/
Matrix* AffineTransforms::translate(double ax, double ay, double az)
{
    Matrix* trans = new Matrix(4, 4);
    trans->setElement(1,1,1);
    trans->setElement(1,2,0);
    trans->setElement(1,3,0);
    trans->setElement(1,4,ax);
    trans->setElement(2,1,0);
    trans->setElement(2,2,1);
    trans->setElement(2,3,0);
    trans->setElement(2,4,ay);
    trans->setElement(3,1,0);
    trans->setElement(3,2,0);
    trans->setElement(3,3,1);
    trans->setElement(3,4,az);
    trans->setElement(4,1,0);
    trans->setElement(4,2,0);
    trans->setElement(4,3,0);
    trans->setElement(4,4,1);

    return trans;
}

/**
*    rotateX
*    Rotates the object on the x axis.
*    Preconditions:
*        The degrees of rotation.
*    Postconditions:
*        Returns a matrix that holds the values used for rotation to be used
*        within I matrix.
*/
Matrix* AffineTransforms::rotateX( double degrees )
{
    double radians = degrees * RADIANS;
    Matrix* rx = new Matrix(4,4);
    
    rx->setElement(1,1,1);
    rx->setElement(1,2,0);
    rx->setElement(1,3,0);
    rx->setElement(1,4,0);
    rx->setElement(2,1,0);
    rx->setElement(2,2,cos(radians));
    rx->setElement(2,3,(-1.0*sin(radians)));
    rx->setElement(2,4,0);
    rx->setElement(3,1,0);
    rx->setElement(3,2,sin(radians));
    rx->setElement(3,3,cos(radians));
    rx->setElement(3,4,0);
    rx->setElement(4,1,0);
    rx->setElement(4,2,0);
    rx->setElement(4,3,0);
    rx->setElement(4,4,1);

    return rx;
}

/**
*    rotateY
*    Rotates the object on the Y axis.
*    Preconditions:
*        The degrees of rotation.
*    Postconditions:
*        Returns a matrix that holds the values used for rotation to be used
*        within I matrix.
*/
Matrix* AffineTransforms::rotateY( double degrees )
{
    double radians = degrees * RADIANS;
    Matrix* ry = new Matrix(4,4);

    ry->setElement(1,1,cos(radians));
    ry->setElement(1,2,0);
    ry->setElement(1,3,sin(radians));
    ry->setElement(1,4,0);
    ry->setElement(2,1,0);
    ry->setElement(2,2,1);
    ry->setElement(2,3,0);
    ry->setElement(2,4,0);
    ry->setElement(3,1,(-1.0*sin(radians)));
    ry->setElement(3,2,0);
    ry->setElement(3,3,cos(radians));
    ry->setElement(3,4,0);
    ry->setElement(4,1,0);
    ry->setElement(4,2,0);
    ry->setElement(4,3,0);
    ry->setElement(4,4,1);

    return ry;
}

/**
*    rotateZ
*    Rotates the object on the Z axis.
*    Preconditions:
*        The degrees of rotation.
*    Postconditions:
*        Returns a matrix that holds the values used for rotation to be used
*        within I matrix.
*/
Matrix* AffineTransforms::rotateZ( double degrees )
{
    double radians = degrees * RADIANS;
    Matrix* rz = new Matrix(4,4);
    
    rz->setElement(1,1,cos(radians));
    rz->setElement(1,2,(-1.0*sin(radians)));
    rz->setElement(1,3,0);
    rz->setElement(1,4,0);
    rz->setElement(2,1,sin(radians));
    rz->setElement(2,2,cos(radians));
    rz->setElement(2,3,0);
    rz->setElement(2,4,0);
    rz->setElement(3,1,0);
    rz->setElement(3,2,0);
    rz->setElement(3,3,1);
    rz->setElement(3,4,0);
    rz->setElement(4,1,0);
    rz->setElement(4,2,0);
    rz->setElement(4,3,0);
    rz->setElement(4,4,1);

    return rz;
}
