#include <iostream>
#include "IOStuff/program.h"
#include <cmath>

#include <typeindex>

#include "micro3D/rasterization.h"
#include "micro3D/transformation.h"
#include "micro3D/mesh.h"
#include "micro3D/micro3d.h"

#include "micro3D/rendercontext.h"
#include <limits.h>
#include <chrono>
#include <string>

#include "lighttest.h"


//#define TRI_SORTED

using namespace std;

class P : public ProgramStage
{
    Touchable guzik1, guzik2;

    // ProgramStage interface
public:
    P(): guzik1(100,100,100,100), guzik2(210,100,100,100)  {}

    void init(){
        guzik1.setFillColor(sf::Color::Black);
        guzik2.setFillColor(sf::Color::Black);
        guzik1.setHandler([](const TouchEvent & ev){
            Console::get()<<"siemaaa\n";
            ProgramStage::next_stage<LightTest>();
        });

        guzik2.setHandler([](const TouchEvent & ev){
            Console::get()<<"siema2aa\n";
        });
    }
    void input(sf::Event &ev){

    }

    void update(double dt){

    }

    void render(sf::RenderWindow &win){

    }
    void release(){
    }
};

int main()
{
    Program p;
    ProgramStage::next_stage<P>();
    p.run();
    cout << "Hello World!" << endl;
    return 0;
}
