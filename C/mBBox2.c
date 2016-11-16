

// Neal McDonald 2009
	


#include "mBBox2.h"


void mBB2Copy(mBBox2 *it) { 
	xlo = it->xlo; 
	xhi = it->xhi; 
	zlo = it->zlo; 
	zhi = it->zhi; 
}


void mBB2AddPt(mVec3 *data) {
	if (xlo>data->x) { xlo = data->x; } 
	if (xhi<data->x) { xhi = data->x; } 
	if (zlo>data->z) { zlo = data->z; } 
	if (zhi<data->z) { zhi = data->z; } 
}


bool mBB2Touches(mBBox2 *it) {
	if (xlo>it->xhi) { return false; }
	if (xhi<it->xlo) { return false; }
	if (zlo>it->zhi) { return false; }
	if (zhi<it->zlo) { return false; }
	return true; 
}


bool mBB2Contains(mVec3 *data) {
	if (xlo>data->x) { return false; }
	if (xhi<data->x) { return false; }
	if (zlo>data->z) { return false; }
	if (zhi<data->z) { return false; }
	return true; 
}


bool mBB2ContainsAll(const int ptCt, mVec3 *data) {
	int i; 

	for (i=0; i<ptCt; ++i) {
		if (!mBB2ontains(&(data[i]))) { return false; }
	}
	return true; 
}


bool mBB2ContainsAny(const int ptCt, mVec3 *data) {
	int i; 

	for (i=0; i<ptCt; ++i) {
		if (mBB2Contains(&(data[i]))) { return true; }
	}
	return false; 

}
