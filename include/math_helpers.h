#ifndef MATH_HELPERS_H
#define MATH_HELPERS_H

#include <stdbool.h>

#define EPSILON 1e-9
#define PHYSICS_PI 3.14159265358979323846

bool math_is_near_zero(double value);
bool math_is_negative(double value);
bool math_can_divide(double denominator);
bool math_can_sqrt(double value);

#endif
