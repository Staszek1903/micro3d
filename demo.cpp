#include "demo.h"

Demo::Demo()
{

}


void Demo::init()
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

    milky_way.setRot({0.78,0,0});
    milky_way.setColor({0,0,0},0.6);
    sun.move({0,0,5});
    planet.setScale({10,10,10});
    planet.move({15,0,-5});
    moon_red.move({0,0,-10});
    moon_green.move({0,0,-10});
    moon_green.setScale({3,3,3});
    ship.rotate({0,3.14,0});
    ship.setScale({2,2,2});


    context.getRenderState().setAmbientLight(0.05);
    context.getRenderState().setDirectionalLight({0,0,1},1);
    context.getRenderState().setFar_clip(100);

    nullRenderState = planetRenderState = normalRenderState = context.getRenderState();

    LightPoint point;
    point.distance = 10;
    point.position = {0,0,3};
    normalRenderState.setLightPoint(point);
    normalRenderState.setFar_clip(20);

    Console::get()<<"RenderState size: "<<sizeof (m3d::RenderState)<<"\n";
}

void Demo::input(sf::Event &ev)
{
    const int rotation_button_index = 2;
    static bool rotation_button = false;

    if(ev.type == sf::Event::MouseButtonPressed){
        if(ev.mouseButton.button == rotation_button_index) rotation_button = true;
    } else if ( ev.type == sf::Event::MouseButtonReleased){
        if(ev.mouseButton.button == rotation_button_index) rotation_button = false;
    }else if( ev.type == sf::Event::MouseMoved){
        static auto prev = ev.mouseMove;

//        if(ev.mouseMove.x <0){
//            sf::Mouse::setPosition({1300,ev.mouseMove.y});
//            ev.mouseMove.x = 1300;
//            prev.x = 1300;
//        }
//        if(ev.mouseMove.x > 1300){
//            sf::Mouse::setPosition({0,ev.mouseMove.y});
//            ev.mouseMove.x = 0;
//            prev.x = 0;
//        }


//        if(ev.mouseMove.y <=10){
//            sf::Mouse::setPosition({ev.mouseMove.x,1000});
//            ev.mouseMove.y = 1000;
//            prev.y = 1000;
//        }
//        if(ev.mouseMove.y > 1000){
//            sf::Mouse::setPosition({ev.mouseMove.y, 80});
//            ev.mouseMove.y = 70;
//            prev.y = 70;
//        }
      //  Console::get()<<"mouse pos {"<<ev.mouseMove.x<<","<<ev.mouseMove.y<<"}\n";


        float dx =  ev.mouseMove.x - prev.x;
        float dy = ev.mouseMove.y - prev.y;

        if(rotation_button){
            nullRenderState.rotateCamera({dx/500.0f, dy/500.0f, 0.0f});
            planetRenderState.rotateCamera({dx/500.0f, dy/500.0f, 0.0f});
            normalRenderState.rotateCamera({dx/500.0f, dy/500.0f, 0.0f});

        }

        prev = ev.mouseMove;

    }else if(ev.type == sf::Event::KeyPressed){

    }
}

void Demo::update(double dt)
{
    if(sf::Keyboard::isKeyPressed( sf::Keyboard::W) ){
        Point3 forward_vect = context.getRenderState().getCameraForward();
        normalRenderState.moveCamera(mult(forward_vect,0.05f));
    }
    else if(sf::Keyboard::isKeyPressed( sf::Keyboard::S) ){
        Point3 forward_vect = context.getRenderState().getCameraForward();
        normalRenderState.moveCamera(neg(mult(forward_vect,0.05f)));
    }
    else if(sf::Keyboard::isKeyPressed( sf::Keyboard::A) ){
        Point3 right_vect = context.getRenderState().getCameraRight();
        normalRenderState.moveCamera(mult(right_vect,0.05f));
    }
    else if(sf::Keyboard::isKeyPressed( sf::Keyboard::D) ){
        Point3 right_vect = context.getRenderState().getCameraRight();
        normalRenderState.moveCamera(neg(mult(right_vect,0.05f)));
    }

    planet.rotate({0,0.0005,0});
}

void Demo::render(sf::RenderWindow &win)
{
    static auto last_stamp = std::chrono::steady_clock::now();
    auto new_stamp = std::chrono::steady_clock::now();
    int us = std::chrono::duration_cast<std::chrono::microseconds>(new_stamp-last_stamp).count();
    last_stamp = new_stamp;
    fps_text.setString("frame:\t"+ std::to_string(us)
                       +"\nFPS:\t"+ std::to_string(1000000.0f/us)
                       );
    context.clear();
    context.setRenderState(nullRenderState);
    context.setPixelFunc(m3d::putPixelZBuff);
    context.render(milky_way);
    context.render(sun);

    context.clearZBuff();
    context.setRenderState(planetRenderState);
    context.setPixelFunc(m3d::putPixel);
    context.render(planet);
    context.render(moon_red);
    context.render(moon_green);

    context.clearZBuff();               // duze obicązenie. razem z powyższymi 50% fps
    context.setPixelFunc(m3d::putPixel);
    context.setRenderState(normalRenderState);
    context.render(ship);

    context.display();  // nie wiele obciązenia ~1fps


    win.draw(context);
    win.draw(fps_text);
}

void Demo::release()
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
