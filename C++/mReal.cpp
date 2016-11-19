// Neal McDonald 2009


#include "mReal.h"
#include <stdlib.h>
#include <math.h>



void real_rand(real *x) {
	*x = ((real)rand()) / RAND_MAX;
}

void in_rand(const int range, int *x) {
	real d;
	real_rand(&d);
	*x = (int)(floor(range * d));
}




bool real_eq(real a, real b) {
	if (((a-b)<REAL_GAMMA)&&
		((b-a)<REAL_GAMMA)) {
		return true;
	}
	return false;
}

bool real_gteq(real a, real b) {
	if ((a-REAL_GAMMA)>=b) { return true; }
	return false;
}

bool real_lteq(real a, real b) {
	if ((a+REAL_GAMMA)<=b) { return true; }
	return false;
}

bool real_gt(real a, real b) {
	if ((a-REAL_GAMMA)>b) { return true; }
	return false;
}

bool real_lt(real a, real b) {
	if ((a+REAL_GAMMA)<b) { return true; }
	return false;
}
