


/* (c) Greenhouse Software 2002
	Excuse: 
		how do we want this to work?
		1) minimize/control allocation of new space
		2) minimize copies-> pass pointers
		3) explicit calls-> no overloading/inlining.
			all functions explicitly say where they're 
			from

		mVec3 a, b, c;
		a.set(1,2,3);
		b.set(1,2,3);
		a.addEq(&b);  // a += b;
		c.eqAdd(&a, &b); // c = a + b
		
		  but this won't work. Oh, well.
		a.eqAdd(&(c.eqAdd(&a, &b)), 
	
	Used by:
	Using: 
	*/



#ifndef M_VEC3_TYPE
#define M_VEC3_TYPE

#include "stdDefines.h" 


class mVec3 {
public:
	real x, y, z;

	mVec3();
	virtual ~mVec3();
	mVec3(const real a, const real b, const real c);
	
	// These are self-explanatory
	void set(const int a, const int b, const int c);
	void set(const real a, const real b, const real c);
	void copy(mVec3 *it);
	
	/////////////////////////
	// ops not modifying this

	// res = dot product of this and it
	void dotWith(mVec3 *it, real *res);

	// res = distance between this and it
	void distanceFrom(mVec3 *it, real *res); 

	// res = distance squared between this and it
	void distanceFromsq(mVec3 *it, real *res); 

	// res = xyz length of this
	void length2(real *res);

	// res = xy length of this
	void length3(real *res);

	// res = xy squared length of this
	void length2sq(real *res);

	// res = xyz squared length of this
	void length3sq(real *res);

	//////////////////////
	// ops modifying this
		
	// Scale xyz of this by s
	void scale(const real s);

	// Set xyz of this to 0 (zero)
	void zero();

	// Negate xyz of this
	void negate();

	// Normalize this based on the xyz length of this
	void normalize(); 

	// Randomize xyz of this based on r
	void random(const real r); 

	//jitter the point by +-random(amount) in x/y/z
	void addNoise(const real amount);

	// given 2 vectors, set this

	// this = a + b;
	void eqAdd(mVec3 *a, mVec3 *b);

	// this = a - b
	void eqSub(mVec3 *a, mVec3 *b);

	// this = cross product of a and b
	void eqCross(mVec3 *a, mVec3 *b);

	// this = a * b
	void eqConv(mVec3 *a, mVec3 *b);

	// this = a scaled by b
	void eqScale(mVec3 *a, real b);

	// given 1 vector, set this

	// this = this + a
	void addEq(mVec3 *a);

	// this = this - a
	void subEq(mVec3 *a);

	// this = cross product of this and a
	void crossEq(mVec3 *a);

	// this = this * a
	void convEq(mVec3 *a);

	////////////
	// Rotations

	// rotate this about x-axis by angle r
	void rotateX(const real r);

	// rotate this about y-axis by angle r
	void rotateY(const real r);

	// rotate this about z-axis by angle r
	void rotateZ(const real r);
	
	// this = rotate v about x-axis by angle r
	void rotateX(mVec3 *v, const real r);

	// this = rotate v about y-axis by angle r
	void rotateY(mVec3 *v, const real r);

	// this = rotate v about z-axis by angle r
	void rotateZ(mVec3 *v, const real r);
};

#endif // M_VEC3_TYPE
