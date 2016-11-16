 

/* (c) Greenhouse Software 2002
	All rights reserved, but who cares?

	Excuse: 
		Quaternions class. Mmmm-lovely.

	*/



#ifndef M_QUAT_TYPE
#define M_QUAT_TYPE

#include "mVec3.h" 
#include "mx4.h"

class mQuat {
public:
	real	w, x, y, z;

	mQuat();
	mQuat(const real win, const real xin, const real yin, const real zin);
//	mQuat(const real win, const real xin, const real yin, const real zin);
/*
	mQuat(const real win, mVec3 *axis);
	mQuat(const real win, mVec3Phys *axis);
*/
	virtual ~mQuat();

	void addEq(mQuat *it);		// this = this + it
	void copy(mQuat *it); 

	void setToIdentity();

	void set(const real w, mVec3 *p);
//	void set(const real w, mVec3Phys *p);
	void setAngle(const real theta, mVec3 *p);
//	void setAngle(const real theta, mVec3Phys *p);

	void normalize();			// normalize (x,y,z)
	
	void conjugate();			// this = conj(this)
//	void invert();				// this = inverse(this)
//	void exponentiate();		// this = e^this
//	void log();					// this = ln(this)

	void preMult(mQuat *it);	// this = this * it
	void postMult(mQuat *it);	// this = it * this
	void vecPreMult(mVec3 *vec);	//this = this * vec
	void vecPostMult(mVec3 *vec);	//this = vec * this

	void scale(real r);			//this = (w, x*r, y*r, z*r)
//	void scale(real r);		//this = (w, x*r, y*r, z*r)

	void setMx(mx4 *it);
//	void setMx(mx4Phys *it);
	void setFromMatrix(const mx4 *m);
//	void setFromMatrix(const mx4Phys &m);

	void slerp(mQuat *a, mQuat *b, const real t);

	void QVRotate(mVec3 *in, mVec3 *out); //out = this * v * ~this
private:
	void magnitude(real *mag);	// *result = sqrt(w*w + x*x + y*y + z*z);
};



#endif // M_QUAT_TYPE