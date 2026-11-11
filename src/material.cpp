#include "material.h"
#include "hittable.h"

#include <cmath>

// Diffuse material
bool lambertian::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
    auto scatter_direction = rec.normal + random_unit_vector();

    // Catch bad scatter directions
    if (scatter_direction.near_zero()) 
        scatter_direction = rec.normal;

    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
}

// Metal material
bool metal::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
    auto reflected = reflect(r_in.direction(), rec.normal);
    reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
    scattered = ray(rec.p, reflected);
    attenuation = albedo;
    return (dot(reflected, rec.normal) > 0);
}

// Dielectric material
bool dielectric::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
    attenuation = color(1.0, 1.0, 1.0);
    double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

    vec3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

    vec3 direction;

    // Can't refract, must reflect
    // Schlick approximation is also included
    if (ri * sin_theta > 1.0 || reflectance(cos_theta, ri) > random_double()) { 
        direction = reflect(unit_direction, rec.normal);
    } else { // Can refract, will refract
        direction = refract(unit_direction, rec.normal, ri);
    }

    scattered = ray(rec.p, direction);
    return true;
}

// Using polynomial approximation by Christophe Schlick
// Lookat: Schlick's approximation for reflectance
double dielectric::reflectance(double cosine, double refraction_index) {
    auto r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 *= r0;
    return r0 + (1-r0)*std::pow((1 - cosine), 5);
}
