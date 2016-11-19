// Neal McDonald 2009


#ifndef M_TRI_CLASS
#define M_TRI_CLASS

#include "math/mVec3.h"
#include "math/mSegment.h"

class mTri {

public:
	mTri() {}
	~mTri() {}

	// initialize cost=3 sqrts; returns false for degens
	bool init(real *x, real *y);
	bool init(mVec3 *t1, mVec3 *t2, mVec3 *t3);

	void copy(mTri *it);

	real area(real *xs, real *ys);
	real area2(real *xs, real *ys);

	// callable only after using "init"
	// returns true if more than a point is in the triangle
	bool clip(mSegment *in, mSegment *out);

	// should return char: 0->outside 1->corner 2->on line 3->inside
	char inside(real x, real y);

	// check after init
	bool degenerate;

	// a triangle defines a coord plane.
	void getParam(real x, real y, real *u, real *v); 
	void getPoint(real u, real v, real *x, real *y);
	// (texture mapping applications)

	real xs[3], ys[3];
	mSegment segs[3];
private:
	bool flipped; 
	void front(mSegment *in, mSegment *out);
	void back(mSegment *in, mSegment *out);
	bool outsider(mSegment *in, mSegment *out);

};


#endif


// Neal McDonald 1/1/2005
