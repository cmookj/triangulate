//
// numeric.h
//
// Utility functions for floating-point calculations
//

#ifndef __NUMERIC_H__
#define __NUMERIC_H__

//// NAMESPACE: numeric
namespace numeric {

// Determine whether two scalars are within the error tolerance (eps)
bool close_enough(const double a, const double b, const double eps = 1e-12);

}  // namespace numeric

#endif
