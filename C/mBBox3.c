

// Neal McDonald 2009
	


#include "mBBox3.h"



void mBB3Copy(mBBox3 *it) { 
	xlo = it->xlo; 
	xhi = it->xhi; 
	ylo = it->ylo; 
	yhi = it->yhi; 
	zlo = it->zlo; 
	zhi = it->zhi; 
}


void mBB3SetP1P2(mVec3 *p1, mVec3 *p2) {
	if (p1->x < p2->x) { xlo = p1->x; xhi = p2->x; }
	else { xlo = p2->x; xhi = p1->x; }

	if (p1->y < p2->y) { ylo = p1->y; yhi = p2->y; }
	else { ylo = p2->y; yhi = p1->y; }

	if (p1->z < p2->z) { zlo = p1->z; zhi = p2->z; }
	else { zlo = p2->z; zhi = p1->z; }
}




bool mBB3Touches(mBBox3 *it) {
	if (xlo>it->xhi) { return false; }
	if (xhi<it->xlo) { return false; }
	if (ylo>it->yhi) { return false; }
	if (yhi<it->ylo) { return false; }
	if (zlo>it->zhi) { return false; }
	if (zhi<it->zlo) { return false; }
	return true; 
}


bool mBB3Contains(mVec3 *data) {
	if (xlo>data->x) { return false; }
	if (xhi<data->x) { return false; }
	if (ylo>data->y) { return false; }
	if (yhi<data->y) { return false; }
	if (zlo>data->z) { return false; }
	if (zhi<data->z) { return false; }
	return true; 
}


bool mBB3ContainsAll(const int ptCt, mVec3 *data) {
	int i; 

	for (i=0; i<ptCt; ++i) {
		if (!contains(&(data[i]))) { return false; }
	}
	return true; 
}


bool mBB3ContainsAny(const int ptCt, mVec3 *data) {
	int i; 

	for (i=0; i<ptCt; ++i) {
		if (contains(&(data[i]))) { return true; }
	}
	return false; 

}
