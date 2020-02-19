#include "menu.h"

Menu::Menu(){}


void Menu::init()
{
    guzik1.setHandler([](const TouchEvent & ev){
        Console::get()<<"siemaaa\n";
        ProgramStage::next_stage<LightTest>();
    });

    guzik2.setHandler([](const TouchEvent & ev){
        Console::get()<<"siema2aa\n";
        ProgramStage::next_stage<Benchmark>();
    });

    guzik3.setHandler([](const TouchEvent & ev){
       ProgramStage::next_stage<Terrain>();
    });

    guzik4.setHandler([](const TouchEvent & ev){
       ProgramStage::next_stage<Demo>();
    });

    text.setFont(font);
    text.setPosition(110,100);
    text.setString("Light Test              Terrain\n\n\nBenchmark           Demo");
}

void Menu::input(sf::Event &ev)
{
}

void Menu::update(double dt)
{
}

void Menu::render(sf::RenderWindow &win)
{
    win.draw(text);
}

void Menu::release()
{
}
