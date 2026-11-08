#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <string>


int main(int argc, char *argv[]) {

    // TODO: Improve here, assert given filename has no extension maybe?
    std::string filename;
    if (argc == 1) {
        filename = "image.ppm";
    } else {
        filename = argv[1];
        filename.append(".ppm");
    }

    std::ofstream output_file(filename);
    if (!output_file.is_open()) {
        std::cerr << "Failed to open the file: " << filename << std::endl;
        return 1;
    }

    // Material initialization
    auto ground_mat = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto center_mat = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));

    // Glass ball with air in it
    auto left_mat   = std::make_shared<dielectric>(1.50);
    auto bubble_mat = std::make_shared<dielectric>(1.00 / 1.50);

    auto right_mat  = std::make_shared<metal>(color(0.8, 0.2, 0.1), 1.0);

    // World
    hittable_list world;
    world.add(std::make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, ground_mat));
    world.add(std::make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, center_mat));
    world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, left_mat));
    world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, bubble_mat));
    world.add(std::make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, right_mat));
    
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1000;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(output_file, world);

    output_file.close();
    return 0;
}
