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


//#define TRI_SORTED

using namespace std;

class P : public ProgramStage
{
    m3d::RenderContext render_context;

    sf::Texture tex;
    sf::Sprite sprite;
    float circle_max = 0.0f;

    m3d::PixelArg pixelArg;
    RasterFunc raster_func;

    sf::Image image;
    float * z_buffer;
    sf::Color color = sf::Color::White;

    Point3 camera_pos= {0.0f,0.0f,0.0f},
    camera_target = {0.0f,0.0f,1.0f},
    forward_vect = {0.0f,0.0f,1.0f},
    right_vect = {1.0f,0.0f,0.0f};
    float yaw = 0, pitch = 0;

    Matrix4 projection = {0};

    Mesh diamond = {0};
    ColorInfo c_info = {0};

    void drawAxis(){
        Point3 o = {0,0,0};
        Point3 x = {1,0,0};
        Point3 y = {0,1,0};
        Point3 z = {0,0,1};

        Matrix4 rot = {0},rotY = {0},rotX = {0};
        rotationXMatrix(&rotX, pitch);
        rotationYMatrix(&rotY, yaw);
        rot = multMatrix(&rotY, &rotX);



        o = transformPoint(o, &rot);
        x = transformPoint(x, &rot);
        y = transformPoint(y, &rot);
        z = transformPoint(z, &rot);

        o.p[2] += 5;
        x.p[2] += 5;
        z.p[2] += 5;
        y.p[2] += 5;

        o = transformPoint(o, &projection);
        x = transformPoint(x, &projection);
        y = transformPoint(y, &projection);
        z = transformPoint(z, &projection);

        o = pointToScreen(o,256,256);
        x = pointToScreen(x,256,256);
        y = pointToScreen(y,256,256);
        z = pointToScreen(z,256,256);
        color = sf::Color::Red;
        line({(int)o.p[0],(int)o.p[1]},{(int)x.p[0], (int)x.p[1]},raster_func);
        color = sf::Color::Green;
        line({(int)o.p[0],(int)o.p[1]},{(int)y.p[0], (int)y.p[1]},raster_func);
        color = sf::Color::Blue;
        line({(int)o.p[0],(int)o.p[1]},{(int)z.p[0], (int)z.p[1]},raster_func);
    }

    // ProgramStage interface
public:
    P() : render_context(256,256) {}

    void init(){

        render_context.move(550,100);
        render_context.scale(1.7,2);

        image.create(256,256,sf::Color::Black);
        z_buffer = new float [256*256];
        tex.loadFromImage(image);
        sprite.setTexture(tex);
        sprite.setScale(1.7,2);
        sprite.move(20,100);

        pixelArg = {&image, z_buffer, &color, {256,256}};
        raster_func = {&pixelArg, m3d::putPixel};

        projectionMatrix(&projection,1.0f,90, 1, 30);
        printMatix(projection);


        loadMeshFromPlyFile("./ship.ply", &diamond, &c_info);
        //loadMeshFromObjFile("./ship.obj",&diamond);


        //test
//        Console::get()<<"LINE INTERSECT TEST\n";
//        Point3 inter = linePlaneIntersection({0,10,0},{1,0,0},{-2,0,0},{1,2,0});
//        Console::get()<<" {"<<inter.p[0]<<" , "<<inter.p[1]<<" , "<<inter.p[2]<<"}\n";
    }
    void input(sf::Event &ev){

        if(ev.type == sf::Event::MouseButtonPressed){

        }else if( ev.type == sf::Event::MouseMoved){
            float dx =  ev.mouseMove.x - 100;//
            yaw -= dx/1000.0f;

            float dy = ev.mouseMove.y - 66;//
            pitch += dy/1000.0f;

            render_context.getRenderState().rotateCamera({dx/1000.0f, dy/1000.0f, 0.0f});

            //Console::get()<<"cx:"<<dx<<" cy:"<<dy<<" wpx:"<<win_pos.x<<" wpy:"<<win_pos.y<<"\n";

            Matrix4 rot = {0},rotY = {0},rotX = {0};
            rotationXMatrix(&rotX, pitch);
            rotationYMatrix(&rotY, yaw);
            rot = multMatrix(&rotX, &rotY);

            camera_target = {0,0,1};
            forward_vect = camera_target = transformPoint(camera_target, &rot);
            right_vect = cross(forward_vect, {0,-1,0});
            camera_target = add(camera_pos, camera_target);

        }else if(ev.type == sf::Event::KeyPressed){

        }
    }

