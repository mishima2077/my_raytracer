#pragma once

#include "color.h"

// Forward declerations
class ray;
class hit_record;

// Material interface
class material {
  public:
    virtual ~material() = default;

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const = 0;
};

class lambertian: public material {
  public:
    lambertian(const color& albedo): albedo(albedo) {} 

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

  private:
    color albedo;
};

class metal: public material {
  public:
    metal(const color& albedo, double fuzz): albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

  private:
    color albedo;
    double fuzz;
};

class dielectric: public material {
  public:
    dielectric(double refraction_index): refraction_index(refraction_index) {}
    
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

  private:
    // Refractive index in vacuum/air, or the ratio of materials refractive index
    // over the refractive index of the enclosing material.
    double refraction_index;

    static double reflectance(double cosine, double refraction_index);
};
