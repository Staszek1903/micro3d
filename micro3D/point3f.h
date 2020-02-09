#ifndef POINT3F_H
#define POINT3F_H

#include <assert.h>
#include <math.h>

typedef struct { float p[3] = {0,0,0}; } Point3;

Point3 add(Point3 a, Point3 b);
Point3 sub(Point3 a, Point3 b);
Point3 mult(Point3 a, float s);
Point3 div(Point3 a, float s);
Point3 neg(Point3 a);
Point3 cross(Point3 a, Point3 b);
float dot(Point3 a, Point3 b);
float lensq(Point3 a);
float len(Point3 a);
Point3 normalize(Point3 a);

#endif // POINT3F_H
