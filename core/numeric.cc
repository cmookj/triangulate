#include "core/numeric.h"

#include <cmath>

//// NAMESPACE: numeric
namespace numeric {

// Determine whether two scalars are within the error tolerance (eps)
bool
close_enough (const double a, const double b, const double eps) {
    return std::abs (a - b) < eps;
}

}  // namespace numeric
