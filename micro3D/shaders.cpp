#include "shaders.h"


//#define DRAW_Z_BUFFER

void m3d::putPixelSimple(unsigned int x, unsigned int y, void * arg){
    std::mutex raster_mutex;
    std::lock_guard quard (raster_mutex);

    m3d::PixelArgSimple * parg = (m3d::PixelArgSimple*)arg;
    sf::Vector2u size = parg->image->getSize();
    if(x >= size.x || x < 0 || y >= size.y || y <0) return;

    sf::Color color(parg->colors[0].r,parg->colors[0].g,parg->colors[0].b);
    parg->image->setPixel(x,y, color);
}

void m3d::putPixelZBuff(unsigned int x, unsigned int y, void *arg){
    PixelArg * parg = (PixelArg*)arg;
    Point3 (&v)[3] = parg->vertices_projected;
    Color (&c)[3] = parg->colors;
    float view_x = ((float)x - (float)parg->screen_size.x *0.5f) /((float)parg->screen_size.x*0.5f);    // calculate normalized view coordinates (-1,1)
    float view_y = ((float)y - (float)parg->screen_size.y *0.5f) /((float)parg->screen_size.y*0.5f);
    Point3 bary  = get_barycentric_coords(view_x,view_y,parg->vertices_projected[0],parg->vertices_projected[1],parg->vertices_projected[2]);
    float z = get_z(v[0],v[1],v[2],bary);

    if(z <= parg->z_buffer[y * parg->screen_size.x + x]){
        Color result_color = bary_avg(c[0],c[1],c[2], bary);
        sf::Color pix_color( result_color.r, result_color.g, result_color.b);
        parg->image->setPixel(x%parg->screen_size.x,y%parg->screen_size.y, pix_color);
        parg->z_buffer[y * parg->screen_size.x + x] = z;
    }
}

void m3d::putPixelZDebug(unsigned int x, unsigned int y, void *arg){
    PixelArg * parg = (PixelArg*)arg;
    Point3 (&v)[3] = parg->vertices_projected;
    float view_x = ((float)x - (float)parg->screen_size.x *0.5f) /((float)parg->screen_size.x*0.5f);    // calculate normalized view coordinates (-1,1)
    float view_y = ((float)y - (float)parg->screen_size.y *0.5f) /((float)parg->screen_size.y*0.5f);
    Point3 bary  = get_barycentric_coords(view_x,view_y,parg->vertices_projected[0],parg->vertices_projected[1],parg->vertices_projected[2]);
    float z = get_z(v[0],v[1],v[2],bary);

    if(z <= parg->z_buffer[y * parg->screen_size.x + x]){
        int z_temp = z*255;
        z_temp= (z_temp<0)? 0 : (z_temp>255)? 255 : z_temp;
        sf::Color depth_color(z_temp,z_temp,z_temp);

        parg->image->setPixel(x%parg->screen_size.x,y%parg->screen_size.y,depth_color);
        parg->z_buffer[y * parg->screen_size.x + x] = z;
    }
}

void m3d::putPixel(unsigned int x, unsigned int y, void * arg){
    PixelArg * parg = (PixelArg*)arg;
    Color (&c)[3] = parg->colors;
    Point3 (&v)[3] = parg->vertices_projected;
    Point3 (&vw)[3] = parg->vertices_world;
    LightPoint & light_point = parg->light_point;

    if(x >= parg->screen_size.x || y >= parg->screen_size.y) return;    // if pixel is outside screen do nothing

    float view_x = ((float)x - (float)parg->screen_size.x *0.5f) /((float)parg->screen_size.x*0.5f);    // calculate normalized view coordinates (-1,1)
    float view_y = ((float)y - (float)parg->screen_size.y *0.5f) /((float)parg->screen_size.y*0.5f);
    Point3 bary  = get_barycentric_coords(view_x,view_y,parg->vertices_projected[0],parg->vertices_projected[1],parg->vertices_projected[2]);
    Point3 view_pos = get_bary_point(vw[0],vw[1],vw[2],bary);

    float z = get_z(v[0],v[1],v[2],bary);

    if(z <= parg->z_buffer[y * parg->screen_size.x + x]){
        float point_shading = 0.0f;
        if(light_point.distance >0){
            Point3 light_vector = sub(view_pos, light_point.position);  // vector from light to pixel
            float light_distance_sq = lensq(light_vector);              // distance  light to pixel
            float dist_shading = (-1.0f/(light_point.distance*light_point.distance))*light_distance_sq + 1.0f;   // amount of light from point

            light_vector = normalize(light_vector);
            parg->normal = normalize(parg->normal);
            dist_shading = (dist_shading<0)? 0 : dist_shading;
            point_shading = -dot(light_vector, parg->normal) * dist_shading;  // point shading quotient (normal * amount)
            point_shading = (point_shading<0)? 0 : point_shading;
        }

        parg->z_min = std::min(parg->z_min, parg->shade);  // debug info
        parg->z_max = std::max(parg->z_max, parg->shade);

        float shade_result = std::max(point_shading, std::max(parg->ambient, parg->shade)); // final shading quotient

        Color result_color = bary_avg(c[0],c[1],c[2], bary);    // base color of pixel
        result_color = shade(result_color,shade_result);

        sf::Color pix_color( result_color.r, result_color.g, result_color.b);
        parg->image->setPixel(x%parg->screen_size.x,y%parg->screen_size.y, pix_color);
        parg->z_buffer[y * parg->screen_size.x + x] = z;
    }
}
