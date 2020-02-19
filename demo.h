#ifndef DEMO_H
#define DEMO_H

#include "IOStuff/programstage.h"
#include "IOStuff/touchable.h"
#include "micro3D/rendercontext.h"
#include "menu.h"
#include "chrono"

class Demo : public ProgramStage
{
    Touchable exit{900,500,50,50};
    m3d::RenderContext context{356,256};
    sf::Font &font{Console::get().getFont()};
    sf::Text fps_text{"FPS:", font};

    Mesh milky_way_mesh, sun_mesh, planet_mesh, moon_red_mesh, moon_green_mesh, ship_mesh;
    ColorInfo milky_way_color, sun_color, planet_color, moon_red_color, moon_green_color, ship_color;
    m3d::Model milky_way{milky_way_mesh, &milky_way_color},
    sun{sun_mesh, &sun_color},
    planet{planet_mesh, &planet_color},
    moon_red{moon_red_mesh, &moon_red_color},
    moon_green{moon_green_mesh, &moon_green_color},
    ship{ship_mesh, &ship_color};

    m3d::RenderState nullRenderState, planetRenderState, normalRenderState;

public:
    Demo();

    // ProgramStage interface
public:
    virtual void init() override;
    virtual void input(sf::Event &ev) override;
    virtual void update(double dt) override;
    virtual void render(sf::RenderWindow &win) override;
    virtual void release() override;
};

#endif // DEMO_H
