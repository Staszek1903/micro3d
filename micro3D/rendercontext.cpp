#include "rendercontext.h"

m3d::RenderContext::RenderContext(int w, int h)
    :pixel_arg(vertexColors,vertices_projected, vertices_world, normal, light_point)
{
    img.create(w,h);
    this->z_buffer = std::make_unique<float[]>(w*h);
    tex.loadFromImage(img);
    this->setTexture(tex);

    pixel_arg.image = &img;
    pixel_arg.z_buffer = this->z_buffer.get();
    pixel_arg.screen_size = sf::Vector2u(w,h);

    render_state.setContextSize( img.getSize() );
}

void m3d::RenderContext::alloc_workspace(unsigned int triangle_count, unsigned int vertex_count)
{
    if(model_normals.size() < triangle_count){
        model_normals.resize(triangle_count);
        world_normals.resize(triangle_count);
        projected_normals.resize(triangle_count);
    }
    if(projected_vertices.size() < vertex_count){
        projected_vertices.resize(vertex_count);
        world_vertices.resize(vertex_count);
    }
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
    func.putPixel = current_put_pixel;
    func.arg = &pixel_arg;
    return func;
}

m3d::RenderState &m3d::RenderContext::getRenderState()
{
    return render_state;
}

void m3d::RenderContext::setRenderState(const m3d::RenderState &state)
{
    render_state = state;
}

void m3d::RenderContext::setPixelFunc(const PixelFunc &value)
{
    current_put_pixel = value;
}

void m3d::RenderContext::drawAxis()
{
    Point3 o = {0,0,0};
    Point3 x = {1,0,0};
    Point3 y = {0,1,0};
    Point3 z = {0,0,1};
    RasterFunc raster_func = getRasterFunc();
    raster_func.putPixel = m3d::putPixelSimple;

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

    vertexColors[0] = {255,0,0};
    line({(int)o.p[0],(int)o.p[1]},{(int)x.p[0], (int)x.p[1]},raster_func);
    vertexColors[0] = {0,255,0};
    line({(int)o.p[0],(int)o.p[1]},{(int)y.p[0], (int)y.p[1]},raster_func);
    vertexColors[0] = {0,0,255};
    line({(int)o.p[0],(int)o.p[1]},{(int)z.p[0], (int)z.p[1]},raster_func);

}

void m3d::RenderContext::render(Model &model)
{
    RasterFunc raster_func = getRasterFunc();
    Mesh & mesh = model.getMesh();
    ColorInfo & c_info = model.getColorInfo();

    alloc_workspace(mesh.triangle_count, mesh.vertex_count);

    // getting transformation matrices
    Matrix4 & model_rotation = model.getRotation();
    Matrix4 & m = model.getTransformation();
    Matrix4 & view = render_state.getViewMatrix();
    Matrix4 & projection = render_state.getProjecetionMatrix();
    Matrix4 mv = multMatrix(&m, &view);
    Matrix4 mvp = multMatrix(&mv, &projection);

    // transforming data
    for(size_t i=0; i<mesh.vertex_count; ++i)
        world_vertices[i] = transformPoint(mesh.vertices[i],&m);            // transform mesh vertices to world coordinates

    for(size_t i=0; i<mesh.vertex_count; ++i)
        projected_vertices[i] = transformPoint(mesh.vertices[i],&mvp);      // transform mesh vertices to frustrum coordinates

    calculateNormals(mesh.triangles, mesh.triangle_count, mesh.vertices, model_normals.data());      // get model normals

    for(size_t i=0; i<mesh.triangle_count; ++i)         // trasform model normals to world space
    {
        world_normals[i] = transformPoint(model_normals[i], &model_rotation);
    }

    calculateNormals(mesh.triangles, mesh.triangle_count, projected_vertices.data(), projected_normals.data());   // calculate frustrum normals

    light_point.position = render_state.lightpoint.position;        //setting light data
    light_point.color = render_state.lightpoint.color;
    light_point.distance = render_state.lightpoint.distance;
    pixel_arg.ambient = render_state.ambient_light;

    for(size_t i=0; i<mesh.triangle_count; ++i){        // draw triangles

        float direction = dot(projected_normals[i],{0,0,1});            //culling
        if( direction >0 ) continue;

        TriangleInd ind = mesh.triangles[i];
        vertices_projected[0] = projected_vertices[ind.i[0]];      // load vertex data for pixel calculation
        vertices_projected[1] = projected_vertices[ind.i[1]];
        vertices_projected[2] = projected_vertices[ind.i[2]];
        vertices_world[0] = world_vertices[ind.i[0]];
        vertices_world[1] = world_vertices[ind.i[1]];
        vertices_world[2] = world_vertices[ind.i[2]];

        vertexColors[0] = c_info.colors[ind.i[0]];          // load color data
        vertexColors[1] = c_info.colors[ind.i[1]];
        vertexColors[2] = c_info.colors[ind.i[2]];

        vertexColors[0] = blend(vertexColors[0], model.color,model.alpha);  // blend vertex color with model color
        vertexColors[1] = blend(vertexColors[1], model.color,model.alpha);
        vertexColors[2] = blend(vertexColors[2], model.color,model.alpha);

        normal = world_normals[i];                          // load triangle normal

        Point3 clip_result[64][3];
        int clip_size = clipToScreen(vertices_projected,0.0f,1.0f, clip_result);    //clipping

        float shading = dot(world_normals[i], render_state.directional_light_normal) * render_state.directional_light;         //shading
        shading = (shading<0)?0:(shading>1)?1:shading;
        pixel_arg.shade = shading;

        for(int j=0; j<clip_size; ++j)
            fillTriangleBres(toScreenSpace(clip_result[j][0], pixel_arg.screen_size.x, pixel_arg.screen_size.y),
                    toScreenSpace(clip_result[j][1],pixel_arg.screen_size.x, pixel_arg.screen_size.y),
                    toScreenSpace(clip_result[j][2], pixel_arg.screen_size.x, pixel_arg.screen_size.y),
                    raster_func);

    }
}
