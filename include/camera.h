#pragma once

#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

class hittable;

class camera {
  public:
    double aspect_ratio      = 1.0;  // Ratio of width over height
    int    image_width       = 100;  // Rendered image width
    int    samples_per_pixel = 10;   // Number of random samples taken for each pixel
    int    max_depth         = 10;   // Maximum number of ray bounces in the scene

    double vfov     = 90;                // Vertical view angle (field of view)
    point3 lookfrom = point3(0, 0, 0);   // Point camera is looking from
    point3 lookat   = point3(0, 0, -1);  // Point camera is looking at
    vec3   vup      = vec3(0, 1, 0);     // Camera up direction
    
    double defocus_angle = 0;   // Variation angle of rays through each pixel
    double focus_dist = 10;     // Distance from camera lookfrom point to plane of perfect focus

    void render(std::ostream& output_file, const hittable& world);

  private:
    int image_height;           // Rendered image height
    point3 center;              // Position of the camera
    point3 pixel00_loc;         // Location of pixel 0,0     
    vec3 pixel_delta_u;         // Offset to pixel to the right
    vec3 pixel_delta_v;         // Offset to the pixel below
    double pixel_samples_scale; // Color scale factor for a sum of pixel samples
    vec3 u, v, w;               // Camera frame basis vectors
    vec3 defocus_disk_u;        // Defocus disk horizontal radius
    vec3 defocus_disk_v;        // Defocus disk vertical radius

    void initialize();

    ray get_ray(int i, int j) const;

    vec3 sample_square() const;

    point3 defocus_disk_sample() const;

    color ray_color(const ray& r, int depth, const hittable& world) const;
};
