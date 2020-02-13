#ifndef COLOR_H
#define COLOR_H

#include <inttypes.h>
#include "point3f.h"

typedef struct { uint8_t r; uint8_t g; uint8_t b;} Color;
typedef struct { unsigned int size; Color * colors;} ColorInfo;

Color blend(Color a, Color b, float alpha);
Color shade(Color a, float bright);
Color bary_avg(Color a, Color b, Color c, Point3 bary);
Color add(Color a, Color b);
Color sub(Color a, Color b);

#endif // COLOR_H
