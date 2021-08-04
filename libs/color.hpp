#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>
#include "vec3.hpp"

inline void write_color(std::ostream &out , color pixel_color){
    out << static_cast<int>(255.99 * pixel_color.x()) << " "
        << static_cast<int>(255.99 * pixel_color.y()) << " "
        << static_cast<int>(255.99 * pixel_color.z()) << "\n";
}

#endif
