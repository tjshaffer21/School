#include "AffineTransforms.h"
#include <math.h>
#include <iostream>
using namespace std;

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

