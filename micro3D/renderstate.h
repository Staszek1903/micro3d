#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include <SFML/Graphics.hpp>
#include "transformation.h"
#include "lightpoint.h"

namespace m3d {

class RenderState
{
    sf::Vector2u contextSize = {256,256};
    float fov = 90.0f;
    float near_clip = 1.0f;
    float far_clip = 30.0f;

    bool obsolete_projection = true;
    Matrix4 projection_matix;

    Point3 cam_pos = {0}, cam_rot = {0};

    bool obsolete_view = true;
    Matrix4 view_matrix;
    Point3 forward_vect, right_vect;

    LightPoint lightpoint;
    float ambient_light = 1.0f;
    Point3 directional_light_normal = {0,0,0};
    float directional_light = 0.0f;

    void projection_update();
    void view_update();

    friend class RenderContext;

public:
    RenderState();

    Matrix4 & getProjecetionMatrix();
    Matrix4 & getViewMatrix();

    sf::Vector2u getContextSize() const;
    void setContextSize(const sf::Vector2u &value);
    float getFov() const;
    void setFov(float value);
    float getNear_clip() const;
    void setNear_clip(float value);
    float getFar_clip() const;
    void setFar_clip(float value);
    Point3 getCam_pos() const;
    void setCam_pos(const Point3 &value);
    Point3 getCam_rot() const;
    void setCam_rot(const Point3 &value);
    void setLightPoint(const LightPoint & lightpoint);
    LightPoint & getLightPoint();
    void setAmbientLight(float ambient);
    void setDirectionalLight(Point3 normal, float strenght);

    void rotateCamera(Point3 rot);
    void moveCamera(Point3 move);

    Point3 getCameraForward();
    Point3 getCameraRight();

};

}
#endif // RENDERSTATE_H
