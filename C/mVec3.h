


/* Neal McDonald 2009
 
 */



#ifndef M_VEC3_TYPE
#define M_VEC3_TYPE 1

#import "mReal.h" 


typedef struct {
	real x, y, z;
} mVec3; 


void mvInit(mVec3 *t, real a, real b, real c);

// These are self-explanatory
void mvSeti(mVec3 *t, int a, int b, int c);
void mvSetf(mVec3 *t, real a, real b, real c);
void mvCopy(mVec3 *t, mVec3 *it);

/////////////////////////
// ops not modifying this

// res = dot product of this and it
void mvDotWith(mVec3 *t, mVec3 *it, real *res);

// res = distance between this and it
void mvDistanceFrom(mVec3 *t, mVec3 *it, real *res); 

// res = distance squared between this and it
void mvDistanceFromsq(mVec3 *t, mVec3 *it, real *res); 

// res = xyz length of this
void mvLength2(mVec3 *t, real *res);

// res = xy length of this
void mvLength3(mVec3 *t, real *res);

// res = xy squared length of this
void mvLength2sq(mVec3 *t, real *res);

// res = xyz squared length of this
void mvLength3sq(mVec3 *t, real *res);

//////////////////////
// ops modifying this

// Scale xyz of this by s
void mvScale(mVec3 *t, const real s);

// Set xyz of this to 0 (zero)
void mvZero(mVec3 *t);

// Negate xyz of this
void mvNegate(mVec3 *t);

// Normalize this based on the xyz length of this
void mvNormalize(mVec3 *t); 

// Randomize xyz of this based on r
void mvRandom(mVec3 *t, real r); 

//jitter the point by +-random(amount) in x/y/z
void mvAddNoise(mVec3 *t, real amount);

// given 2 vectors, set this

// this = a + b;
void mvEqAdd(mVec3 *t, mVec3 *a, mVec3 *b);

// this = a - b
void mvEqSub(mVec3 *t, mVec3 *a, mVec3 *b);

// this = cross product of a and b
void mvEqCross(mVec3 *t, mVec3 *a, mVec3 *b);

// this = a * b
void mvEqConv(mVec3 *t, mVec3 *a, mVec3 *b);

// this = a scaled by b
void mvEqScale(mVec3 *t, mVec3 *a, real b);

// given 1 vector, set this

// this = this + a
void mvAddEq(mVec3 *t, mVec3 *a);

// this = this - a
void mvSubEq(mVec3 *t, mVec3 *a);

// this = cross product of this and a
void mvCrossEq(mVec3 *t, mVec3 *a);

// this = this * a
void mvConvEq(mVec3 *t, mVec3 *a);

////////////
// Rotations

// rotate this about x-axis by angle r
void mvRotateX(mVec3 *t,  real r);

// rotate this about y-axis by angle r
void mvRotateY(mVec3 *t,  real r);

// rotate this about z-axis by angle r
void mvRotateZ(mVec3 *t,  real r);
/*
// this = rotate v about x-axis by angle r
void mvRotateX(mVec3 *t, mVec3 *v, const real r);

// this = rotate v about y-axis by angle r
void mvRotateY(mVec3 *t, mVec3 *v, const real r);

// this = rotate v about z-axis by angle r
void mvRotateZ(mVec3 *t, mVec3 *v, const real r);
*/
#endif
