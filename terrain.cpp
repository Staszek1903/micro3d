#include "terrain.h"

Terrain::Terrain()
{

}


void Terrain::init()
{
    loadMeshFromPlyFile("./terrain.ply",&cube_mesh, &cube_color);
    render_context.setPosition(10,10);
    render_context.setScale(2.5,2.5);
    exit.setHandler([](const TouchEvent & ev){
       ProgramStage::next_stage<Menu>();
    });
    fps_text.setPosition(700,100);

    auto & state = render_context.getRenderState();
    state.setFar_clip(100);
    state.setAmbientLight(0.1);
    state.setDirectionalLight({1,1,1},1);
}

void Terrain::input(sf::Event &ev)
{
    if(ev.type == sf::Event::MouseButtonPressed){

    }else if( ev.type == sf::Event::MouseMoved){
        static auto prev = ev.mouseMove;

        if(ev.mouseMove.x <=0){
            sf::Mouse::setPosition({1700,ev.mouseMove.y+70});
            ev.mouseMove.x = 1700;
            prev.x = 1700;
        }
        if(ev.mouseMove.x > 1700){
            sf::Mouse::setPosition({1,ev.mouseMove.y+70});
            ev.mouseMove.x = 1;
            prev.x = 1;
        }

        if(ev.mouseMove.y <=10){
            sf::Mouse::setPosition({ev.mouseMove.x,1000});
            ev.mouseMove.y = 1000;
            prev.y = 1000;
        }
        if(ev.mouseMove.y > 1000){
            sf::Mouse::setPosition({ev.mouseMove.y, 80});
            ev.mouseMove.y = 70;
            prev.y = 70;
        }


        float dx =  ev.mouseMove.x - prev.x;
        float dy = ev.mouseMove.y - prev.y;

        render_context.getRenderState().rotateCamera({dx/500.0f, dy/500.0f, 0.0f});

        prev = ev.mouseMove;

    }else if(ev.type == sf::Event::KeyPressed){

    }
}

void Terrain::update(double dt)
{
    if(sf::Keyboard::isKeyPressed( sf::Keyboard::W) ){
        Point3 forward_vect = render_context.getRenderState().getCameraForward();
        render_context.getRenderState().moveCamera(mult(forward_vect,0.1f));
    }
    else if(sf::Keyboard::isKeyPressed( sf::Keyboard::S) ){
        Point3 forward_vect = render_context.getRenderState().getCameraForward();
        render_context.getRenderState().moveCamera(neg(mult(forward_vect,0.1f)));
    }
    else if(sf::Keyboard::isKeyPressed( sf::Keyboard::A) ){
        Point3 right_vect = render_context.getRenderState().getCameraRight();
        render_context.getRenderState().moveCamera(mult(right_vect,0.05f));
    }
    else if(sf::Keyboard::isKeyPressed( sf::Keyboard::D) ){
        Point3 right_vect = render_context.getRenderState().getCameraRight();
        render_context.getRenderState().moveCamera(neg(mult(right_vect,0.05f)));
    }
}

void Terrain::render(sf::RenderWindow &win)
{
    static auto last_stamp = std::chrono::steady_clock::now();
     auto new_stamp = std::chrono::steady_clock::now();

    int us = std::chrono::duration_cast<std::chrono::microseconds>(new_stamp-last_stamp).count();
    fps_text.setString("FRAME:\t"+ std::to_string(us) + "us \nFPS:\t"
                       + std::to_string(1000000.0f/us)
                       );

    last_stamp = new_stamp;

    render_context.clear();
    render_context.render(model);
    render_context.display();
    win.draw(render_context);
    win.draw(fps_text);
}

void Terrain::release()
{
    deleteMesh(&cube_mesh);
    deleteColorInfo(&cube_color);
}
