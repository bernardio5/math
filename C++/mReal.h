

#ifndef M_REAL_TYPE
#define M_REAL_TYPE

#define NULL 0
typedef double real;
typedef unsigned char byte;

#define PI		(real)3.141592653589793238462643383
#define TWOPI	(real)6.283185307179586476925286767
#define HALFPI	(real)1.570796326794896619231321691

#define REAL_GAMMA 	(real)0.00000000001

void real_rand(real *x);
void in_rand(const int range, int *x);


bool real_eq(real a, real b);
bool real_gteq(real a, real b);
bool real_lteq(real a, real b);
bool real_gt(real a, real b);
bool real_lt(real a, real b);

#endif

// Neal McDonald 1/1/2005

