#include "model.h"

void m3d::Model::update_matrices()
{
    if(obsolete_transformation){
        Matrix4 rx,ry,rz;
        Matrix4 translation, scaling;
        rotationXMatrix(&rx,rot.p[0]);
        rotationYMatrix(&ry,rot.p[1]);
        rotationZMatrix(&rz,rot.p[2]);
        translationMatrix(&translation, pos);

        scaleMatrix(&scaling,scale);

        rotation_matrix = multMatrix(&rx, &ry);
        rotation_matrix = multMatrix(&rotation_matrix, &rz);
        transformation_matrix = multMatrix(&rotation_matrix, &translation);
        transformation_matrix = multMatrix(&scaling, &transformation_matrix);

        obsolete_transformation = false;
    }
}

m3d::Model::Model(Mesh &mesh, ColorInfo *c_info)
    :mesh(mesh), colors(c_info),pos({0}),rot({0})
{
    identityMatrix(&rotation_matrix);
    identityMatrix(&transformation_matrix);
}

Mesh & m3d::Model::getMesh() const
{
    return mesh;
}

ColorInfo & m3d::Model::getColorInfo() const
{
    return *colors;
}

Point3 m3d::Model::getPos() const
{
    return pos;
}

void m3d::Model::setPos(const Point3 &value)
{
    obsolete_transformation = true;
    pos = value;
}

Point3 m3d::Model::getRot() const
{
    return rot;
}

void m3d::Model::setRot(const Point3 &value)
{
    obsolete_transformation = true;
    rot = value;
}

Point3 m3d::Model::getScale() const
{
    return scale;
}

void m3d::Model::setScale(const Point3 &value)
{
    obsolete_transformation = true;
    scale = value;
}

void m3d::Model::move(Point3 move)
{
    obsolete_transformation = true;
    pos = add(pos, move);
}

void m3d::Model::rotate(Point3 rot)
{
    obsolete_transformation = true;
    this->rot = add(this->rot, rot);
}

Matrix4 & m3d::Model::getTransformation()
{
    update_matrices();
    return transformation_matrix;
}

Matrix4 &m3d::Model::getRotation()
{
    update_matrices();
    return rotation_matrix;
}

Color m3d::Model::getColor() const
{
    return color;
}

void m3d::Model::setColor(const Color &value, float alpha)
{
    color = value;
    this->alpha = alpha;
}


