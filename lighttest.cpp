#include "lighttest.h"

LightTest::LightTest()
    : render_context(256,256), model(ship_mesh, &ship_color), model2(ship_mesh, &ship_color), light_model(cube_mesh, &cube_color), last_stamp(std::chrono::steady_clock::now()) {}

void LightTest::init(){
    fps_text.setFont(Console::get().getFont());
    fps_text.setString("siemanko");
    fps_text.setPosition(700,10);

    render_context.move(10,20);
    render_context.scale(2.5,2.5);

    loadMeshFromPlyFile("./ship.ply", &ship_mesh, &ship_color);
    loadMeshFromPlyFile("./cube.ply", &cube_mesh, &cube_color);

    model.move({0.1,0.1,5.1});
    model2.move({0,0,5});
    model.color = {255,0,0};
    model.alpha = 0.25;
    model2.color = {0,255,0};
    model2.alpha = 0.25;
    light_model.setScale({0.2,0.2,0.2});

    LightPoint point;
    point.position = {0,0,0};
    point.color = {255,255,255};
    point.distance = 5.0f;

    render_context.getRenderState().setLightPoint(point);
    render_context.getRenderState().setAmbientLight( 0.1f);
    render_context.getRenderState().setDirectionalLight({0,1,-1},0);
    render_context.getRenderState().setAmbientLight(0.1f);

    Renderer::get().addDrawable(render_context);
    Renderer::get().addDrawable(fps_text);

    exit.create(700,400,100,100);
    exit.setFillColor(sf::Color::Black);
    exit.setHandler([](const TouchEvent & ev){
        ProgramStage::next_stage<Menu>();
    });
}
void LightTest::input(sf::Event &ev){

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

void LightTest::update(double dt){
    if(sf::Keyboard::isKeyPressed( sf::Keyboard::W) ){
        Point3 forward_vect = render_context.getRenderState().getCameraForward();
        render_context.getRenderState().moveCamera(mult(forward_vect,0.05f));
    }
    else if(sf::Keyboard::isKeyPressed( sf::Keyboard::S) ){
        Point3 forward_vect = render_context.getRenderState().getCameraForward();
        render_context.getRenderState().moveCamera(neg(mult(forward_vect,0.05f)));
    }
    else if(sf::Keyboard::isKeyPressed( sf::Keyboard::A) ){
        Point3 right_vect = render_context.getRenderState().getCameraRight();
        render_context.getRenderState().moveCamera(mult(right_vect,0.05f));
    }
    else if(sf::Keyboard::isKeyPressed( sf::Keyboard::D) ){
        Point3 right_vect = render_context.getRenderState().getCameraRight();
        render_context.getRenderState().moveCamera(neg(mult(right_vect,0.05f)));
    }
    else if(sf::Keyboard::isKeyPressed( sf::Keyboard::I) ){
        Point3 pos = render_context.getRenderState().getLightPoint().position;
        pos.p[2] += 0.05f;
        render_context.getRenderState().getLightPoint().position = pos;
        light_model.setPos(pos);
    }
    else if(sf::Keyboard::isKeyPressed( sf::Keyboard::K) ){
        Point3 pos = render_context.getRenderState().getLightPoint().position;
        pos.p[2] -= 0.05f;
        render_context.getRenderState().getLightPoint().position = pos;
        light_model.setPos(pos);
    }
    else if(sf::Keyboard::isKeyPressed( sf::Keyboard::O) ){
        Point3 pos = render_context.getRenderState().getLightPoint().position;
        pos.p[1] += 0.05f;
        render_context.getRenderState().getLightPoint().position = pos;
        light_model.setPos(pos);
    }
    else if(sf::Keyboard::isKeyPressed( sf::Keyboard::L) ){
        Point3 pos = render_context.getRenderState().getLightPoint().position;
        pos.p[1] -= 0.05f;
        render_context.getRenderState().getLightPoint().position = pos;
        light_model.setPos(pos);
    }

    else if(sf::Keyboard::isKeyPressed( sf::Keyboard::U) ){
        render_context.getRenderState().getLightPoint().distance+=0.05f;
    }
    else if(sf::Keyboard::isKeyPressed( sf::Keyboard::J) ){
        render_context.getRenderState().getLightPoint().distance-=0.05f;
    }
}

void LightTest::render(sf::RenderWindow &win){
    auto new_stamp = std::chrono::steady_clock::now();

//    auto win_pos = win.getPosition();
//    sf::Mouse::setPosition({win_pos.x +100, win_pos.y + 100});

    circle_max += 0.01f;
    model.setRot({circle_max,0,0});
    model2.setRot({0,circle_max,0});

    render_context.clear();
    //render_context.setPixelFunc(m3d::putPixelZDebug);
    render_context.setPixelFunc(m3d::putPixel);
    render_context.render(model);
    render_context.render(model2);

    render_context.setPixelFunc(m3d::putPixelZBuff);
    render_context.render(light_model);
    render_context.drawAxis();

    render_context.display();

    m3d::PixelArg * parg = (m3d::PixelArg*)/*raster_func.arg;//*/render_context.getRasterFunc().arg;

    int us = std::chrono::duration_cast<std::chrono::microseconds>(new_stamp-last_stamp).count();
    fps_text.setString("FRAME:\t"+ std::to_string(us) + "us \nFPS:\t"
                       + std::to_string(1000000.0f/us)
                       + "\nz_min:\t" + std::to_string(parg->z_min)
                       + "\nz_max:\t" + std::to_string(parg->z_max)
                       + "\nlight_distance:\t" + std::to_string(render_context.getRenderState().getLightPoint().distance)
                       );
    last_stamp = new_stamp;
    parg->z_min = std::numeric_limits<float>::max();
    parg->z_max = std::numeric_limits<float>::min();
}
void LightTest::release(){
    //delete [] z_buffer;
    deleteMesh(&ship_mesh);
    deleteColorInfo(&ship_color);
    deleteMesh(&cube_mesh);
    deleteColorInfo(&cube_color);

    Renderer::get().removeDrawable(fps_text);
    Renderer::get().removeDrawable(render_context);
}
