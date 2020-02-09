#include "micro3d.h"

void calculateNormals(TriangleInd *tris, size_t tris_s, Point3 *verts, Point3 *normals)
{
    for(size_t i=0; i<tris_s; ++i){
        TriangleInd ind = tris[i];
        Point3 a = verts[ind.i[0]],
               b = verts[ind.i[1]],
               c = verts[ind.i[2]];
        normals[i] = getTriangleNormal(a,b,c);
    }
}

Point3 linePlaneIntersection(Point3 plane_point, Point3 plane_normal, Point3 lineBegin, Point3 lineEnd)
{
    plane_normal = normalize(plane_normal);
    float plane_distance = dot(plane_normal, plane_point); // plane to origin distance
    float begin_distance = dot(lineBegin,plane_normal);
    float   end_distance = dot(lineEnd, plane_normal);

    float fraction = (plane_distance - begin_distance) / (end_distance - begin_distance);
    Point3 line_vector = sub(lineEnd, lineBegin);
    return add(lineBegin, mult(line_vector, fraction));
}



int planeClipping(Point3 triangle[3], Point3 plane_point, Point3 plane_normal, Point3 additional[3])
{
    plane_normal = normalize(plane_normal);

    int inside_s = 0, outside_s = 0;
    Point3 inside[3], outside[3];
    //Point3 tp[3];
    for(int i=0; i<3; ++i){
        float dist = dot(plane_normal, sub(triangle[i], plane_point));
        if(dist >= 0)
            inside[inside_s++] = triangle[i];
        else
            outside[outside_s++] = triangle[i];
    }

    if(inside_s == 3){
        return 1;
    }else if(inside_s == 0){
        return 0;
    }else if (inside_s == 1){
        Point3 newa = linePlaneIntersection(plane_point,plane_normal,inside[0],outside[0]);
        Point3 newb = linePlaneIntersection(plane_point,plane_normal,inside[0],outside[1]);
        triangle[0] = inside[0];
        triangle[1] = newa;
        triangle[2] = newb;
        return 1;
    }else if (inside_s == 2){
        Point3 newa = linePlaneIntersection(plane_point,plane_normal,outside[0],inside[0]);
        Point3 newb = linePlaneIntersection(plane_point,plane_normal,outside[0],inside[1]);

        triangle[0] = inside[0];
        triangle[1] = inside[1];
        triangle[2] = newa;

        additional[0] = inside[1];
        additional[1] = newa;
        additional[2] = newb;

        return 2;
    }

    return 0;
}

#define PLANES_NUM 6
int clipToScreen(Point3 triangle[3], float near, float far, Point3 r[][3])
{
    Point3 plane_point[6] = { {-1,0,0}, {1,0,0}, {0,-1,0}, {0,1,0}, {0,0, near}, {0,0, far} };
    Point3 plane_normal[6] = { {1,0,0}, {-1,0,0}, {0,1,0}, {0,-1,0}, {0,0,1}, {0,0,-1}};
    Point3 temp_tri[3];

    int source_size = 1;

    Point3 buffB [64][3];
    Point3 * temp = NULL;
    Point3 * sourceBuff = &r[0][0];
    Point3 * destinBuff = &buffB[0][0];

    sourceBuff[0] = triangle[0];
    sourceBuff[1] = triangle[1];
    sourceBuff[2] = triangle[2];


    for(int i = 0; i < PLANES_NUM; ++i){
        int k = 0;
        for(int j = 0; j < source_size; ++j) {
            int current_result = planeClipping(&sourceBuff[j*3], plane_point[i], plane_normal[i], temp_tri);
            destinBuff[k*3+0] = sourceBuff[j*3+0];
            destinBuff[k*3+1] = sourceBuff[j*3+1];
            destinBuff[k*3+2] = sourceBuff[j*3+2];
            destinBuff[k*3+3] = temp_tri[0];
            destinBuff[k*3+4] = temp_tri[1];
            destinBuff[k*3+5] = temp_tri[2];
            k+=current_result;
        }
        temp = destinBuff;
        destinBuff = sourceBuff;
        sourceBuff = temp;
        source_size = k;
    }

    if(PLANES_NUM == 0) source_size =1;
    if(PLANES_NUM %2 == 1){
        memcpy(r,buffB,sizeof(Point3) * 64*3);
    }
    return source_size;
}

Point3 pointToScreen(Point3 a, float s_width, float s_height)
{
    return {a.p[0] * s_width * 0.5f + s_width * 0.5f,
    a.p[1] * s_height * 0.5f + s_height * 0.5f,
    a.p[2]};
}
