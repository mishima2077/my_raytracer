#ifndef RTUTIL_H
#define RTUTIL_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Helper utils

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Common headers

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif // !RTUTIL_H
