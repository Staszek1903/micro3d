#ifndef MENU_H
#define MENU_H

#include "IOStuff/programstage.h"
#include "IOStuff/touchable.h"

#include "lighttest.h"
#include "benchmark.h"
#include "terrain.h"
#include "demo.h"

class Menu : public ProgramStage
{
    Touchable guzik1{100,100,200,50},
    guzik2{100,200,200,50},
    guzik3{350,100,200,50},
    guzik4{350,200,200,50};

    sf::Font &font{Console::get().getFont()};
    sf::Text text;
public:
    Menu();

    // ProgramStage interface
public:
    virtual void init() override;
    virtual void input(sf::Event &ev) override;
    virtual void update(double dt) override;
    virtual void render(sf::RenderWindow &win) override;
    virtual void release() override;
};

#endif // MENU_H
