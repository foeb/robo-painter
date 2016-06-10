/* A small header file for making dealing with floating-point numbers a bit
 * more sane. C99+ required. */

#ifndef KOSHER_HEADER
#define KOSHER_HEADER

#include <math.h>

/* NOT_KOSHER: is 0 if x is NaN or Inf */
#define NOT_KOSHER(x) (isnan(x) || isinf(x))
#define KOSHER(x) (!NOT_KOSHER(x))

/* MAKE_KOSHER: set x to 1.0 if x is NaN or Inf */
#define MAKE_KOSHER(x) (NOT_KOSHER(x) ? (x) = 1 : (x))

#endif

