#include "mesh.h"
#include "IOStuff/console.h"

void loadMeshFromObjFile(const char *dir, Mesh *mesh)
{
    *mesh = {0};
    FILE * file = fopen(dir,"r");
    if(!file) throw std::runtime_error("file"+std::string(dir)+" not found");


    fpos_t file_begining;
    fgetpos(file, &file_begining);

    char buff[100] = {0};
    float x,y,z;
    int a,b,c;

    while(true){
        fgets(buff,100,file);
        if(feof(file)) break;

        if(buff[0] == 'v') mesh->vertex_count++;
        if(buff[0] == 'f') mesh->triangle_count++;
    }

    mesh->vertices = (Point3*)          malloc(sizeof (Point3)      * mesh->vertex_count);
    mesh->triangles = (TriangleInd*)    malloc(sizeof (TriangleInd) * mesh->triangle_count);
    fsetpos(file,&file_begining);

    size_t v_index = 0;
    size_t f_index = 0;
    while(true){
        fgets(buff,100,file);
        if(feof(file)) break;

        if(buff[0] == 'v'){
            sscanf(buff,"v %f %f %f", &x,&y,&z);
            mesh->vertices[v_index] = {x,y,z};
            v_index++;
        }
        if(buff[0] == 'f'){
            sscanf(buff,"f %i %i %i", &a,&b,&c);
            mesh->triangles[f_index] = {{a-1,b-1,c-1}};
            f_index++;
        }
    }

    fclose(file);
}

void deleteMesh(Mesh *mesh)
{
    mesh->vertex_count = 0;
    mesh->triangle_count = 0;
    if(mesh->vertices){
        free(mesh->vertices);
        mesh->vertices = NULL;
    }
    if(mesh->triangles){
        free(mesh->triangles);
        mesh->triangles = NULL;
    }
}

Color colorAverage(Color * array, int * indices, int size){
    float r=0,g=0,b=0;
    Color average = {0,0,0};

    for(int i=0; i<size; ++i){
        r += array[indices[i]].r;
        g += array[indices[i]].g;
        b += array[indices[i]].b;
    }
    average.r = r/size;
    average.g = g/size;
    average.b = b/size;

    return average;
}

void loadMeshFromPlyFile(const char *dir, Mesh *mesh, ColorInfo *c_info)
{
    *mesh = {0};
    FILE * file = fopen(dir,"r");
    if(!file) throw std::runtime_error("file"+std::string(dir)+" not found");

    char buff[100] = {0};
    float x,y,z;
    unsigned char r,g,bl;
    size_t face_count = 0;

    while(strcmp(buff, "end_header\n")){
        fgets(buff,100,file);
        if(feof(file)) break;
        sscanf(buff,"element vertex %ld", &(mesh->vertex_count));
        sscanf(buff,"element face %ld", &face_count);
    }

    if(mesh->vertex_count == 0 || face_count == 0)
        throw std::runtime_error("in file" + std::string(dir) +" could not find vertex size and triangle size info");

    mesh->vertices = (Point3*)          malloc(sizeof (Point3)      * mesh->vertex_count);
    Color * temp_colors = (Color*)           malloc(sizeof (Color)  * mesh->vertex_count);

    for(size_t i=0; i<mesh->vertex_count; ++i){
        fgets(buff,100,file);
        if(feof(file)) throw std::runtime_error("in file" + std::string(dir) + "unexpecte EOF");
        sscanf(buff, "%f %f %f %hhu %hhu %hhu", &x,&y,&z,&r,&g,&bl);
        mesh->vertices[i] = {{x,y,z}};
        temp_colors[i] = {r,g,bl};
    }

    int * temp_indices = (int*) malloc(sizeof (int[10]) * face_count);

    mesh->triangle_count = face_count;
    for(size_t i=0; i<face_count; ++i){
        int count;
        fscanf(file, "%d", &count);
        temp_indices[i*10] = count;
        if(feof(file)) throw std::runtime_error("in file" + std::string(dir) + "unexpecte EOF");
        for(int j=0; j<count; ++j){
            fscanf(file, "%d", &temp_indices[i*10 + j+1]);
        }

        mesh->triangle_count += (count-3);
    }

    c_info->size = mesh->triangle_count;
    mesh->triangles = (TriangleInd*) malloc(sizeof (TriangleInd)*mesh->triangle_count);
    c_info->colors = (Color*) malloc(sizeof (Color) * c_info->size);

    //TRIANGULARIZATION
    int triangle_i = 0;
    for(size_t i=0; i<face_count; ++i){
        mesh->triangles[triangle_i++] = {{temp_indices[i*10+1], temp_indices[i*10+2], temp_indices[i*10+3]}};
        int olda = temp_indices[i*10+1];
        int oldb = temp_indices[i*10+3];
        for(int j=3; j<temp_indices[i*10]; ++j){
            mesh->triangles[triangle_i++] = {{olda,oldb,temp_indices[i*10+(j+1)]}};
            oldb = temp_indices[i*10+(j+1)];
        }
    }

    for(size_t i=0; i<mesh->triangle_count; ++i){
        Color avr = colorAverage(temp_colors,mesh->triangles[i].i,3);
        c_info->colors[i] = avr;
    }

    free(temp_colors);
    free(temp_indices);
}

void deleteColorInfo(ColorInfo *info)
{
    info->size = 0;
    if(info->colors){
        free(info->colors);
        info->colors = NULL;
    }
}
