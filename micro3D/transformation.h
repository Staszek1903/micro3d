#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <math.h>
#include "point3f.h"

typedef struct { float m[4][4]; } Matrix4;

void zeroMatrix(Matrix4 * m);
void identityMatrix(Matrix4 *m);
Matrix4 multMatrix(const Matrix4 * m1, const Matrix4 * m2);
Point3 transformPoint(Point3 p, const Matrix4 * m);

/**
 * @brief projectionMatrix
 * @param m             destination matrix
 * @param asect_ratio   screen_w/sreen_h
 * @param fov           field of view in degreen (0-360)
 * @param znear         near clipping plane
 * @param zfar          far clipping plane
 */
void projectionMatrix(Matrix4 * m, float asect_ratio, float fov, float znear, float zfar);

void rotationXMatrix(Matrix4 *m, float angle);
void rotationYMatrix(Matrix4 *m, float angle);
void rotationZMatrix(Matrix4 *m, float angle);

/**
 * @brief rotationMatrix
 * @param m     destination matrix
 * @param axis  axis normal vector
 * @param angle angle in radians
 */
void rotationMatrix(Matrix4 *m, Point3 axis, float angle);

void pointAtMatrix(Matrix4 * m, Point3 pos, Point3 target, Point3 up_vect);
void translationMatrix(Matrix4 *m, Point3 vect);

Matrix4 inverseTransform(Matrix4 * m);
Point3 getTriangleNormal(Point3 a, Point3 b, Point3 c);
Point3 toScreenSpace(Point3 p, float w, float h);

//DEBUG
void printMatix(Matrix4 m);

#endif // TRANSFORMATION_H
