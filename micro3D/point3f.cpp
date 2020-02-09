#include "point3f.h"

Point3 add(Point3 a, Point3 b)
{
    return {a.p[0] + b.p[0] , a.p[1] + b.p[1] , a.p[2] + b.p[2]};
}

Point3 sub(Point3 a, Point3 b)
{
    return {a.p[0] - b.p[0] , a.p[1] - b.p[1] , a.p[2] - b.p[2]};
}

Point3 mult(Point3 a, float s)
{
    return {a.p[0] * s , a.p[1] * s , a.p[2] * s};
}

Point3 div(Point3 a, float s)
{
    assert(s != 0);
    //if(s == 0) return a;
    return {a.p[0] / s , a.p[1] / s , a.p[2] / s};
}

Point3 neg(Point3 a)
{
    return {-a.p[0],-a.p[1],-a.p[2]};
}

Point3 cross(Point3 a, Point3 b)
{
    return {a.p[1] * b.p[2] - a.p[2] * b.p[1],
            a.p[2] * b.p[0] - a.p[0] * b.p[2],
            a.p[0] * b.p[1] - a.p[1] * b.p[0]};
}

float dot(Point3 a, Point3 b)
{
    return (a.p[0] * b.p[0] + a.p[1] * b.p[1] + a.p[2] * b.p[2]);
}

float lensq(Point3 a)
{
    return (a.p[0] * a.p[0] + a.p[1] * a.p[1] + a.p[2] * a.p[2]);
}

float len(Point3 a)
{
    return sqrt(lensq(a));
}

Point3 normalize(Point3 a)
{
    return div(a,len(a));
}


