#include <cmath>
#include <limits>
#include <numbers>
#include <random>

#include "core/types.h"

template <typename T> struct random_int_gen {
    // By default, random integral numbers are generated between the minimum
    // and maximum possible values.
    random_int_gen (
        const T lower = std::numeric_limits<T>::min(),
        const T upper = std::numeric_limits<T>::max()
    ) {
        // Create a random device to seed the generator
        std::random_device rd;

        // Use the Mersenne Twister engine
        gen = std::mt19937 (rd());

        // Define distributions
        dist = std::uniform_int_distribution<T> (lower, upper);
    }

    T
    operator() () {
        return dist (gen);
    }

    std::mt19937                     gen;
    std::uniform_int_distribution<T> dist;
};

template <typename T> struct random_float_gen {
    // By default, random floating point numbers are generated between 0.0 and 1.0
    random_float_gen (const T lower = T (0.), const T upper = T (1.)) {
        // Create a random device to seed the generator
        std::random_device rd;

        // Use the Mersenne Twister engine
        gen = std::mt19937 (rd());

        // Define distributions
        if constexpr (!std::is_floating_point_v<T>)
            throw std::runtime_error ("The template parameter should be floating_point");

        dist = std::uniform_real_distribution<T> (lower, upper);
    }

    T
    operator() () {
        return dist (gen);
    }

    std::mt19937                      gen;
    std::uniform_real_distribution<T> dist;
};

struct random_point_around_semicircle {
    random_point_around_semicircle (const double angle = 0.0)
        : base_angle{angle} {}

    Point
    operator() () {
        const double angle = gen() * std::numbers::pi + base_angle;
        return Point{.x = std::cos (angle), .y = std::sin (angle)};
    }

    random_float_gen<double> gen;
    const double             base_angle;
};
