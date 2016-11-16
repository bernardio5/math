// Neal McDonald 2002


#ifndef M_STAR_CLASS
#define M_STAR_CLASS

#include "math/mTri.h"
#include "math/mVert.h"

/* 	Yeah, not usng hStar for anything.
	Revise, upgrade, make better, copy from mPoly

	An mStar is a star-shaped mPoly.
	There is a point from which all vertices are visisble,
	the "center point"

	An mStar is also a cylindrical coordinate system.
	Inside/outside tests are easy in polars. I guess.

	An mStar will be used to make non-overlapping regions.
	An mStar will be expanded and shrunk.

	An mStar is triangulated!
	So it doesn't need mPoly's resources.
	*/



class mStar {

public:
	mStar();
	~mStar();

	void init(real cx, real cy);
	void addPointCart(real x, real y);
	void addPointCyl(real d, real r);
	void setR(int which, real r);
	void setA(int which, real a);
	void getR(int which, real *r);
	void getA(int which, real *a);

	void cyl2Cart(real d, real r, real *x, real *y);
	void cart2Cyl(real x, real y, real *d, real *r);
	void perim2Cart(real t, real *x, real *y);


	// check after init
	bool ready;

	void squeeze(real degree);
	void spin(real radians);
	void round(real degree);
	void smooth(real degree);
	void grow(real factor);
	void moveTo(real dx, real dy);

	// move me to get me off of other
	bool unoverlapWith(mStar *other, int steps, real gap);
	bool setWithin(mStar *other);
	void blend(mStar *a, mStar *b);

	bool inside(real x, real y);
	void moveToEdge(real *x, real *y);

	void getCenter(real *x, real *y);
	void getSides(int max, int *ct, real *xs, real *ys);

	// init it to be the nth triangle of the wedge
	bool getTri(int n, mTri *it);

private:
	real *as, *rs;
	void expand(int toWhat);

	void reset();
	int whichTri(real a);
	bool triParam(int which, real r, real ang,
		real *u, real *lr);

	real xc, yc, direction;
	int lastPt, maxPt;

};







#endif


// Neal McDonald 1/1/2005
