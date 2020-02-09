#include "renderstate.h"


m3d::RenderState::RenderState()
{
    identityMatrix(&projection_matix);
    identityMatrix(&view_matrix);
}

Matrix4 &m3d::RenderState::getProjecetionMatrix()
{
    if(obsolete_projection){
        projectionMatrix(&projection_matix,
                         contextSize.x/contextSize.y,
                         fov, near_clip, far_clip);
        obsolete_projection = false;
    }

    return projection_matix;
}

Matrix4 &m3d::RenderState::getViewMatrix()
{
    if(obsolete_view){
        Matrix4 point_at, rotY, rotX, rot;
        Point3 camera_target, forward_vect, right_vect;

        rotationXMatrix(&rotX, cam_rot.p[1]);
        rotationYMatrix(&rotY, cam_rot.p[0]);
        rot = multMatrix(&rotX, &rotY);

        camera_target = {0,0,1};
        forward_vect = camera_target = transformPoint(camera_target, &rot);
        right_vect = cross(forward_vect, {0,-1,0});
        camera_target = add(cam_pos, camera_target);


        pointAtMatrix(&point_at, cam_pos, camera_target, {0,-1,0});
        view_matrix = inverseTransform(&point_at);
        obsolete_view = false;
    }

    return view_matrix;
}

sf::Vector2u m3d::RenderState::getContextSize() const
{
    return contextSize;
}

void m3d::RenderState::setContextSize(const sf::Vector2u &value)
{
    obsolete_projection = true;
    contextSize = value;
}

float m3d::RenderState::getFov() const
{
    return fov;
}

void m3d::RenderState::setFov(float value)
{
    obsolete_projection = true;
    fov = value;
}

float m3d::RenderState::getNear_clip() const
{
    return near_clip;
}

void m3d::RenderState::setNear_clip(float value)
{
    obsolete_projection = true;
    near_clip = value;
}

float m3d::RenderState::getFar_clip() const
{
    return far_clip;
}

void m3d::RenderState::setFar_clip(float value)
{
    obsolete_projection = true;
    far_clip = value;
}

Point3 m3d::RenderState::getCam_pos() const
{
    return cam_pos;
}

void m3d::RenderState::setCam_pos(const Point3 &value)
{
    obsolete_view = true;
    cam_pos = value;
}

Point3 m3d::RenderState::getCam_rot() const
{
    return cam_rot;
}

void m3d::RenderState::setCam_rot(const Point3 &value)
{
    obsolete_view = true;
    cam_rot = value;
}


void m3d::RenderState::rotateCamera(Point3 rot)
{
    obsolete_view = true;
    rot.p[0] = -rot.p[0];
    cam_rot = add(cam_rot, rot);
}

void m3d::RenderState::moveCamera(Point3 move)
{
    obsolete_view = true;
    cam_pos = add(cam_pos, move);
}


