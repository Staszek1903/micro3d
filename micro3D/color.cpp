#include "color.h"

Color blend(Color a, Color b, float alpha)
{
    a.r =  a.r * (1.0f-alpha) + b.r * alpha;
    a.g =  a.g * (1.0f-alpha) + b.g * alpha;
    a.b =  a.b * (1.0f-alpha) + b.b * alpha;

    return a;
}

Color shade(Color a, float bright)
{
    a.r *= bright;
    a.g *= bright;
    a.b *= bright;

    return a;
}

Color bary_avg(Color a, Color b, Color c, Point3 bary)
{
    return{
        (uint8_t)(bary.p[0] * a.r + bary.p[1] * b.r + bary.p[2] * c.r),
        (uint8_t)(bary.p[0] * a.g + bary.p[1] * b.g + bary.p[2] * c.g),
        (uint8_t)(bary.p[0] * a.b + bary.p[1] * b.b + bary.p[2] * c.b)
    };
}

Color add(Color a, Color b)
{
    return {
        (uint8_t)(a.r + b.r),
        (uint8_t)(a.g + b.g),
        (uint8_t)(a.b + b.b)
    };
}

Color sub(Color a, Color b)
{
    return {
        (uint8_t)(a.r - b.r),
        (uint8_t)(a.g - b.g),
        (uint8_t)(a.b - b.b)
    };
}

Color colorAverage(Color * array, int * indices, int size){
    float r=0,g=0,b=0;
    Color average = {0,0,0};

    for(int i=0; i<size; ++i){
        r += array[indices[i]].r;
        g += array[indices[i]].g;
        b += array[indices[i]].b;
    }
    average.r = r/size;
    average.g = g/size;
    average.b = b/size;

    return average;
}
