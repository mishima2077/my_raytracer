#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

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

    // World
    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));
    
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 100;

    cam.render(output_file, world);

    output_file.close();
    return 0;
}
