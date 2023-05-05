#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

vec3 color_correction(color c, int samples_per_pixel) {
    auto r = c.x();
    auto g = c.y();
    auto b = c.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    return {r, g, b};
}

std::ostream &write_color(std::ostream &out, const std::vector<color> &colors, int samples_per_pixel) {

    for (color pixel_color: colors) {

        color corrected = color_correction(pixel_color, samples_per_pixel);

        // Write the translated [0,255] value of each color component.
        out << static_cast<int>(256 * clamp(corrected.x(), 0.0, 0.999)) << ' '
            << static_cast<int>(256 * clamp(corrected.y(), 0.0, 0.999)) << ' '
            << static_cast<int>(256 * clamp(corrected.z(), 0.0, 0.999)) << '\n';
    }

    return out;
}

#endif