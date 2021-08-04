#include<iostream>
#include<fstream>
#include "vec3.hpp"
#include "color.hpp"
#include "ray.hpp"

double hit_sphere(const point3 &center, double radius, const ray &r){
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;

    auto discriminant = b*b -4*a*c;

    if(discriminant < 0){
        return -1.0;
    }
    else {
        return (-b - sqrt(discriminant)) / (2.0*a);
    }

}
color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if(t > 0.0){
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(1.0, 0.7, 0.5);
}

int main(){
    
    std::ofstream img_file("ray.ppm", std::ofstream::out);
    
    // Image here
    const auto aspect_ratio = 16.0/9.0;
    const int img_width = 1000; 
    const int img_height = static_cast<int>(img_width / aspect_ratio); 

    // Camera here
    
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0 ,0, focal_length);

    // Rendering here
    img_file<<"P3\n"<< img_width << " " << img_height<<"\n255\n";

    for(int i = img_height-1; i >= 0; i--){
        std::cerr << "\rScanlines remaining: " << i << " " << std::flush;
        for(int j = 0; j < img_width; j++){
            auto u = double(j) / (img_width-1); 
            auto v = double(i) / (img_height-1); 
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            
            color pixel_color = ray_color(r);
            write_color(img_file, pixel_color);
        }
    }               
    std::cerr << "\nDone.\n";
    return 0;
}