#include "rendercontext.h"

//#define DRAW_Z_BUFFER

void m3d::putPixel(unsigned int x, unsigned int y, float z, void * arg){
    PixelArg * parg = (PixelArg*)arg;

    if(x >= parg->screen_size.x || y >= parg->screen_size.y) return;

    if(z <= parg->z_buffer[y * parg->screen_size.x + x]){
#ifdef DRAW_Z_BUFFER
        int z_temp = (z-128)*2;
        z_temp= (z_temp<0)? 0 : (z_temp>255)? 255 : z_temp;
        sf::Color depth_color(z_temp,z_temp,z_temp);

        parg->image->setPixel(x%parg->screen_size.x,y%parg->screen_size.y,depth_color);

        parg->z_min = std::min(parg->z_min, z);
        parg->z_max = std::max(parg->z_max, z);
#else
        parg->image->setPixel(x%parg->screen_size.x,y%parg->screen_size.y,/*depth_color);*/*parg->color);
#endif
        parg->z_buffer[y * parg->screen_size.x + x] = z;


    }
}

m3d::RenderContext::RenderContext(int w, int h)
{
    img.create(w,h);
    this->z_buffer = std::make_unique<float[]>(w*h);
    tex.loadFromImage(img);
    this->setTexture(tex);

    pixel_arg.image = &img;
    pixel_arg.z_buffer = this->z_buffer.get();
    pixel_arg.screen_size = sf::Vector2u(w,h);
    pixel_arg.color = &draw_color;

    render_state.setContextSize( img.getSize() );
}

void m3d::RenderContext::clearColor(sf::Color color)
{
    auto size = tex.getSize();
    for(unsigned int i=0; i<size.x; ++i){
        for(unsigned int j=0; j<size.y; ++j)
            img.setPixel(i,j,color);
    }
}

void m3d::RenderContext::clearZBuff()
{
    auto size = tex.getSize();
    unsigned int len = size.x * size.y;
    for(unsigned int i=0; i< len; ++i)
        z_buffer[i] = std::numeric_limits<float>::max();
}

void m3d::RenderContext::clear(sf::Color color)
{
    clearColor(color);
    clearZBuff();
}

sf::Vector2u m3d::RenderContext::getSize()
{
    return tex.getSize();
}

void m3d::RenderContext::display()
{
    tex.loadFromImage(img);
}

RasterFunc m3d::RenderContext::getRasterFunc()
{ 
    RasterFunc func;
    func.putPixel = m3d::putPixel;
    func.arg = &pixel_arg;
    return func;
}

m3d::RenderState &m3d::RenderContext::getRenderState()
{
    return render_state;
}

void m3d::RenderContext::drawAxis()
{
    Point3 o = {0,0,0};
    Point3 x = {1,0,0};
    Point3 y = {0,1,0};
    Point3 z = {0,0,1};
    RasterFunc raster_func = getRasterFunc();
    sf::Vector2u size = getSize();

    Point3 cam_rot = render_state.getCam_rot();
    float pitch = cam_rot.p[1];
    float yaw = cam_rot.p[0];

    Matrix4 rot = {0},rotY = {0},rotX = {0},
            projection = render_state.getProjecetionMatrix();

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

    o = toScreenSpace(o,size.x,size.y);
    x = toScreenSpace(x,size.x,size.y);
    y = toScreenSpace(y,size.x,size.y);
    z = toScreenSpace(z,size.x,size.y);

    draw_color = sf::Color::Red;
    line({(int)o.p[0],(int)o.p[1]},{(int)x.p[0], (int)x.p[1]},raster_func);
    draw_color = sf::Color::Green;
    line({(int)o.p[0],(int)o.p[1]},{(int)y.p[0], (int)y.p[1]},raster_func);
    draw_color = sf::Color::Blue;
    line({(int)o.p[0],(int)o.p[1]},{(int)z.p[0], (int)z.p[1]},raster_func);

}

void m3d::RenderContext::render(Model &model)
{
    RasterFunc raster_func = getRasterFunc();
    Mesh & mesh = model.getMesh();
    ColorInfo & c_info = model.getColorInfo();
    std::unique_ptr<Point3[]> model_normals = std::make_unique<Point3[]>(mesh.triangle_count);
    std::unique_ptr<Point3[]> world_normals = std::make_unique<Point3[]>(mesh.triangle_count);
    std::unique_ptr<Point3[]> projected_normals = std::make_unique<Point3[]>(mesh.triangle_count);
    std::unique_ptr<Point3[]> projected_vertices = std::make_unique<Point3[]>(mesh.vertex_count);

    Matrix4 & model_rotation = model.getRotation();
    Matrix4 & m = model.getTransformation();
    Matrix4 & view = render_state.getViewMatrix();
    Matrix4 & projection = render_state.getProjecetionMatrix();
    Matrix4 mv = multMatrix(&m, &view);
    Matrix4 mvp = multMatrix(&mv, &projection);

    for(size_t i=0; i<mesh.vertex_count; ++i)
        projected_vertices[i] = transformPoint(mesh.vertices[i],&mvp);


    calculateNormals(mesh.triangles, mesh.triangle_count, mesh.vertices, model_normals.get());

    for(size_t i=0; i<mesh.triangle_count; ++i)
    {
        world_normals[i] = transformPoint(model_normals[i], &model_rotation);
    }

    // CALCULATE PROJECTED_NORMALS (SORTED_TRIANGLES, PROJECTED_VERTS) -> PROJECTED NORMALS
    calculateNormals(mesh.triangles, mesh.triangle_count, projected_vertices.get(), projected_normals.get());

    for(size_t i=0; i<mesh.triangle_count; ++i){

        //CULLING
        float direction = dot(projected_normals[i],{0,0,1});
        if( direction >0 ) continue;

        TriangleInd ind = mesh.triangles[i];
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


        draw_color = sf::Color(c_info.colors[i].r *  shading ,
                                           c_info.colors[i].g *  shading ,
                                           c_info.colors[i].b *  shading);

        for(int j=0; j<clip_size; ++j)
            fillTriangle3D(toScreenSpace(clip_result[j][0], 256,256),
                    toScreenSpace(clip_result[j][1], 256,256),
                    toScreenSpace(clip_result[j][2], 256,256),
                    raster_func);

    }

    drawAxis();

    draw_color = sf::Color::Blue;
    fillTriangle3D({10,10,100},
    {100,50,100},
    {30,100,100},
                   getRasterFunc());
}
