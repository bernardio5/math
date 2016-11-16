// Neal McDonald 2002


#ifndef MATH_SEGMENT_CLASS
#define MATH_SEGMENT_CLASS

#include "math/mReal.h"
#include <math.h>

/*
	mSegments represent line segments bounded by two points. 
	so, they don't curve. 

*/


class mSegment {
public:
	real x1, y1, x2, y2, dx, dy, length;

	mSegment() {}
	~mSegment() {}
	mSegment(real a1, real b1, real a2, real b2);

	void copy(mSegment *it);
	void set(real a1, real b1, real a2, real b2);

	// exchange end points
	void reverse();

	// given t, get xy
	void getPoint(real t, real *x, real *y);

	// given xy, return t of closest
	void closestPoint(real x1, real y1, real *t, real *distance);

	bool pointOn(real x, real y);

	// move p1 or p2 along segment line
	void move1(real t);
	void move2(real t);

	// if keepLowEnd, move2(t), else move1(t)
	void trim(real t, bool keepLowEnd);

	// cut segment in two, return new segment
	void split(real t, mSegment *it);

	// for xy on seg line, return whether it's
	// outside the segment
	bool outside(real x, real y);

	// return t if the two intersect.
	// t1 is point on this segment, t2 on it
	bool intersect(mSegment *it, real *t1, real *t2);

	
	int above(real x, real y);

};






#endif


// Neal McDonald 1/1/2005
