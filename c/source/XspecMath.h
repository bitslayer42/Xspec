/********************************************/
/*XspecMath.h                               */
/********************************************/




/**********************************************/
/*given 3D point, returns the theta and phi of the view from the global 'eye' */

extern angles findAngles(point3d point);

/********************************************/
/* given theta and phi, the view angles, return points to be plotted */

extern point2d findScreenPoints(angles anglePair);

/*******************************************/
