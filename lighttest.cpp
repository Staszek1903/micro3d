#include "lighttest.h"

LightTest::LightTest()
    : render_context(256,256), model(diamond, &c_info), model2(diamond, &c_info), last_stamp(std::chrono::steady_clock::now()) {}

void LightTest::init(){
    fps_text.setFont(Console::get().getFont());
    fps_text.setString("siemanko");
    fps_text.setPosition(700,10);

    render_context.move(10,20);
    render_context.scale(2.5,2.5);

    loadMeshFromPlyFile("./ship.ply", &diamond, &c_info);
    model.move({0.1,0.1,5.1});
    model2.move({0,0,5});
    model.color = {255,0,0};
    model.alpha = 0.25;
    model2.color = {0,255,0};
    model2.alpha = 0.25;

    LightPoint point;
    point.position = {0,0,3};
    point.color = {255,255,255};
    point.distance = 100.0f;

    render_context.getRenderState().setLightPoint(point);
    render_context.getRenderState().setAmbientLight( 0.1f);
    render_context.getRenderState().setDirectionalLight({0,1,-1},0);
    render_context.getRenderState().setAmbientLight(0.1f);

    Renderer::get().addDrawable(render_context);
    Renderer::get().addDrawable(fps_text);
}
void LightTest::input(sf::Event &ev){

    if(ev.type == sf::Event::MouseButtonPressed){

    }else if( ev.type == sf::Event::MouseMoved){
        float dx =  ev.mouseMove.x - 100;//
        float dy = ev.mouseMove.y - 66;//

        render_context.getRenderState().rotateCamera({dx/1000.0f, dy/1000.0f, 0.0f});
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
}

void LightTest::render(sf::RenderWindow &win){
    auto new_stamp = std::chrono::steady_clock::now();

    auto win_pos = win.getPosition();
    sf::Mouse::setPosition({win_pos.x +100, win_pos.y + 100});

    circle_max += 0.01f;
    model.setRot({circle_max,0,0});
    model2.setRot({0,circle_max,0});

    render_context.clear();
    //render_context.setPixelFunc(m3d::putPixelZDebug);
    render_context.render(model);
    //render_context.setPixelFunc(m3d::putPixel);
    render_context.render(model2);
    render_context.drawAxis();

    render_context.display();

    m3d::PixelArg * parg = (m3d::PixelArg*)/*raster_func.arg;//*/render_context.getRasterFunc().arg;

    int us = std::chrono::duration_cast<std::chrono::microseconds>(new_stamp-last_stamp).count();
    fps_text.setString("FRAME:\t"+ std::to_string(us) + "us \nFPS:\t"
                       + std::to_string(1000000.0f/us)
                       + "\nz_min:\t" + std::to_string(parg->z_min)
                       + "\nz_max:\t" + std::to_string(parg->z_max));
    last_stamp = new_stamp;
    parg->z_min = std::numeric_limits<float>::max();
    parg->z_max = std::numeric_limits<float>::min();
}
void LightTest::release(){
    //delete [] z_buffer;
    deleteMesh(&diamond);
    deleteColorInfo(&c_info);
}
