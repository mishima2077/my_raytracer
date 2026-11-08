#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "interval.h"
#include <cmath>
#include <cstdint>
#include <memory>

class sphere: public hittable {
public:
    sphere(const point3& center, double radius, std::shared_ptr<material> mat): 
        center(center), radius(std::fmax(0, radius)), mat(mat) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        auto center_vector = (center - r.origin());
        auto a = r.direction().length_squared();
        // Simplified for b = -2h
        auto h = dot(r.direction(), center_vector);
        auto c = center_vector.length_squared() - radius * radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);
        auto root = (h - sqrtd) / a;

        // If the smaller hit is not within desired range, try with the other
        if (!ray_t.surrounds(root)) {
            root  = (h + sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(root);
        auto outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    } 
private:
    point3 center;
    double radius;
    std::shared_ptr<material> mat;
};

#endif // !SPHERE_H
