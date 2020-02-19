#ifndef TERRAIN_H
#define TERRAIN_H

#include "IOStuff/programstage.h"
#include "micro3D/rendercontext.h"
#include "IOStuff/touchable.h"
#include "menu.h"

class Terrain : public ProgramStage
{
    m3d::RenderContext render_context{356,256};
    float circle_max = 0.0f;

    Mesh ship_mesh;
    ColorInfo ship_color;

    Mesh cube_mesh;
    ColorInfo cube_color;

    m3d::Model model{cube_mesh, &cube_color};

    Touchable exit{700,500,100,100};

    sf::Font &font{Console::get().getFont()};

    sf::Text text{"RUN",font},
    fps_text{"FPS:", font};

public:
    Terrain();

    // ProgramStage interface
public:
    virtual void init() override;
    virtual void input(sf::Event &ev) override;
    virtual void update(double dt) override;
    virtual void render(sf::RenderWindow &win) override;
    virtual void release() override;
};

#endif // TERRAIN_H
