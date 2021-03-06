#ifndef LIGHTTEST_H
#define LIGHTTEST_H

#include "IOStuff/programstage.h"
#include "IOStuff/renderer.h"
#include "IOStuff/touchable.h"
#include "micro3D/rendercontext.h"
#include <chrono>
#include "menu.h"

class LightTest : public ProgramStage
{
    m3d::RenderContext render_context;
    float circle_max = 0.0f;

    Mesh ship_mesh;
    ColorInfo ship_color;

    Mesh cube_mesh;
    ColorInfo cube_color;

    m3d::Model model, model2, light_model;

    sf::Text fps_text;

    std::chrono::steady_clock::time_point last_stamp;

    Touchable exit;
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
