#ifndef RTUTIL_H
#define RTUTIL_H

#include <cstdlib>
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

// Returns a random real number in [0,1).
inline double random_double() {
    return std::rand() / (RAND_MAX + 1.0);
}

// Returns a random real number in [min, max).
inline double random_double(double min, double max) {
    return (max-min)*random_double() + min;
}

#endif // !RTUTIL_H
