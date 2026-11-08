#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"
#include "interval.h"

class material;

// p: point of the hit
// normal: normal vector from center of the object to hit point
// t: the t where the ray hits the point
class hit_record {
public:
    point3 p;
    vec3 normal;
    std::shared_ptr<material> mat;
    double t;
    bool front_face;

    // Normal is always the opposite direction of the ray
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record for normal vector and front_face.
        // outward_normal is assumed to have unit_length

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

// What is this?
class hittable {
public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif // !HITTABLE_H
