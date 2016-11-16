

/* 2002 Neal McDonald
	*/


#include "mBBox.h"
// #include <cmath>  


mBBox::mBBox() {
	const real zero = (real)0.0; 
	xlo=zero; 
	xhi=zero; 
	zlo=zero; 
	zhi=zero; 
}


mBBox::~mBBox(){
}


void mBBox::copy(mBBox *it) { 
	xlo = it->xlo; 
	xhi = it->xhi; 
	zlo = it->zlo; 
	zhi = it->zhi; 
}


void mBBox::clear(mVec3 *data) { 
	xlo = data->x; 
	xhi = data->x; 
	zlo = data->y; 
	zhi = data->y; 
}


void mBBox::addPt(mVec3 *data) {
	if (xlo>data->x) { xlo = data->x; } 
	if (xhi<data->x) { xhi = data->x; } 
	if (zlo>data->z) { zlo = data->z; } 
	if (zhi<data->z) { zhi = data->z; } 
}


bool mBBox::touches(mBBox *it) {
	if (xlo>it->xhi) { return false; }
	if (xhi<it->xlo) { return false; }
	if (zlo>it->zhi) { return false; }
	if (zhi<it->zlo) { return false; }
	return true; 
}


bool mBBox::contains(mVec3 *data) {
	if (xlo>data->x) { return false; }
	if (xhi<data->x) { return false; }
	if (zlo>data->z) { return false; }
	if (zhi<data->z) { return false; }
	return true; 
}


bool mBBox::containsAll(const int ptCt, mVec3 *data) {
	int i=0; 

	for (i=0; i<ptCt; ++i) {
		if (!contains(&(data[i]))) { return false; }
	}
	return true; 
}


bool mBBox::containsAny(const int ptCt, mVec3 *data) {
	int i=0; 

	for (i=0; i<ptCt; ++i) {
		if (contains(&(data[i]))) { return true; }
	}
	return false; 

}




//////////////////////////////
//////////////////////////////



mBBox3::mBBox3() {
	const real zero = (real)0.0; 
	xlo=zero; 
	xhi=zero; 
	ylo=zero; 
	yhi=zero; 
	zlo=zero; 
	zhi=zero; 
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
	int i=0; 

	for (i=0; i<ptCt; ++i) {
		if (!contains(&(data[i]))) { return false; }
	}
	return true; 
}


bool mBBox3::containsAny(const int ptCt, mVec3 *data) {
	int i=0; 

	for (i=0; i<ptCt; ++i) {
		if (contains(&(data[i]))) { return true; }
	}
	return false; 

}
