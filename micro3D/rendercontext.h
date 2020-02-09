#ifndef MICRO3DAPI_H
#define MICRO3DAPI_H

#include <SFML/Graphics.hpp>
#include <limits>
#include "micro3D/rasterization.h"
#include "micro3D/transformation.h"
#include "micro3D/model.h"
#include "micro3D/renderstate.h"
#include "micro3D/micro3d.h"

namespace m3d {

typedef struct { sf::Image * image; float * z_buffer; sf::Color * color; sf::Vector2u screen_size; float z_min, z_max; } PixelArg;

void putPixel(unsigned int x, unsigned int y, float z, void * arg);

class RenderContext : public sf::Sprite{
    sf::Image img;
    sf::Texture tex;
    std::unique_ptr <float[]> z_buffer;

    m3d::RenderState render_state;
    sf::Color draw_color;
    PixelArg pixel_arg;


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

    void drawAxis();
    void render(Model &model); // pamietać rzeby wyjebac jak najwiecej do C
};



}


#endif // MICRO3DAPI_H
