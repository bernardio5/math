
#ifndef MATH_MULTI_SEGMENT_CLASS
#define MATH_MULTI_SEGMENT_CLASS

#include "math/mReal.h"
#include <math.h>

/*// Neal McDonald 2002

	multiSegments are lists of points. 

	they will provide parametric coords, 
	like mLongPath, but discrete
	
	operations on them: straighten, align
	couple of generate-from-X
	
	
*/


class mMultiSeg {
public:
	real *xs, *ys, vxCount, vxSpace;
	
	mMultiSeg();
	~mMultiSeg();
	mMultiSeg(int count, real *xs, real *ys);

	void copy(mMultiSeg *it);
	
	void reverse();
	void expand(int toWhat); 
	void appendPoint(real x, real y); 
	void deletePoint(int which); 


	// given t, get xy
	void getPoint(real t, real *x, real *y);

	// given xy, return t of closest
	void closestPoint(real x1, real y1, real *t, real *distance);

	bool pointOn(real x, real y);

	// if keepLowEnd, discard all before t, ow, after
	void trim(real t, bool keepLowEnd);

	// cut segment in two, return new segment
	void split(real t, mSegment *it);

	// for xy on seg line, return whether it's
	// outside the segment
	bool outside(real x, real y);

	// return t if the two intersect.
	// t1 is point on this segment, t2 on it
	bool intersect(mSegment *it, real *t1, real *t2);
	bool intersect(mMultiSeg *it, real *t1, real *t2);


	// infinite, the number of ways to straighten a line
	// strA moves each point towards 
	// the midpoint of the segment connecting prev and next.
	// 0<howMuch<1
	void strA(real howMuch); 
	// working from the middle segment, move all points toward
	// the line through that segment
	void strB(real howMuch) {}
	// make a line through the first, middle and last points, 
	// move points toward that line.
	void strC(real howMuch) {}
	// use n points, make best-fit line through them, move
	// those points toward the line
	void strD(int howMany, real howMuch) {}
	// comvert to mLongPath, then use that to resample n samples
	void strE(int howMany) {}
	// 
	
	
	// deletes points that are less than howMuch from the
	// line connecting prev and next
	void cull(real howMuch); 
	
	// 
	void bendToSegment(mSegment it, real howMuch); 
	void bendToCircle(real cx, real cy, real r, real howMuch); 
	void bendToMultiSegment(mSegment it, real howMuch); 
	
	void translate(real xd, real dy); 
	void rotate(real x, real y, real d); 
	void scale(real x, real y, real s); 
	
	// generation
	void fromCircle(int howMany, real x, real y, real r); 
	void fromStar(int howMany, mStar *it); 
	void fromSegment(int howMany, mSegment *it); 
	void fromLongPath(int howMany, mLongPath *it); 
	void fromArc(); // got bored
	
	// 
	
	// conversion
	void toLongPath(mLongPath *it); 
	
	
};






#endif


// Neal McDonald 1/1/2005
