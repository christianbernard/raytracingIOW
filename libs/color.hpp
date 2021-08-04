#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>
#include "rtweekend.hpp"

inline void write_color(std::ostream &out , color pixel_color, int samples_per_pixel){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    
    out << static_cast<int>(256 * cclamp(r, 0, 0.999)) << " "
        << static_cast<int>(256 * cclamp(g, 0, 0.999)) << " "
        << static_cast<int>(256 * cclamp(b, 0, 0.999)) << "\n";
}

#endif
