#include "rasterization.h"

enum{
    swp_cord,
    mirror_x,
    mirror_y
};

void bresenham_init(Point p1, Point p2, BresenhamState * state)
{
    state->flags = 0;
    state->dx = p2.x - p1.x;
    state->dy = p2.y - p1.y;
    state->b = p2;
    state->a = p1;

   //if(dy < 0){
        SET_BIT(state->flags,mirror_y,(state->dy<0))
        state->dy = state->dy - GET_BIT(state->flags,mirror_y)*2*state->dy;
   //if(dx < 0){
        SET_BIT(state->flags,mirror_x,(state->dx<0))
        state->dx = state->dx - GET_BIT(state->flags,mirror_x)*2*state->dx;

    //if(dy > dx){
        SET_BIT(state->flags,swp_cord,(state->dy>state->dx))
        uint8_t swp_cond = GET_BIT(state->flags,swp_cord);
        CONDITIONAL_SWAP(state->dx,state->dy,swp_cond)

    state->y = 0;
    state->x = 0;

    state->D = 2*state->dy-state->dx;
}

Point bresenham_next(BresenhamState * state)
{
    Point ret = {0,0};
    int x_tmp = state->x;
    int y_tmp = state->y;
    //if(swp)
        uint8_t swp_cond = GET_BIT(state->flags,swp_cord);
        CONDITIONAL_SWAP(x_tmp,y_tmp,swp_cond)
    //if(mirror_x)
        //x_tmp = x_tmp - GET_BIT(flags,mirror_x)*2*x_tmp + GET_BIT(flags,mirror_x)*2*a.x;
        x_tmp = x_tmp - GET_BIT(state->flags,mirror_x)*(2*x_tmp);
    //if(mirror_y)
        //y_tmp = y_tmp - GET_BIT(flags,mirror_y)*2*y_tmp + GET_BIT(flags,mirror_y)*2*a.y;
        y_tmp = y_tmp - GET_BIT(state->flags,mirror_y)*(2*y_tmp);

   // if(state->D>0){
        state->y += (state->D > 0);
        state->D -= (state->D > 0)*2*state->dx;

    state->D += 2*state->dy;
    state->x++;

    ret.x =  state->a.x + x_tmp;
    ret.y =  state->a.y + y_tmp;
    return ret;
}

uint8_t bresenham_hasnext(BresenhamState * state){
    return (state->x < state->dx);
}


void line(Point a, Point b, RasterFunc func)
{
  //  if(a.x >255 || a.y >255 || b.x > 255 || b.y > 255 || a.x <0 || a.y < 0 || b.x < 0 || b.y < 0) return;
    BresenhamState state;
    bresenham_init(a,b,&state);

    while (bresenham_hasnext(&state)) {
        Point pixel = bresenham_next(&state);
        func.putPixel(pixel.x, pixel.y, func.arg);
    }
}

void strokeTriangle(Triangle t, RasterFunc func)
{
    line(t.p[0],t.p[1],func);
    line(t.p[1],t.p[2],func);
    line(t.p[2],t.p[0],func);
}

Point3 get_barycentric_coords(float x, float y, Point3 a, Point3 b, Point3 c){
    Point3 va = {a.p[0] - x, a.p[1] - y, 0},
            vb = {b.p[0] - x, b.p[1] - y, 0},
            vc = {c.p[0] - x, c.p[1] - y, 0};

// współrzędne barycentryczne XD
    float la = 0.5f*(vb.p[0] * vc.p[1] - vb.p[1] * vc.p[0]);
    float lb = 0.5f*(vc.p[0] * va.p[1] - vc.p[1] * va.p[0]);
    float lc = 0.5f*(va.p[0] * vb.p[1] - va.p[1] * vb.p[0]);

//    assert(la>0);
//    assert(lb>0);
//    assert(lc>0);

    float l_sum = la+lb+lc;

    la = (l_sum)? la/l_sum : 0;
    lb = (l_sum)? lb/l_sum : 0;
    lc = (l_sum)? lc/l_sum : 0;

    return {la,lb,lc};
}

float get_z(Point3 a, Point3 b, Point3 c, Point3 barocentric){
    float mean = barocentric.p[0] * a.p[2]
            + barocentric.p[1] * b.p[2]
            + barocentric.p[2] * c.p[2];

    return mean;
}

