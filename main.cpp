#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

double hit_sphere(const point3& center, double radius, const ray& r) {
    auto center_vector = (center - r.origin());
    auto a = r.direction().length_squared();
    // Simplified for b = -2h
    auto h = dot(r.direction(), center_vector);
    auto c = center_vector.length_squared() - radius * radius;

    auto discriminant = h*h - a*c;

    if (discriminant < 0.0) {
        return -1.0;
    } else {
        // Currently even the spheres behind the camera returns a hit,
        // we will address that issue later, for now we just take the closest hit
        return (h - std::sqrt(discriminant)) / a;
    }
}

color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0, 0, -1.0), 0.5, r);
    if (t > 0.0) {
        // Normal vector from the center of the sphere to hit point
        vec3 N = unit_vector(r.at(t) - vec3(0.0, 0.0, -1.0));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }

    auto unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int main(int argc, char *argv[]) {

    // TODO: Improve here, currently just ".ppm" is accepted as a filename
    std::string filename;
    if (argc == 1) {
        filename = "image.ppm";
    } else {
        std::string candidate_file = argv[1];
        if (candidate_file.length() >= 4 && candidate_file.rfind(".ppm") == candidate_file.length() - 4)
            filename = candidate_file;
        else {
            std::cerr << "Provided output file must have the extension .ppm !" << std::endl;
            return 1;
        }
    }

    std::ofstream output_file(filename);
    if (!output_file.is_open()) {
        std::cerr << "Failed to open the file: " << filename << std::endl;
        return 1;
    }

    // Image

    // Set image width and height

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 1400;
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = point3(0, 0, 0);

    // Calculate viewport vectors
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate pixel deltas
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate upper left pixel location
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    output_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(output_file, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
    output_file.close();
    return 0;
}
