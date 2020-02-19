#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "IOStuff/programstage.h"
#include "IOStuff/touchable.h"
#include "micro3D/rendercontext.h"
#include "menu.h"
#include <chrono>
#include <random>

class Benchmark : public ProgramStage
{
    Touchable exit, run_benchmark{700,400,200,50}, stop_bemchmark{700,300,200,50};
    m3d::RenderContext render_context{256,256};
    Mesh cube_mesh;
    ColorInfo cube_color;

    sf::Font & font{Console::get().getFont()};
    sf::Text text{"RUN",font},
    fps_text{"FPS:", font};

    std::uniform_real_distribution<double> distribution{-5, 4};
    std::uniform_int_distribution<int> int_dist{0,255};
    std::mt19937 rand_generator;

    std::vector <m3d::Model> models;

public:
    Benchmark();

    // ProgramStage interface
public:
    virtual void init() override;
    virtual void input(sf::Event &ev) override;
    virtual void update(double dt) override;
    virtual void render(sf::RenderWindow &win) override;
    virtual void release() override;
};

#endif // BENCHMARK_H
