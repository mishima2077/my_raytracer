#pragma once

#include <cstdlib>
#include <limits>
#include <random>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Helper utils

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Returns a random real number in [0,1).
inline double random_double() {
    thread_local std::mt19937 gen(42); 
    thread_local std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(gen);
}

// Returns a random real number in [min, max).
inline double random_double(double min, double max) {
    return (max-min)*random_double() + min;
}
