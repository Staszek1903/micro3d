#ifndef RENDERCONTEXTMT_H
#define RENDERCONTEXTMT_H

#include <SFML/Graphics.hpp>
#include "micro3d.h"
#include "rasterization.h"
#include "model.h"
#include "shaders.h"
#include "renderstate.h"

#include "multiThread/vertexworker.h"
#include "multiThread/fragmentworker.h"

//class VertexJobQueue;
//class FragmentJobQueue;

//void vertex_thread(VertexJobQueue& v_queue, FragmentJobQueue& f_queue);
//void fragment_thread(FragmentJobQueue& f_queue);

namespace m3d {

class RenderContextMT : public sf::Sprite
{
    static constexpr int VWORKERSCOUNT = 8;
    //static constexpr int FWORKERSCOUNT = 0;

    sf::Image img;
    sf::Texture tex;
    std::unique_ptr <float[]> z_buffer;
    sf::Vector2u screen_size;

//    Color vertexColors[3];
//    Point3 vertices_projected[3];
//    Point3 vertices_world[3];
//    Point3 normal;
//    LightPoint light_point;

    PixelFunc current_put_pixel = m3d::putPixel;
    m3d::RenderState render_state;
    //PixelArg pixel_arg;


    JobQueue<VertexJob> vertex_queue;
    JobQueue<FragmentJob> fragment_queue;
    //VertexWorker worker1, worker2, worker3, worker4;
    std::vector<VertexWorker> vworkers;
    //std::vector<FragmentWorker> fworkers;

    //VertexWorker vworkers[VWORKERSCOUNT];
    //FragmentWorker fworkers[FWORKERSCOUNT];

    void runWorkers();
    void stopJoinWorkers();
    bool areWorkersWorking();

public:
    RenderContextMT(int w, int h);
    ~RenderContextMT();

    void clearColor(sf::Color color = sf::Color::Black);
    void clearZBuff();

    /**
     * @brief clear both color and z_buffer
     * @param color
     */
    void clear(sf::Color color = sf::Color::Black);


    void setPixelFunc(const PixelFunc & value);
    void render(Model & model);
    void display();

    m3d::RenderState getRenderState() const;
    void setRenderState(const m3d::RenderState &value);
};

}


#endif // RENDERCONTEXTMT_H
