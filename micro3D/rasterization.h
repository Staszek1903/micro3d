#ifndef RASTERIZATION_H
#define RASTERIZATION_H

#include <inttypes.h>
#include "point3f.h"
#include <algorithm>

#define CONDITIONAL_SWAP(x,y,cond) { \
    x = x + cond*y; \
    y = y - cond*2*y + cond*x; \
    x = x - cond*y;\
    }

#define GET_BIT(var,bit) ((var>>bit)&1)
#define SET_BIT(var,bit,val) {var|=((val&1)<<bit);}
#define CLR_BIT(var,bit) {var&=~(1<<bit);}

typedef struct { void * arg; void (*putPixel)(unsigned int x, unsigned int y, float z, void * arg); } RasterFunc;

typedef struct { int x; int y; } Point;
typedef struct { Point a,b; int dx, dy, x, y, D; char flags;} BresenhamState;
typedef struct { Point p[3]; } Triangle;

void bresenham_init(Point3 p1, Point3 p2, BresenhamState * state);
Point bresenham_next(BresenhamState * state);

void line(Point a, Point b, RasterFunc func);
void strokeTriangle(Triangle t, RasterFunc func);
void fillTriangle(Triangle t, RasterFunc func);

void fillTriangle3D(Point3 a, Point3 b, Point3 c, RasterFunc func);

#endif // RASTERIZATION_H
