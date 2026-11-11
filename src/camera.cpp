#include "camera.h"
#include "interval.h"
#include "ray.h"
#include "hittable.h"
#include "material.h"
#include <cmath>
#include <iostream>

void camera::render(std::ostream& output_file, const hittable& world) {
    initialize();

    output_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                ray r = get_ray(i, j);

                // ??
                pixel_color += ray_color(r, max_depth, world);
            }

            write_color(output_file, pixel_samples_scale * pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}


void camera::initialize() {
    // Calculate image height
    image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    pixel_samples_scale = 1.0 / samples_per_pixel;

    center = lookfrom;

    auto theta = degrees_to_radians(vfov);
    auto h = std::tan(theta/2) * focus_dist;
    auto viewport_height = 2 * h;
    auto viewport_width = viewport_height * (double(image_width) / image_height);

    // Calculate camera basis vectors
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    // Calculate viewport vectors
    auto viewport_u = viewport_width * u;
    auto viewport_v = viewport_height * -v;

    // Calculate pixel deltas
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Calculate upper left pixel location
    auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Calculate camera defocus disk basis vectors
    auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
}


ray camera::get_ray(int i, int j) const {
    // Construct a ray origination from the defocus disk and directed at a 
    // randomly  sampled point around the pixel location i, j.

    auto offset = sample_square();
    auto pixel_sample = pixel00_loc
                      + ((i + offset.x()) * pixel_delta_u)
                      + ((j + offset.y()) * pixel_delta_v);

    // Ray from center to random sample
    auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
}

vec3 camera::sample_square() const {
    // Returns the vector to a random point in the [-.5, -.5] - [.5, .5] unit square
    return vec3(random_double() - 0.5, random_double() - 0.5, 0.0);
}

point3 camera::defocus_disk_sample() const {
    // Returns a random point in the camera defocus disk.
    auto p = random_in_unit_disk();
    return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}

color camera::ray_color(const ray& r, int depth, const hittable& world) const {
    if (depth <= 0)
        return color(0, 0, 0);

    hit_record rec;
    // 0.001 is given to fix shadow acne problem
    if (world.hit(r, interval(0.001, infinity), rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered)) 
            return attenuation * ray_color(scattered, depth-1, world);
        return color(0, 0, 0);
    }

    auto unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}
