#include "transformation.h"

#include "IOStuff/console.h"


Matrix4 multMatrix(const Matrix4 *m1, const Matrix4 *m2)
{
    Matrix4 result;

    for(int i=0; i<4; ++i)
        for(int j=0; j<4; ++j)
            result.m[i][j] =    m1->m[i][0] * m2->m[0][j] +
                                m1->m[i][1] * m2->m[1][j] +
                                m1->m[i][2] * m2->m[2][j] +
                                m1->m[i][3] * m2->m[3][j];

    return result;
}


Point3 transformPoint(Point3 p, const Matrix4 *m)
{
    Point3 result;
    float w =1;
    for(int i=0 ;i < 3; ++i)
        result.p[i] =   p.p[0] * m->m[0][i] +
                        p.p[1] * m->m[1][i] +
                        p.p[2] * m->m[2][i] +
                        m->m[3][i];

    w = p.p[0] * m->m[0][3] +
        p.p[1] * m->m[1][3] +
        p.p[2] * m->m[2][3] +
        m->m[3][3];

    if(w <= 0.01f) w = 0.01f;       // kijowy kod    ale dzieki temu nie ma glitchy jak objekt przechodzi za kamere
                                    // tylko dla tego żeby nie dodawać w do Point3d bo jest redundantna
    for(int i=0 ;i < 3; ++i)
        result.p[i] /= w;

    return result;
}

void projectionMatrix(Matrix4 * m, float asect_ratio, float fov, float znear, float zfar)
{       // sprawdzić referencja vs kopia w tablicah w struktutrach
    *m = {0};
    float scale = 1.0f/tan(fov*0.5f*(float)M_PI /180);
    float q = zfar / (zfar - znear);

    *m = {{{asect_ratio*scale   ,0      ,0          ,0},
           {0                   ,scale  ,0          ,0},
           {0                   ,0      ,q          ,1},
           {0                   ,0      ,-q*znear   ,0}}};
}

void printMatix(Matrix4 m)
{
    for(int i=0; i<4; ++i){
        for(int j=0; j<4; ++j)
            Console::get()<<m.m[i][j]<<" ";
        Console::get()<<"\n";
    }
}

void zeroMatrix(Matrix4 *m)
{
    for(int i=0; i<4; ++i)
        for(int j=0; j<4; ++j)
            m->m[i][j] = 0.0f;
}

void identityMatrix(Matrix4 *m)
{
    zeroMatrix(m);
    for(int i=0; i<4; ++i)
        m->m[i][i] = 1.0f;
}

void rotationXMatrix(Matrix4 *m, float angle)
{
    *m = {{{    1,          0,          0,          0               },
           {    0,          cos(angle), sin(angle), 0               },
           {    0,          -sin(angle),cos(angle), 0               },
           {    0,          0,          0,          1               }}};

}

void rotationYMatrix(Matrix4 *m, float angle)
{
    *m = {{{    cos(angle), 0,          -sin(angle),0               },
           {    0,          1,          0,          0               },
           {    sin(angle), 0,          cos(angle), 0               },
           {    0,          0,          0,          1               }}};
}

void rotationZMatrix(Matrix4 *m, float angle)
{
    *m = {{{    cos(angle), sin(angle), 0,          0               },
           {    -sin(angle),cos(angle), 0,          0               },
           {    0,          0,          1,          0               },
           {    0,          0,          0,          1               }}};
}

void rotationMatrix(Matrix4 *m, Point3 axis, float angle)
{
    axis = normalize(axis);

    m->m[0][0] = axis.p[0] * axis.p[0] * (1 - cos(angle)) + cos(angle);
    m->m[0][1] = axis.p[1] * axis.p[0] * (1 - cos(angle)) - axis.p[2] * sin(angle);
    m->m[0][2] = axis.p[2] * axis.p[0] * (1 - cos(angle)) + axis.p[1] * sin(angle);
    m->m[0][3] = 0;

    m->m[1][0] = axis.p[0] * axis.p[1] * (1 - cos(angle)) + axis.p[2] * sin(angle);
    m->m[1][1] = axis.p[1] * axis.p[1] * (1 - cos(angle)) + cos(angle);
    m->m[1][2] = axis.p[2] * axis.p[1] * (1 - cos(angle)) - axis.p[0] * sin(angle);
    m->m[1][3] = 0;

    m->m[2][0] = axis.p[0] * axis.p[2] * (1 - cos(angle)) - axis.p[1] * sin(angle);
    m->m[2][1] = axis.p[1] * axis.p[2] * (1 - cos(angle)) + axis.p[0] * sin(angle);
    m->m[2][2] = axis.p[2] * axis.p[2] * (1 - cos(angle)) + cos(angle);
    m->m[2][3] = 0;

    m->m[3][0] = 0;
    m->m[3][1] = 0;
    m->m[3][2] = 0;
    m->m[3][3] = 1;
}

void pointAtMatrix(Matrix4 *m, Point3 pos, Point3 target, Point3 up_vect)
{
    Point3 forward = normalize(sub(target,pos));
    Point3 a = mult(forward, dot(up_vect, forward));
    Point3 up = normalize(sub(up_vect, a));
    Point3 right = cross(up,forward);

    *m = {{{    right.p[0],  right.p[1],  right.p[2],  0               },
           {    up.p[0],     up.p[1],     up.p[2],     0               },
           {    forward.p[0],forward.p[1],forward.p[2],0               },
           {    pos.p[0],    pos.p[1],    pos.p[2],    1               }}};
}

Matrix4 inverseTransform(Matrix4 * m){
    return {{{m->m[0][0], m->m[1][0],m->m[2][0], 0.0f},
             {m->m[0][1],m->m[1][1],m->m[2][1], 0.0f},
             {m->m[0][2],m->m[1][2],m->m[2][2], 0.0f},
             {-(m->m[3][0] * m->m[0][0] + m->m[3][1] * m->m[0][1] + m->m[3][2] * m->m[0][2]),
              -(m->m[3][0] * m->m[1][0] + m->m[3][1] * m->m[1][1] + m->m[3][2] * m->m[1][2]),
              -(m->m[3][0] * m->m[2][0] + m->m[3][1] * m->m[2][1] + m->m[3][2] * m->m[2][2]),
              1.0f}}};
}

void translationMatrix(Matrix4 *m, Point3 vect)
{
    identityMatrix(m);
    for(int i=0; i<3; ++i)
        m->m[3][i] = vect.p[i];
}

Point3 getTriangleNormal(Point3 a, Point3 b, Point3 c)
{
    Point3 side_a = sub(b, a);
    Point3 side_b = sub(c, a);
    return normalize(cross(side_a,side_b));
}


Point3 toScreenSpace(Point3 p, float w, float h)
{
    return {0.5f*(p.p[0]*w + w),
            0.5f*(p.p[1]*h + h),
            0.5f*(p.p[2]*w + w)};
}
