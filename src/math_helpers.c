#include "math_helpers.h"

bool math_is_near_zero(double value) {
    return value > -EPSILON && value < EPSILON;
}

bool math_is_negative(double value) {
    return value < -EPSILON;
}

bool math_can_divide(double denominator) {
    return !math_is_near_zero(denominator);
}

bool math_can_sqrt(double value) {
    return !math_is_negative(value);
}
