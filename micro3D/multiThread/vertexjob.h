#ifndef VERTEXJOB_H
#define VERTEXJOB_H

#include "../point3f.h"
#include "../transformation.h"
#include "../shaders.h"

class VertexJob
{
public:
    Point3 triangle[3];
    Matrix4 model_matrix;
    Matrix4 model_rotation;
    Matrix4 mvp;
    Color colors [3] {200,100,200};


    VertexJob();
    VertexJob(const VertexJob &);
    VertexJob(const Point3 * tri,Matrix4& mvp);
};

#endif // VERTEXJOB_H
