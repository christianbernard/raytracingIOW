#include<iostream>
#include<fstream>

#include "rtweekend.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"

color ray_color(const ray& r, const hittable &world) {
    hit_record rec; 

    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }

    vec3 unit_direction = unit_vector(r.direction());

    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0-t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main(){
    
    std::ofstream img_file("ray.ppm", std::ofstream::out);

    // Image here
    const auto aspect_ratio = 16.0/9.0; // width/height
    const int img_width = 600; 
    const int img_height = static_cast<int>(img_width / aspect_ratio); 

    // Scene here
    auto viewport_height = 2.0; // height of the scene
    auto viewport_width = aspect_ratio * viewport_height; // width of  the scene
    auto focal_length = 1.0; // distance from the camera to the 'scene'

    
    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);

    //from the center of the scene, walk half the size of its width and height to end up on the lower left corner
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0 ,0, focal_length); 

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(-viewport_width/2, 0.25, -2.0), 0.5));
    world.add(make_shared<sphere>(point3(viewport_width/2, 0.25, -2.0), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
    
    // Rendering here
    img_file<<"P3\n"<< img_width << " " << img_height<<"\n255\n";

    for(int i = img_height-1; i >= 0; i--){
        std::cerr << "\rScanlines remaining: " << i << " " << std::flush;
        for(int j = 0; j < img_width; j++){
            auto u = double(j) / (img_width-1); 
            auto v = double(i) / (img_height-1); 
            // moving the ray of light from left to right, bottom to top 
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            
            // get a pixel color based on the current ray and write it 
            color pixel_color = ray_color(r, world);
            write_color(img_file, pixel_color);
        }
    }               

    std::cerr << "\nDone.\n";
    return 0;
}
