#include <gtest/gtest.h>

#include "core/geometry.h"
#include "core/numeric.h"
#include "test/random_gen.h"

//// Test Numeric
TEST (BasicTest, Numeric) { ASSERT_TRUE (numeric::close_enough (0.0, 0.0)); }

//// Test Geometry