    void update(double dt){
        if(sf::Keyboard::isKeyPressed( sf::Keyboard::W) ){
            camera_pos = add(camera_pos, mult(forward_vect,0.05f));
            render_context.getRenderState().moveCamera(mult(forward_vect,0.05f));
        }
        else if(sf::Keyboard::isKeyPressed( sf::Keyboard::S) ){
            camera_pos = sub(camera_pos, mult(forward_vect,0.05f));
            render_context.getRenderState().moveCamera(neg(mult(forward_vect,0.05f)));
        }
        else if(sf::Keyboard::isKeyPressed( sf::Keyboard::A) ){
            camera_pos = add(camera_pos, mult(right_vect,0.05f));
            render_context.getRenderState().moveCamera(mult(right_vect,0.05f));
        }
        else if(sf::Keyboard::isKeyPressed( sf::Keyboard::D) ){
            camera_pos = sub(camera_pos, mult(right_vect,0.05f));
            render_context.getRenderState().moveCamera(neg(mult(right_vect,0.05f)));
        }
    }

    void render(sf::RenderWindow &win){      

        auto win_pos = win.getPosition();
        sf::Mouse::setPosition({win_pos.x +100, win_pos.y + 100});

        win.clear(sf::Color(100,100,100));

        //CZYSZCZENIE KONTEKSTU
        for(int i=0; i<256; ++i)
            for(int j=0; j<256; ++j)
                image.setPixel(i,j,sf::Color::Black);

        for(int i=0; i<256*256; ++i)
            z_buffer[i] = std::numeric_limits<int>::max();

        // GENERACJA MACIERZY
        Matrix4 rotation = {0};
        Matrix4 translation = {0};
        Matrix4 view = {0}, point_at = {0};

        rotationMatrix(&rotation,{-1,0,0},circle_max);
        translationMatrix(&translation, {0,0,5.0f});
        pointAtMatrix(&point_at, camera_pos, camera_target, {0,-1,0});
        view = inverseTransform(&point_at);
        circle_max += 0.01f;

        Matrix4 m = multMatrix(&rotation, &translation);
        Matrix4 mv = multMatrix(&m, &view);
        Matrix4 mvp = multMatrix(&mv, &projection);

        std::unique_ptr<Point3[]> model_normals = std::make_unique<Point3[]>(diamond.triangle_count);
        std::unique_ptr<Point3[]> world_normals = std::make_unique<Point3[]>(diamond.triangle_count);
        std::unique_ptr<Point3[]> projected_normals = std::make_unique<Point3[]>(diamond.triangle_count);
        std::unique_ptr<Point3[]> projected_vertices = std::make_unique<Point3[]>(diamond.vertex_count);
        std::vector<TriangleInd> sorted_triangles (diamond.triangle_count);

        //TRANSFORM VERTS(MVP, VERTS) -> PROJECTED_VERTS
        for(size_t i=0; i<diamond.vertex_count; ++i)
            projected_vertices[i] = transformPoint(diamond.vertices[i],&mvp);

        //SORT TRIANGLES BY PROJECTED_Z (PROJECTED_VERTS, TRIANGLES) -> SORTED_TRIANGLES
        for(size_t i=0; i<diamond.triangle_count; ++i)
            sorted_triangles[i] = diamond.triangles[i];

#ifdef TRI_SORTED
        auto compare = [&projected_vertices](TriangleInd &a, TriangleInd &b){
            Point3 a_v[3], b_v[3];

            for(int i=0; i<3; ++i){
                a_v[i] = projected_vertices[a.i[i]];
                b_v[i] = projected_vertices[b.i[i]];
            }

            float a_z = (a_v[0].p[2] + a_v[1].p[2] + a_v[2].p[2])/ 3.0f;
            float b_z = (b_v[0].p[2] + b_v[1].p[2] + b_v[2].p[2])/ 3.0f;

            return a_z > b_z;
        };


        std::sort(sorted_triangles.begin(),
                  sorted_triangles.end(),
                  compare);
#endif

        // CALCULATE WORLD NORMALS (SORTED_TRIANGLES, VERTS) -> WORLD NORMALS
        calculateNormals(&sorted_triangles[0],sorted_triangles.size(), diamond.vertices, model_normals.get());
        for(size_t i=0; i<diamond.triangle_count; ++i)
        {
            world_normals[i] = transformPoint(model_normals[i], &rotation);
        }

        // CALCULATE PROJECTED_NORMALS (SORTED_TRIANGLES, PROJECTED_VERTS) -> PROJECTED NORMALS
        calculateNormals(&sorted_triangles[0], sorted_triangles.size(), projected_vertices.get(), projected_normals.get());




        //RENDER
        for(size_t i=0; i<diamond.triangle_count; ++i){

            //CULLING
            float direction = dot(projected_normals[i],{0,0,1});
            if( direction >0 ) continue;

            TriangleInd ind = sorted_triangles[i];
            Point3 triangle[3] = {projected_vertices[ind.i[0]],
                                  projected_vertices[ind.i[1]],
                                  projected_vertices[ind.i[2]]};


            //CLIPPING
            Point3 clip_result[64][3];

            int clip_size = clipToScreen(triangle,0.0f,1.0f, clip_result);

            //SHADING
            float shading = dot(world_normals[i], normalize({1,-1,-1}));

            shading += 1.0f;
            shading /= 2;
            if(shading > 1) shading = 1;


            sf::Color shaded_color = sf::Color(c_info.colors[i].r *  shading ,
                                               c_info.colors[i].g *  shading ,
                                               c_info.colors[i].b *  shading);

            //shaded_color = sf::Color(shading* 255.0f, shading* 255.0f, shading * 255.0f);

//            float mean_z = (clip_result[0][0].p[2] +
//                    clip_result[0][1].p[2] +
//                    clip_result[0][2].p[2]) /3.0f;

            //if(clip_size>0) Console::get()<<"z["<<i<<"]] = "<<mean_z<<"\t";
           // endl++;
//            if(endl == 3){
//                endl = 0;
//                Console::get()<<"\n";
//            }
            for(int j=0; j<clip_size; ++j){
                color = shaded_color;
                //Triangle renderable = toRender(clip_result[j][0],clip_result[j][1],clip_result[j][2]);
            //    fillTriangle(renderable,raster_func);
                fillTriangle3D(toScreenSpace(clip_result[j][0], 256,256),
                        toScreenSpace(clip_result[j][1], 256,256),
                        toScreenSpace(clip_result[j][2], 256,256),
                        raster_func);

                color = sf::Color::White;
                //strokeTriangle(renderable,raster_func);

//                tex.loadFromImage(image);
//                win.draw(sprite);
//                win.display();
            }
        }

//        color = sf::Color::Blue;
//        fillTriangle3D({10,50,0},
//        {100,100,150},
//            {40,150,300},
//                raster_func);


       drawAxis();

       // Console::get()<<"\n\n";

        tex.loadFromImage(image);
        win.draw(sprite);

        m3d::Model model(diamond,&c_info);
        model.move({0,0,5});
        model.setRot({circle_max,0,0});

        render_context.clear();
        render_context.render(model);
        render_context.display();
        win.draw(render_context);

        m3d::PixelArg * parg = (m3d::PixelArg*)/*raster_func.arg;//*/render_context.getRasterFunc().arg;
        Console::get()<<"MIN: "<<parg->z_min<<"\nMAX: "<<parg->z_max<<"\n";
        parg->z_min = std::numeric_limits<float>::max();
        parg->z_max = std::numeric_limits<float>::min();
    }
    void release(){
        delete [] z_buffer;
        deleteMesh(&diamond);
        deleteColorInfo(&c_info);
    }
};

int main()
{
    ProgramStage::next_stage<P>();
    Program p;
    p.run();
    cout << "Hello World!" << endl;
    return 0;
}
