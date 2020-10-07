#include "vertexjob.h"

VertexJob::VertexJob()
    :triangle({0}), mvp({0})
{

}

VertexJob::VertexJob(const VertexJob & other)
    :mvp(other.mvp)
{
    triangle[0] = other.triangle[0];
    triangle[1] = other.triangle[1];
    triangle[2] = other.triangle[2];

    colors[0] = other.colors[0];
    colors[1] = other.colors[1];
    colors[2] = other.colors[2];
}

VertexJob::VertexJob(const Point3 *tri, Matrix4 &mvp)
    :mvp(mvp)
{
    triangle[0] = tri[0];
    triangle[1] = tri[1];
    triangle[2] = tri[2];
}
