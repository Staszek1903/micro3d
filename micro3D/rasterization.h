#ifndef RASTERIZATION_H
#define RASTERIZATION_H

#include <inttypes.h>
#include "point3f.h"
//#include "color.h"
#include <algorithm>

#define CONDITIONAL_SWAP(x,y,cond) { \
    x = x + cond*y; \
    y = y - cond*2*y + cond*x; \
    x = x - cond*y;\
    }

#define GET_BIT(var,bit) ((var>>bit)&1)
#define SET_BIT(var,bit,val) {var|=((val&1)<<bit);}
#define CLR_BIT(var,bit) {var&=~(1<<bit);}

typedef void (*PixelFunc)(unsigned int x,
                 unsigned int y,
                 void * arg);

typedef struct { void * arg;
                 PixelFunc putPixel; } RasterFunc;

typedef struct { int x; int y; } Point;
typedef struct { Point a,b; int dx, dy, x, y, D; char flags;} BresenhamState;
typedef struct { Point p[3]; } Triangle;

void bresenham_init(Point3 p1, Point3 p2, BresenhamState * state);
Point bresenham_next(BresenhamState * state);

/**
 * @brief line - strokes line
 */
void line(Point a, Point b, RasterFunc func);
void strokeTriangle(Triangle t, RasterFunc func);

/**
 * @brief get_barycentric_coords - calculates barycetric coordinates of a point in a triangle
 * @param x point x coordinate
 * @param y point y coordinate
 * @param a vertex1
 * @param b vertex2
 * @param c vertex3
 * @return barycentric coordinates
 */
Point3 get_barycentric_coords(float x, float y, Point3 a, Point3 b, Point3 c);

/**
 * @brief get_z -  calculate a z component of a point using its barycentric coordinates
 */
float get_z(Point3 a, Point3 b, Point3 c, Point3 barocentric);

/**
 * @brief get_bary_point - calculate a position of a point using its barycentrice coordinates
 */
Point3 get_bary_point(Point3 a, Point3 b, Point3 c, Point3 bary);

/**
 * @brief fillTriangleBres - fills triangle using bresenham algorithms
 */
void fillTriangleBres(Point3 a, Point3 b, Point3 c, RasterFunc func);

/**
 * @brief fillTriangle3D - fills triangle using linear interpolation
 */
void fillTriangle3D(Point3 a, Point3 b, Point3 c, RasterFunc func);

#endif // RASTERIZATION_H
