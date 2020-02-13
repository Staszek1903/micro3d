#ifndef LIGHTTEST_H
#define LIGHTTEST_H

#include "IOStuff/programstage.h"
#include "IOStuff/renderer.h"
#include "micro3D/rendercontext.h"
#include <chrono>

class LightTest : public ProgramStage
{
    m3d::RenderContext render_context;
    float circle_max = 0.0f;

    Mesh diamond = {0};
    ColorInfo c_info = {0};

    m3d::Model model, model2;
    sf::Text fps_text;

    std::chrono::steady_clock::time_point last_stamp;
public:
    LightTest();

    // ProgramStage interface
public:
    virtual void init() override;
    virtual void input(sf::Event &ev) override;
    virtual void update(double dt) override;
    virtual void render(sf::RenderWindow &win) override;
    virtual void release() override;
};

#endif // LIGHTTEST_H
