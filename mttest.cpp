#include "mttest.h"
#include <menu.h>

MTTest::MTTest()
{

}

void MTTest::init()
{
    context.setPosition(10,10);
    context.setScale(2.5,2.5);

    exit.setHandler([](const TouchEvent & ev){
        ProgramStage::next_stage<Menu>();
    });

    fps_text.setPosition(700,100);
    exit.setFillColor(sf::Color::White);

    loadMeshFromPlyFile("./milkyway.ply", &milky_way_mesh, &milky_way_color);
    loadMeshFromPlyFile("./planet.ply", &planet_mesh, &planet_color);
    loadMeshFromPlyFile("./sun.ply", &sun_mesh, &sun_color);
    loadMeshFromPlyFile("./moon_green.ply", &moon_green_mesh, &moon_green_color);
    loadMeshFromPlyFile("./moon_red.ply", &moon_red_mesh, &moon_red_color);
    loadMeshFromPlyFile("./demo_ship.ply", &ship_mesh, &ship_color);


    Console::get()<<"TRIS COUNTS"<<
                    "\nmilkyway: "<<milky_way_mesh.triangle_count<<
                    "\nplanet: "<<planet_mesh.triangle_count<<
                    "\nsun "<<sun_mesh.triangle_count<<
                    "\nmoon green: "<<moon_green_mesh.triangle_count<<
                    "\nmoon red: "<<moon_red_mesh.triangle_count<<
                    "\nship: "<<ship_mesh.triangle_count<<
                    "\nSUM: "<<(milky_way_mesh.triangle_count+
                                planet_mesh.triangle_count+
                                sun_mesh.triangle_count+
                                moon_green_mesh.triangle_count+
                                moon_red_mesh.triangle_count+
                                ship_mesh.triangle_count)<< "\n";

//    milky_way.setRot({0.78,0,0});
//    milky_way.setColor({0,0,0},0.6);
//    sun.move({0,0,5});
//    planet.setScale({10,10,10});
//    planet.move({15,0,-5});
        ship.setScale({1,1,1});
        ship.move({0,0,3});
//    moon_red.move({0,0,-10});
//    moon_green.move({0,0,-10});
//    moon_green.setScale({3,3,3});
//    ship.rotate({0,3.14,0});
//    ship.setScale({2,2,2});


//    context.getRenderState().setAmbientLight(0.05);
//    context.getRenderState().setDirectionalLight({0,0,1},1);
//    context.getRenderState().setFar_clip(100);

   // nullRenderState = planetRenderState = normalRenderState = context.getRenderState();

    LightPoint point;
    point.distance = 10;
    point.position = {0,0,3};
   // normalRenderState.setLightPoint(point);
   // normalRenderState.setFar_clip(20);

    Console::get()<<"RenderState size: "<<sizeof (m3d::RenderState)<<"\n";
}

void MTTest::input(sf::Event &ev)
{
    const int rotation_button_index = 2;
    static bool rotation_button = false;

    if(ev.type == sf::Event::MouseButtonPressed){
        if(ev.mouseButton.button == rotation_button_index) rotation_button = true;
    } else if ( ev.type == sf::Event::MouseButtonReleased){
        if(ev.mouseButton.button == rotation_button_index) rotation_button = false;
    }else if( ev.type == sf::Event::MouseMoved){
        static auto prev = ev.mouseMove;

        float dx =  ev.mouseMove.x - prev.x;
        float dy = ev.mouseMove.y - prev.y;

        if(rotation_button){
            render_state.rotateCamera({dx/500.0f, dy/500.0f, 0.0f});
        }

        prev = ev.mouseMove;

    }else if(ev.type == sf::Event::KeyPressed){
        if(ev.key.code == sf::Keyboard::S) render_state.moveCamera({0,0,-0.1f});
        if(ev.key.code == sf::Keyboard::W) render_state.moveCamera({0,0,0.1f});
    }
}

void MTTest::update(double dt)
{

}

void MTTest::render(sf::RenderWindow &win)
{
    static auto last_stamp = std::chrono::steady_clock::now();
    auto new_stamp = std::chrono::steady_clock::now();
    int us = std::chrono::duration_cast<std::chrono::microseconds>(new_stamp-last_stamp).count();
    last_stamp = new_stamp;
    fps_text.setString("frame:\t"+ std::to_string(us)
                       +"\nFPS:\t"+ std::to_string(1000000.0f/us)
                       );

    context.clear(sf::Color(0,100,0));

    context.setRenderState(render_state);
    context.render(ship);

    context.display();
    win.draw(context);
    win.draw(fps_text);
}

void MTTest::release()
{
    deleteMesh(&milky_way_mesh);
    deleteColorInfo(&milky_way_color);

    deleteMesh(&sun_mesh);
    deleteColorInfo(&sun_color);

    deleteMesh(&planet_mesh);
    deleteColorInfo(&planet_color);

    deleteMesh(&moon_red_mesh);
    deleteColorInfo(&moon_red_color);

    deleteMesh(&moon_green_mesh);
    deleteColorInfo(&moon_green_color);

    deleteMesh(&ship_mesh);
    deleteColorInfo(&ship_color);
}
