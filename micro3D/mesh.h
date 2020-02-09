#ifndef MODEL_H
#define MODEL_H

#include "transformation.h"
#include <stdio.h>
#include <memory.h>
#include <inttypes.h>

typedef struct { int i[3]; } TriangleInd;
typedef struct { size_t vertex_count, triangle_count; Point3 * vertices; TriangleInd * triangles; } Mesh;
typedef struct { uint8_t r; uint8_t g; uint8_t b;} Color;
typedef struct { size_t size; Color * colors;} ColorInfo;

void loadMeshFromObjFile(const char * dir, Mesh * mesh);
void loadMeshFromPlyFile(const char * dir, Mesh * mesh, ColorInfo * c_info);
void deleteMesh(Mesh * mesh);
void deleteColorInfo(ColorInfo * info);

#endif // MODEL_H
