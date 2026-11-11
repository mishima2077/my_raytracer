#pragma once

#include "hittable.h"
#include "interval.h"

class sphere: public hittable {
public:
    sphere(const point3& center, double radius, std::shared_ptr<material> mat);

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

private:
    point3 center;
    double radius;
    std::shared_ptr<material> mat;
};
