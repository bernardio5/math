

/* Copyright (C) 2002 Greenhouse Software.  All Rights Reserved.
	see mRoomSet.h for intro information about this code. 
	*/


#include "mRoomSet.h"
#include <cmath>

void mRoomSet::copy(mRoomSet *it) {
	xlow = it->xlow; 
	zlow = it->zlow; 
	xhigh = it->xhigh; 
	zhigh = it->zhigh; 
	xspan = it->xspan; 
	zspan = it->zspan; 
	xrez = it->xrez; 
	zrez = it->zrez; 
}


void mRoomSet::setup(
		const real xlo, const real zlo, 
		const real xhi, const real zhi, 
		const int xrz, const int zrz) {
	xlow = xlo; 
	zlow = zlo; 
	xhigh = xhi; 
	zhigh = zhi; 

	xrez = xrz; 
	zrez = zrz;

	xspan = (xhi-xlo)/xrez; 
	zspan = (zhi-zlo)/zrez;
}


void mRoomSet::index(mVec3 *p, int *result) {
	int xIndex, zIndex;

	xrow(p, &xIndex); 
	zrow(p, &zIndex); 
	if ((xIndex>-1) && (zIndex>-1)) {
		*result = (xIndex * xrez) + zIndex; 	
	} else {
		*result = -1; 
	}
}

void mRoomSet::xrow(mVec3 *p, int *result)
{
	if ((p->x<xlow) || (p->x>xhigh))
	{
		*result = -1;
	}
	else
	{
		*result = (int)floor(((p->x)-xlow)/xspan);
	}
}


void mRoomSet::zrow(mVec3 *p, int *result)
{
	if ((p->z<zlow) || (p->z>zhigh))
	{
		*result = -1;
	}
	else
	{
		*result = (int)floor(((p->z)-zlow)/zspan);
	}
}


// given a triangle and a roomSet, determine the x and z ranges of the triangle
void mRoomSet::extents(mVec3 *p1, mVec3 *p2, mVec3 *p3, 
				 int *xlo, int *zlo, int *xhi, int *zhi) { 
	// forgive the y-z conflation; 
	// i'm an old man, and my hand types y after x, not z. 
	int xi[3], yi[3], xmin, ymin, xmax, ymax, i; 

	*xlo = -1; 

	// get room indices (x and z) for the triangle.
	xrow(p1, &(xi[0])); 
	xrow(p2, &(xi[1])); 
	xrow(p3, &(xi[2])); 

	zrow(p1, &(yi[0])); 
	zrow(p2, &(yi[1])); 
	zrow(p3, &(yi[2])); 	

	// get the bounding box of the triangle
	// watch out for -1 entries. a lot. 
	xmin = xi[0]; 
	ymin = yi[0]; 
	xmax = xi[0]; 
	ymax = yi[0]; 
	for (i=1; i<3; ++i) {
		if (xi[i]<xmin) { xmin = xi[i]; } 
		if (xi[i]>xmax) { xmax = xi[i]; }
		if (yi[i]<ymin) { ymin = yi[i]; } 
		if (yi[i]>ymax) { ymax = yi[i]; }
	}

	*xlo = xmin; 
	*zlo = ymin; 
	*xhi = xmax; 
	*zhi = ymax; 
}