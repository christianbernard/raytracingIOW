#include<iostream>
#include<fstream>
#include<ctime>

#include "sphere.hpp"
#include "hittable_list.hpp"
#include "camera.hpp"
#include "color.hpp"


color ray_color(const ray& r, const hittable &world, int depth) {
    hit_record rec; 

    if(depth <= 0) {
        return color(0, 0, 0);
    }
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
            return attenuation*ray_color(scattered, world, depth-1);
        } 
        return color(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.direction());

    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0-t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main(){
    clock_t start = clock();    
    std::ofstream img_file("ray.ppm", std::ofstream::out);

    const double aspect_ratio = 16.0 / 9.0;
    const int img_width = 600; 
    const int img_height = static_cast<int>(img_width / aspect_ratio); 
    const int samples_per_pixel = 100;
    const int max_depth = 50;
    // Camera
    camera cam;

    // World
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Rendering here
    img_file<<"P3\n"<< img_width << " " << img_height<<"\n255\n";

    for(int i = img_height-1; i >= 0; i--){
        std::cerr << "\rScanlines remaining: " << i << " " << std::flush;
        for(int j = 0; j < img_width; j++){
            color pixel_color(0, 0, 0);
            for(int s = 0; s < samples_per_pixel; s++){
                auto u = (j + random_double()) /(img_width-1);
                auto v = (i + random_double()) / (img_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }    
            write_color(img_file, pixel_color, samples_per_pixel);
        }
    }               
    clock_t end = clock();

    double total = (end - start)*1.0/CLOCKS_PER_SEC;
    std::cerr << "\nTotal time:" << total <<  "s. Done.\n";
    
    return 0;
}
