/* Neal McDonald 2002 
  */

#include "mReal.h"

#include <stdlib.h>
#include <math.h>



void real_rand(real *x) { 
	*x = (random()%100000000)/100000000.0;
}

void in_rand(const int range, int *x) {
	*x = random()%range;
}


real fract(real x) {
    return x-floor(x); 
}
