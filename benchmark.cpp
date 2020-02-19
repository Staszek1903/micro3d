#include "benchmark.h"

Benchmark::Benchmark()
{

}

void Benchmark::init()
{
    render_context.setPosition(10,10);
    render_context.setScale(2.5,2.5);

    exit.create(700,500,100,100);
    exit.setFillColor(sf::Color::Magenta);
    exit.setHandler([](const TouchEvent & ev){
       ProgramStage::next_stage<Menu>();
    });

    run_benchmark.setHandler([&](const TouchEvent & ev){
        if(ev.state != TouchEvent::END) return;
        m3d::Model model(cube_mesh, &cube_color);
        model.setPos({(float)distribution(rand_generator),
                     (float)distribution(rand_generator),
                     5});

        Color color = {(uint8_t) int_dist(rand_generator),
                       (uint8_t) int_dist(rand_generator),
                       (uint8_t) int_dist(rand_generator)};

        model.setColor(color, 1.0f);
        models.push_back(model);
    });

    stop_bemchmark.setHandler([&](const TouchEvent & ev){
        if(ev.state != TouchEvent::END) return;
        models.pop_back();
    });

    text.setPosition(700,400);
    fps_text.setPosition(700,50);

    loadMeshFromPlyFile("./cube.ply", &cube_mesh, &cube_color);

    //render_context.getRenderState().setDirectionalLight({1,1,1},1);
    render_context.getRenderState().setAmbientLight(0.1f);
}

void Benchmark::input(sf::Event &ev)
{
}

void Benchmark::update(double dt)
{
}

void Benchmark::render(sf::RenderWindow &win)
{
    constexpr int max_nanosecond = 100000;

    static auto last_stamp = std::chrono::steady_clock::now();
    auto new_stamp = std::chrono::steady_clock::now();
    int us = std::chrono::duration_cast<std::chrono::microseconds>(new_stamp-last_stamp).count();
    last_stamp = new_stamp;
    fps_text.setString("frame:\t"+ std::to_string(us)
                       +"\nFPS:\t"+ std::to_string(1000000.0f/us)
                       +"\nmodels:\t"+ std::to_string(models.size()));

    render_context.clear();
    for(m3d::Model &m : models){
         render_context.render(m);
    }

    render_context.display();
    win.draw(render_context);
    win.draw(text);
    win.draw(fps_text);
}

void Benchmark::release()
{
    deleteMesh(&cube_mesh);
    deleteColorInfo(&cube_color);
}
