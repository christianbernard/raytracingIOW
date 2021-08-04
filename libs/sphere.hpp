#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "material.hpp"
#include "vec3.hpp"


class sphere : public hittable {
    public:
        sphere() {}
        sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};

        virtual bool hit(
            const ray &r, double t_min, double t_max, hit_record &rec) const override;
    public:
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if(discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    auto root = (-half_b - sqrtd) / a;
    if(root < t_min || t_max < root){
        root =  (-half_b + sqrtd) / a;
        if(root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

class lambertian : public material {
    public:
        lambertian(const color &a) : albedo(a) {}

        virtual bool scatter(
            const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
        ) const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            if(scatter_direction.near_zero())
                scatter_direction = rec.normal;
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
    public: 
        color albedo;
};

#endif