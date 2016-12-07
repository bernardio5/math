
/*
	Neal McDonald 2009

	Room sets classify all 3d points
	by projecting them onto the x-z plane 
	and doing a modulo on thier coordiantes. 

	The grid lies in the +x, +z quadrant of this plane. 

	The classification returns an integer.
	The box with a corner on the origin is room 0. 
	Room one lies next to it, along the x axis. 
	There are xres rooms in a row. 
	The room next to room 0 along the z axis is room "xres" 

	Attempts to classify points not falling onto this grid
	gets the result -1. 

	*/




#ifndef M_ROOMSET_CLASS
#define M_ROOMSET_CLASS


/////////////////////////////////
#include "mVec3.h"
/////////////////////////////////



class  mRoomSet {
public:
	real	xlow, zlow, xhigh, zhigh, xspan, zspan;
	int		xrez, zrez; 

	mRoomSet() {}
	~mRoomSet() {}
	void copy(mRoomSet *it); 
	
	// modify the BB to insure that it contains one more point
	void setup(	const real xlo, const real zlo, 
				const real xhi, const real zhi, 
				const int xrz, const int zrz); 

	// returns *zrow(p)*xrez) + zrow(p) or -1
	void index(mVec3 *p, int *result); 

	// These routines let you sort things into rows and columns. 
	// result is the room in which the point lies, or -1 if p lies in no room
	void xrow(mVec3 *p, int *result); 
	void zrow(mVec3 *p, int *result); 

	// get the rectangle of rooms that bounds a triangle. 
	// some waste here, but the alternative is scan-conversion. 
	void extents(mVec3 *p1, mVec3 *p2, mVec3 *p3, 
				 int *xlo, int *zlo, int *xhi, int *zhi); 
};




#endif // M_ROOMSET_CLASS

