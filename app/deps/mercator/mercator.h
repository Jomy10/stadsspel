#ifndef _MERCATOR_H
#define _MERCATOR_H

#include <math.h>
#define DEG2RAD(a)   ((a) / (180 / M_PI))
#define RAD2DEG(a)   ((a) * (180 / M_PI))
#define EARTH_RADIUS 6378137

/* The following functions take their parameter and return their result in degrees */

double y2lat_d(double y)   ;
double x2lon_d(double x)   ;

double lat2y_d(double lat) ;
double lon2x_d(double lon) ;

/* The following functions take their parameter in something close to meters, along the equator, and return their result in degrees */

double y2lat_m(double y)   ;
double x2lon_m(double x)   ;

/* The following functions take their parameter in degrees, and return their result in something close to meters, along the equator */

double lat2y_m(double lat) ;
double lon2x_m(double lon) ;
#endif
