#ifndef MODEL_H
#define MODEL_H

#include "transformation.h"
#include <stdio.h>
#include <memory.h>
#include <inttypes.h>
#include "color.h"

typedef struct { int i[3]; } TriangleInd;
typedef struct { size_t vertex_count, triangle_count; Point3 * vertices; TriangleInd * triangles; } Mesh;

void loadMeshFromObjFile(const char * dir, Mesh * mesh);
void loadMeshFromPlyFile(const char * dir, Mesh * mesh, ColorInfo * c_info);
void deleteMesh(Mesh * mesh);
void deleteColorInfo(ColorInfo * info);

#endif // MODEL_H
