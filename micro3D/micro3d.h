#ifndef MICRO3D_H
#define MICRO3D_H

#include "point3f.h"
#include "mesh.h"
#include "transformation.h"
#include "IOStuff/console.h"

void calculateNormals(TriangleInd * tris, size_t tris_s, Point3 * verts, Point3 * normals);
Point3 linePlaneIntersection(Point3 plane_point, Point3 plane_normal, Point3 lineBegin, Point3 lineEnd);
int planeClipping(Point3 triangle[3], Point3 plane_point, Point3 plane_normal, Point3 additional[]);
int clipToScreen(Point3 triangle[3], float near, float far, Point3 r[][3]);
Point3 pointToScreen(Point3 a, float s_width, float s_height);

#endif // MICRO3D_H
