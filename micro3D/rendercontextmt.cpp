#include "rendercontextmt.h"

void m3d::RenderContextMT::runWorkers()
{
    for(int i=0; i<VWORKERSCOUNT; ++i)
        vworkers[i].run();
//    for(int i=0; i<FWORKERSCOUNT; ++i)
//        fworkers[i].run();
}

void m3d::RenderContextMT::stopJoinWorkers()
{
    for(int i=0; i<VWORKERSCOUNT; ++i)
        vworkers[i].stop();
//    for(int i=0; i<FWORKERSCOUNT; ++i)
//        fworkers[i].stop();

    for(int i=0; i<VWORKERSCOUNT; ++i)
        vworkers[i].join();
//    for(int i=0; i<FWORKERSCOUNT; ++i)
//        fworkers[i].join();
}

bool m3d::RenderContextMT::areWorkersWorking()
{
    bool ret = false;
    for(int i=0; i<VWORKERSCOUNT; ++i)
        ret |= vworkers[i].isWorking();

    return ret;
}

m3d::RenderContextMT::RenderContextMT(int w, int h)
   // :pixel_arg(vertexColors,vertices_projected, vertices_world, normal, light_point)
//    :worker1(vertex_queue, &img, sf::Vector2u(w,h)),
//      worker2(vertex_queue, &img, sf::Vector2u(w,h)),
//      worker3(vertex_queue, &img, sf::Vector2u(w,h)),
//      worker4(vertex_queue, &img, sf::Vector2u(w,h))
//    :vworkers({VertexWorker(vertex_queue, &img, sf::Vector2u(w,h)),
//             VertexWorker(vertex_queue, &img, sf::Vector2u(w,h)),
//             VertexWorker(vertex_queue, &img, sf::Vector2u(w,h)),
//             VertexWorker(vertex_queue, &img, sf::Vector2u(w,h))})
    : z_buffer(std::make_unique<float[]>(w*h)),
      vworkers(VWORKERSCOUNT, VertexWorker(vertex_queue, fragment_queue, &img, z_buffer.get() , sf::Vector2u(w,h)))
     // fworkers(FWORKERSCOUNT, FragmentWorker(fragment_queue, &img, sf::Vector2u(w,h)))
{
    img.create(w,h);
    //this->z_buffer = std::make_unique<float[]>(w*h);
    tex.loadFromImage(img);
    this->setTexture(tex);

//    pixel_arg.image = &img;
//    pixel_arg.z_buffer = this->z_buffer.get();
//    pixel_arg.screen_size = sf::Vector2u(w,h);
    screen_size = sf::Vector2u(w,h);

    render_state.setContextSize( img.getSize() );

//    worker1.run();
//    worker2.run();
//    worker3.run();
//    worker4.run();
    runWorkers();

    //std::fill(workers, VertexWorker(vertex_queue, &img, sf::Vector2u(w,h));
}

m3d::RenderContextMT::~RenderContextMT()
{
    stopJoinWorkers();
//    worker1.stop_and_join();
//    worker2.stop_and_join();
//    worker3.stop_and_join();
//    worker4.stop_and_join();
    Console::get()<<"workers joined\n";
}

void m3d::RenderContextMT::setPixelFunc(const PixelFunc &value)
{
    current_put_pixel = value;
}

void m3d::RenderContextMT::render(Model &model)
{
    Mesh & mesh = model.getMesh();
    ColorInfo & c_info = model.getColorInfo();

    Matrix4 & model_rotation = model.getRotation();
    Matrix4 & m = model.getTransformation();
    Matrix4 & view = render_state.getViewMatrix();
    Matrix4 & projection = render_state.getProjecetionMatrix();
    Matrix4 mv = multMatrix(&m, &view);
    Matrix4 mvp = multMatrix(&mv, &projection);


    TriangleInd ind;
    Point3 tri [3];
    for(size_t i=0; i<mesh.triangle_count; ++i) {
        ind = mesh.triangles[i];
        tri[0] = mesh.vertices[ind.i[0]];
        tri[1] = mesh.vertices[ind.i[1]];
        tri[2] = mesh.vertices[ind.i[2]];
        VertexJob job(tri, mvp);

        job.colors[0] = c_info.colors[ind.i[0]];
        job.colors[1] = c_info.colors[ind.i[1]];
        job.colors[2] = c_info.colors[ind.i[2]];

        job.model_matrix = m;
        job.model_rotation = model_rotation;

        while(vertex_queue.isFull()){}
        vertex_queue.addJob(job);
    }

//    Console::get()<<"emited "<<mesh.triangle_count<<" jobs\n";

//    worker1.counter = 0;
//    worker2.counter = 0;
//    worker3.counter = 0;
//    worker4.counter = 0;

//    PixelArgSimple pixel_arg(img, {255,255,255});
//    RasterFunc func;
//    func.putPixel = m3d::putPixelSimple;
//    func.arg = &pixel_arg;

//    for(size_t i=0; i<mesh.triangle_count; ++i) {
//        TriangleInd ind = mesh.triangles[i];
//        Point3 v1 = mesh.vertices[ind.i[0]],
//               v2 = mesh.vertices[ind.i[1]],
//               v3 = mesh.vertices[ind.i[2]];
//        v1 = transformPoint(v1, &mvp);
//        v2 = transformPoint(v2, &mvp);
//        v3 = transformPoint(v3, &mvp);

//        Point3 clip_input [3] = {v1,v2,v3};
//        Point3 clip_result[64][3];
//        int clip_size = clipToScreen(clip_input, 0.0f, 1.0f, clip_result);
//        for(int j= 0; j<clip_size; ++j){
//            v1 = toScreenSpace(clip_result[j][0], screen_size.x, screen_size.y);
//            v2 = toScreenSpace(clip_result[j][1], screen_size.x, screen_size.y);
//            v3 = toScreenSpace(clip_result[j][2], screen_size.x, screen_size.y);

//            Triangle tr = make_triange(v1,v2,v3);
//            strokeTriangle(tr, func);
//        }
//    }
}

void m3d::RenderContextMT::display()
{
    while(!vertex_queue.isEmpty()){}
    //while(!fragment_queue.isEmpty()){}
    while(areWorkersWorking()){}
    tex.loadFromImage(img);
}

void m3d::RenderContextMT::clearColor(sf::Color color)
{
    auto size = tex.getSize();
    for(unsigned int i=0; i<size.x; ++i){
        for(unsigned int j=0; j<size.y; ++j)
            img.setPixel(i,j,color);
    }
}

void m3d::RenderContextMT::clearZBuff()
{
    auto size = tex.getSize();
    unsigned int len = size.x * size.y;
    for(unsigned int i=0; i< len; ++i)
        z_buffer[i] = std::numeric_limits<float>::max();
}

void m3d::RenderContextMT::clear(sf::Color color)
{
    clearColor(color);
    clearZBuff();
}


m3d::RenderState m3d::RenderContextMT::getRenderState() const
{
    return render_state;
}

void m3d::RenderContextMT::setRenderState(const m3d::RenderState &value)
{
    render_state = value;
}
