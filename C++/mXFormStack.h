// Neal McDonald 2002



#ifndef M_XFORM_TYPE
#define M_XFORM_TYPE

#include "mVec3.h"
#include "mx4.h"






class mXFormStack {
public:

	mXFormStack();
	~mXFormStack();
	void copy(mXFormStack *it);

	void push();
	void translate(real x, real y);
	void rotate(real deg);
	void scale(real x, real y);
	void pop();

	void map(real xin, real yin, real *xout, real *yout);

private:
	mx4 *xforms;
	real tx, ty, r, sx, sy, sc; 
	int xformTop, xformSz;

	void expandXforms();
	void updateXform(); 

};

#endif

// Neal McDonald 1/1/2005