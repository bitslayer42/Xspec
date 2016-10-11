/******************************************/
/*XspecMath.c                              */
/*******************************************/

#include <stdio.h>
#include <math.h>
#include "XspecConv.h"
/****************************************/
extern double width;    /*these are the globals that can be changed */     
extern point3d eye;   /*from the control window */
extern point3d view;
extern double widthD;  /*widthD is in degrees, width is in radians */

extern vector upV;
extern line  *lines;
extern point3d *verts3d;
extern point2d *screenVerts;
extern angles *viewAngles;

extern constantViewAngle;
extern int numlines;
extern int numpoints;
extern const int WINDOWSIZE;

/****************************************/
/*returns the dot product of two vectors  */

double dotP(vector A, vector B)
{
 double result;                      
 result = A.x*B.x + A.y*B.y + A.z*B.z;
 return result;
}

/****************************************/
/*returns the cross product of two vectors  */

vector crossP(vector A, vector B)
{
 vector result;                   
 result.x = A.y*B.z - A.z*B.y;
 result.y = A.z*B.x - A.x*B.z;
 result.z = A.x*B.y - A.y*B.x;
 return result;
}

/*******************************************/
/*returns the angle between two vectors- the inverse cos of */
/*their dot product over the product of their magnitudes */

double angleBetweenVectors (vector A, vector B)
{
 double dotprod, magA, magB;
 double prod, cosVal;
 dotprod = dotP(A,B);           
 magA = sqrt((A.x*A.x) + (A.y*A.y) + (A.z*A.z));
 magB = sqrt((B.x*B.x) + (B.y*B.y) + (B.z*B.z));
 prod = magA*magB;
 if (prod == 0) prod = 0.001;
 cosVal = (dotprod/(prod));
 if (cosVal >= 1)
   {
    return 0;
  }
 else if (cosVal <= -1)
   {
    return 3.1415;
  }
 else
   {    
    return acos (cosVal);
  }
}

/********************************************/
/*boolean function, returns true if the tip of p is on the opposite side  */
/*of the plane (to which v is a normal) from the tip of v  */
 
int oppositeSide(vector p, vector v)
{
 double t, denominator;
 denominator = v.x*v.x + v.y*v.y + v.z*v.z;                   
 if (denominator == 0) denominator = 0.001;
 t = (v.x*(-p.x) + v.y*(-p.y) + v.z*(-p.z))
    /denominator;
 return (t<0);       /*if t<0 then the point is on the opposite side*/
}

/********************************************/
/*given 3D point, returns the theta and phi of the view from the global 'eye' */

angles findAngles(point3d point)
{
 angles result;
 point3d right;
 vector viewPlaneV, pointPlaneV;
 vector viewV, pointV;
                         
if(constantViewAngle)
  {
 viewV.x = (view.x);
 viewV.y = (view.y);
 viewV.z = (view.z);
  }
else
  {      
 viewV.x = (view.x-eye.x);
 viewV.y = (view.y-eye.y);
 viewV.z = (view.z-eye.z);
  }
 /*set up default views if view=eye or if view straight up*/
 if (viewV.x == 0 && viewV.y == 0)
   {
    if (viewV.y == 0)
      {
	viewV.y = 1;
      }
     upV.x = 1;
   }
 else upV.x = 0;
 pointV.x = (point.x-eye.x);
 pointV.y = (point.y-eye.y);
 pointV.z = (point.z-eye.z);
 
 result.phi = angleBetweenVectors(viewV,pointV);

 if(result.phi <= 0.0001 || result.phi >= 3.1415)
   {
     result.theta = 0;
   }
 else
   {
     viewPlaneV = crossP(viewV, upV);
     pointPlaneV = crossP(viewV, pointV);
     result.theta = angleBetweenVectors(viewPlaneV, pointPlaneV);   
     if(oppositeSide(pointV, viewPlaneV))
      {
        result.theta = -result.theta;
      }
   }                                                        
 return result;   
}

/********************************************/
/* given theta and phi, the view angles, return points to be plotted */

point2d findScreenPoints(angles anglePair)
{
 point2d result;
 result.x = (int)floor(WINDOWSIZE/width * anglePair.phi * sin(anglePair.theta) + WINDOWSIZE/2);
 result.y = (int)floor(-WINDOWSIZE/width * anglePair.phi * cos(anglePair.theta) + WINDOWSIZE/2);
 return result; 
} 

/*******************************************/













