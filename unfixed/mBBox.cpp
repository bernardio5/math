

// Neal McDonald 2009
	


#include "mBBox.h"


mBBox2::mBBox2() {
	xlo=M_ZERO; 
	xhi=M_ZERO; 
	zlo=M_ZERO; 
	zhi=M_ZERO; 
}


mBBox2::~mBBox2(){
}


void mBBox2::copy(mBBox2 *it) { 
	xlo = it->xlo; 
	xhi = it->xhi; 
	zlo = it->zlo; 
	zhi = it->zhi; 
}


void mBBox2::addPt(mVec3 *data) {
	if (xlo>data->x) { xlo = data->x; } 
	if (xhi<data->x) { xhi = data->x; } 
	if (zlo>data->z) { zlo = data->z; } 
	if (zhi<data->z) { zhi = data->z; } 
}


bool mBBox2::touches(mBBox2 *it) {
	if (xlo>it->xhi) { return false; }
	if (xhi<it->xlo) { return false; }
	if (zlo>it->zhi) { return false; }
	if (zhi<it->zlo) { return false; }
	return true; 
}


bool mBBox2::contains(mVec3 *data) {
	if (xlo>data->x) { return false; }
	if (xhi<data->x) { return false; }
	if (zlo>data->z) { return false; }
	if (zhi<data->z) { return false; }
	return true; 
}


bool mBBox2::containsAll(const int ptCt, mVec3 *data) {
	int i; 

	for (i=0; i<ptCt; ++i) {
		if (!contains(&(data[i]))) { return false; }
	}
	return true; 
}


bool mBBox2::containsAny(const int ptCt, mVec3 *data) {
	int i; 

	for (i=0; i<ptCt; ++i) {
		if (contains(&(data[i]))) { return true; }
	}
	return false; 

}




//////////////////////////////
//////////////////////////////



mBBox3::mBBox3() {
	xlo=M_ZERO; 
	xhi=M_ZERO; 
	ylo=M_ZERO; 
	yhi=M_ZERO; 
	zlo=M_ZERO; 
	zhi=M_ZERO; 
}


mBBox3::~mBBox3(){
}


void mBBox3::copy(mBBox3 *it) { 
	xlo = it->xlo; 
	xhi = it->xhi; 
	ylo = it->ylo; 
	yhi = it->yhi; 
	zlo = it->zlo; 
	zhi = it->zhi; 
}


void mBBox3::setP1P2(mVec3 *p1, mVec3 *p2) {
	if (p1->x < p2->x) { xlo = p1->x; xhi = p2->x; }
	else { xlo = p2->x; xhi = p1->x; }

	if (p1->y < p2->y) { ylo = p1->y; yhi = p2->y; }
	else { ylo = p2->y; yhi = p1->y; }

	if (p1->z < p2->z) { zlo = p1->z; zhi = p2->z; }
	else { zlo = p2->z; zhi = p1->z; }

}




bool mBBox3::touches(mBBox3 *it) {
	if (xlo>it->xhi) { return false; }
	if (xhi<it->xlo) { return false; }
	if (ylo>it->yhi) { return false; }
	if (yhi<it->ylo) { return false; }
	if (zlo>it->zhi) { return false; }
	if (zhi<it->zlo) { return false; }
	return true; 
}


bool mBBox3::contains(mVec3 *data) {
	if (xlo>data->x) { return false; }
	if (xhi<data->x) { return false; }
	if (ylo>data->y) { return false; }
	if (yhi<data->y) { return false; }
	if (zlo>data->z) { return false; }
	if (zhi<data->z) { return false; }
	return true; 
}


bool mBBox3::containsAll(const int ptCt, mVec3 *data) {
	int i; 

	for (i=0; i<ptCt; ++i) {
		if (!contains(&(data[i]))) { return false; }
	}
	return true; 
}


bool mBBox3::containsAny(const int ptCt, mVec3 *data) {
	int i; 

	for (i=0; i<ptCt; ++i) {
		if (contains(&(data[i]))) { return true; }
	}
	return false; 

}
