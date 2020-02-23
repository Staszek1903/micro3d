#ifndef COLOR_H
#define COLOR_H

#include <inttypes.h>
#include "point3f.h"

typedef struct { uint8_t r; uint8_t g; uint8_t b;} Color;
typedef struct { unsigned int size; Color * colors;} ColorInfo;

/**
 * @brief blend - blends two colors with alphla
 * @param a color a
 * @param b color b
 * @param alpha - blend coefficient, amount of color a and (1-aplha) of b
 * @return blended color
 */
Color blend(Color a, Color b, float alpha);

/**
 * @brief shade - multiplies color by value
 * @param a
 * @param bright
 * @return
 */
Color shade(Color a, float bright);

/**
 * @brief bary_avg - gives color corresponding to barycentric coordinates
 * @param a - vertex1 color
 * @param b - vertex2 color
 * @param c - vertex3 color
 * @param bary - barycentric coordinates of the point
 * @return color of the point
 */
Color bary_avg(Color a, Color b, Color c, Point3 bary);

/**
 * @brief add - plain add two colors
 */
Color add(Color a, Color b);

/**
 * @brief sub - plain subtract of colors
 */
Color sub(Color a, Color b);

/**
 * @brief colorAverage - gives average of indiced colors in array
 * @param color - color array
 * @param indices - indices array
 * @param size - indices count
 * @return average color
 */
Color colorAverage(Color * color, int * indices, int size);

#endif // COLOR_H
