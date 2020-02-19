#ifndef MICRO3DAPI_H
#define MICRO3DAPI_H

#include <SFML/Graphics.hpp>
#include <limits>
#include "rasterization.h"
#include "transformation.h"
#include "model.h"
#include "renderstate.h"
#include "micro3d.h"
#include "color.h"
#include "shaders.h"
#include <thread>

namespace m3d {

class RenderContext : public sf::Sprite{
    sf::Image img;
    sf::Texture tex;
    std::unique_ptr <float[]> z_buffer;

    m3d::RenderState render_state;

    Color vertexColors[3];
    Point3 vertices_projected[3];
    Point3 vertices_world[3];
    Point3 normal;
    LightPoint light_point;
    PixelArg pixel_arg;
    PixelFunc current_put_pixel = m3d::putPixel;

    std::vector<Point3> model_normals;        // zaalokować to moze jakos na stałoe żeby nie alokowało sie za każdym renderem mesha;
    std::vector<Point3> world_normals;
    std::vector<Point3> projected_normals;
    std::vector<Point3> projected_vertices;
    std::vector<Point3> world_vertices;

    void alloc_workspace(int triangle_count, int vertex_count);
public:
    RenderContext(int w, int h);
    void clearColor(sf::Color color = sf::Color::Black);
    void clearZBuff();

    /**
     * @brief clear both color and z_buffer
     * @param color
     */
    void clear(sf::Color color = sf::Color::Black);
    sf::Vector2u getSize();
    void display();
    RasterFunc getRasterFunc();

    m3d::RenderState & getRenderState();
    void setRenderState (const m3d::RenderState & state);

    void drawAxis();
    void render(Model &model); // pamietać rzeby wyjebac jak najwiecej do C
    void setPixelFunc(const PixelFunc &value);
};



}


#endif // MICRO3DAPI_H