void fillTriangleBres(Point3 a, Point3 b, Point3 c, RasterFunc func)
{

    //UPPER TRIANGLE
    Point top= {(int)a.p[0],(int)a.p[1]};
    Point left = {(int)b.p[0],(int)b.p[1]};
    Point right = {(int)c.p[0],(int)c.p[1]};
    Point temp;

    if(left.y < top.y){
        temp = top;
        top = left;
        left = temp;
    }
    if(right.y < top.y){
        temp = top;
        top = right;
        right = temp;
    }

    Point lvect = {left.x - top.x, left.y - top.y};
    Point rvect = {right.x- top.x, right.y- top.y};
    int cross = lvect.x*rvect.y - lvect.y*rvect.x;
    if(cross > 0){
        temp = left;
        left = right;
        right = temp;
    }

    BresenhamState bs_left, bs_right;//, bs_bottom;
    bresenham_init(top,left,&bs_left);
    bresenham_init(top,right,&bs_right);

    int stop_y = (left.y>right.y)?left.y:right.y;

    bool swaped = false;

    Point left_pix = {0,0}, right_pix = {0,0};
    for(int y = top.y; y<=stop_y; ++y){
        if(y == left.y && !swaped){
            bresenham_init(left,right,&bs_left);//bs_left = bs_bottom;
            swaped = true;
        }
        if(y == right.y && !swaped ){
            bresenham_init(right,left,&bs_right);//bs_right = bs_bottom;
            swaped = true;
        }
        while(left_pix.y != y)
            left_pix = bresenham_next(&bs_left);
        while(right_pix.y != y)
            right_pix = bresenham_next(&bs_right);

        for(int x = left_pix.x; x<right_pix.x /*+ 1*/ ; ++x){
            func.putPixel(x,y,func.arg);
        }
    }

}



#define X(vect) vect.p[0]
#define Y(vect) vect.p[1]
#define Z(vect) vect.p[2]
void fillTriangle3D(Point3 a, Point3 b, Point3 c, RasterFunc func)
{
    X(a) = (int)X(a);
    X(b) = (int)X(b);
    X(c) = (int)X(c);

    Y(a) = (int)Y(a);
    Y(b) = (int)Y(b);
    Y(c) = (int)Y(c);
//    func.putPixel(a.p[0],a.p[1],-1, func.arg);
//    func.putPixel(b.p[0],b.p[1],-1, func.arg);
//    func.putPixel(c.p[0],c.p[1],-1, func.arg);

    Point3  temp = {0};
    if(Y(b) < Y(a)){
        temp = a;
        a = b;
        b = temp;
    }

    if(Y(c) < Y(a)){
        temp = a;
        a = c;
        c = temp;
    }

    if(Y(c) < Y(b)){
        temp = b;
        b = c;
        c = temp;
    }

    float start_y = Y(a), middle_y = Y(b), stop_y = Y(c);
    float ax = X(b) - X(a), ay = Y(b) - Y(a), bx = X(c) - X(a), by = Y(c) - Y(a);
    float da = (ay)? ax/ay : 0, db = (by)? bx/by : 0;
//    float daz = (ay)? az/ay  : 0, dbz = (ay)? bz/ay : 0;

    int upper_height = middle_y - start_y;
    int lower_height = stop_y - middle_y;

    for(int i=0; i<upper_height; ++i){
        float line_a = X(a) + da * i;
        float line_b = X(a) + db * i;
        if(line_a > line_b) { std::swap(line_a,line_b); }
        float y = i + start_y;
        float line_length = line_b - line_a;

        for(int j = 0; j<line_length; ++j){
            float x = line_a + j;
            func.putPixel(x,y,func.arg);
        }
    }

    ax = X(c) - X(b);
    ay = Y(c) - Y(b);
    da = (ay)? ax/ay : 0;

    for(int i=0; i<lower_height; ++i){
        float line_a = X(b) + da * i;
        float line_b = X(a) + db * (i+upper_height);
        if(line_a > line_b) { std::swap(line_a,line_b); }
        float y = i + middle_y;
        float line_length = line_b - line_a;

        for(int j = 0; j<line_length; ++j){
            float x = line_a + j;
            func.putPixel(x,y,func.arg);
        }
    }
}

Point3 get_bary_point(Point3 a, Point3 b, Point3 c, Point3 bary)
{
    return {
        bary.p[0] * a.p[0] + bary.p[1] * b.p[0] + bary.p[2] * c.p[0],
        bary.p[0] * a.p[1] + bary.p[1] * b.p[1] + bary.p[2] * c.p[1],
        bary.p[0] * a.p[2] + bary.p[1] * b.p[2] + bary.p[2] * c.p[2]
    };
}

