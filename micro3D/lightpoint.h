#ifndef LIGHTPOINT_H
#define LIGHTPOINT_H

#include "point3f.h"
#include "mesh.h"

struct LightPoint
{
    Point3 position = {0,0,0};
    Color color = {255,255,255};
    float distance= 10.0f;
};

#endif // LIGHTPOINT_H
