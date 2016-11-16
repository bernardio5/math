// Neal McDonald 2002



#ifndef M_VEC3_TYPE
#define M_VEC3_TYPE

#include "mReal.h"

class mVec3 {
public:
	real x, y, z;

	mVec3();
	virtual ~mVec3();

	void set(const real a, const real b, const real c);
	void copy(mVec3 *it);

	void dotWith(mVec3 *it, real *res);
	void length2(real *res);
	void length3(real *res);
	void normalize2();
	void normalize3();
	void scale(const real s);
	void eqAdd(mVec3 *a, mVec3 *b);
	void eqSub(mVec3 *a, mVec3 *b);
	void eqScale(mVec3 *a, real b);
	void addEq(mVec3 *a);
	void distanceFromsq(mVec3 *a, real *d); 
};

#endif

// Neal McDonald 1/1/2005

