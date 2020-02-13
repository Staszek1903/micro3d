#ifndef SHADERS_H
#define SHADERS_H

#include <SFML/Graphics.hpp>
#include "point3f.h"
#include "lightpoint.h"
#include "rasterization.h"

namespace m3d {

struct PixelArg{
    sf::Image * image;
    float * z_buffer;

    Color (&colors) [3];
    Point3 (&vertices_projected) [3];
    Point3 (&vertices_world) [3];
    Point3 &normal;

    sf::Vector2u screen_size;
    float z_min, z_max;

    LightPoint &light_point;
    float shade = 1.0f;
    float ambient = 0.1f;

    PixelArg(Color (&colors) [3], Point3 (&vertices_projected)[3], Point3 (&vertices_world) [3],
                Point3 & normal, LightPoint & point)
    : colors(colors), vertices_projected(vertices_projected), vertices_world(vertices_world),
      normal(normal), light_point(point) {}
};

void putPixelSimple(unsigned int x, unsigned int y, void * arg);
void putPixelZBuff(unsigned int x, unsigned int y, void * arg);
void putPixelZDebug(unsigned int x, unsigned int y, void * arg);
void putPixel(unsigned int x, unsigned int y, void * arg);

}

#endif // SHADERS_H
