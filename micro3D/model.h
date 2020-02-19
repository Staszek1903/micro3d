#ifndef MODELAPI_H
#define MODELAPI_H

#include "mesh.h"
#include "transformation.h"

namespace m3d {

class Model
{
    Mesh & mesh;
    ColorInfo * colors;

    Point3 pos, rot, scale = {1,1,1};

    bool obsolete_transformation = true;
    Matrix4 transformation_matrix;
    Matrix4 rotation_matrix;

    void update_matrices();

public:

    Color color = {0,0,0};
    float alpha = 0;


    Model(Mesh & mesh, ColorInfo * c_info = nullptr);
    Point3 getPos() const;
    void setPos(const Point3 &value);
    Point3 getRot() const;
    void setRot(const Point3 &value);
    Point3 getScale() const;
    void setScale(const Point3 &value);

    void move(Point3 move);
    void rotate(Point3 rot);

    Matrix4 & getTransformation();
    Matrix4 & getRotation();

    Mesh &getMesh() const;
    ColorInfo & getColorInfo() const;
    Color getColor() const;
    void setColor(const Color &value, float alpha);
};

}

#endif // MODELAPI_H
