#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "interval.h"
#include "rtutil.h"
#include "vec3.h"
#include "material.h"

class camera {
  public:
    double aspect_ratio      = 1.0;  // Ratio of width over height
    int    image_width       = 100;  // Rendered image width
    int    samples_per_pixel = 10;   // Number of random samples taken for each pixel
    int    max_depth         = 10;   // Maximum number of ray bounces in the scene

    void render(std::ostream& output_file, const hittable& world) {
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

  private:
    int image_height;           // Rendered image height
    point3 center;              // Position of the camera
    point3 pixel00_loc;         // Location of pixel 0,0     
    vec3 pixel_delta_u;         // Offset to pixel to the right
    vec3 pixel_delta_v;         // Offset to the pixel below
    double pixel_samples_scale; // Color scale factor for a sum of pixel samples

    void initialize() {
        // Calculate image height
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = point3(0, 0, 0);

        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        // Calculate viewport vectors
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate pixel deltas
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate upper left pixel location
        auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    ray get_ray(int i, int j) const {
        // Construct and send a ray from the origin and directed to random point
        // centered around the pixel location i, j.

        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
                          + ((i + offset.x()) * pixel_delta_u)
                          + ((j + offset.y()) * pixel_delta_v);

        // Ray from center to random sample
        return ray(center, pixel_sample - center);
    }

    vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5, -.5] - [.5, .5] unit square
        return vec3(random_double() - 0.5, random_double() - 0.5, 0.0);
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
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
};

#endif // !CAMERA_H
