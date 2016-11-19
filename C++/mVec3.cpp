// Neal McDonald 2009



#include "mVec3.h"
#include <math.h>


mVec3::mVec3() {
}


mVec3::~mVec3() {
}



void mVec3::set(const real a, const real b, const real c) {
	x=a; y=b; z=c;
}


void mVec3::copy(mVec3 *it) {
	x = it->x;
	y = it->y;
	z = it->z;
}


void mVec3::dotWith(mVec3 *it, real *res) {
	*res =  (it->x * x) + (it->y * y) + (it->z * z);
}


void mVec3::length2(real *res) {
	*res = (real)sqrt( (x*x)+(y*y) );
}

void mVec3::length3(real *res) {
	*res = (real)sqrt( (x*x)+(y*y)+(z*z) );
}

void mVec3::normalize2() {
	real res = (real)sqrt( (x*x)+(y*y) );
	if (real_gt(res,0.0)) { 
		x/=res;
		y/=res;
	}
}

void mVec3::normalize3() {
	real res = (real)sqrt( (x*x)+(y*y)+(z*z) );
	if (real_gt(res,0.0)) { 
		x/=res;
		y/=res;
		z/=res;
	}
}

void mVec3::scale(const real s) {
	x*=s; 
	y*=s; 
	z*=s; 
}
	
void mVec3::eqAdd(mVec3 *a, mVec3 *b) {
	x = (a->x) + (b->x);
	y = (a->y) + (b->y);
	z = (a->z) + (b->z);
}
void mVec3::eqSub(mVec3 *a, mVec3 *b) {
	x = (a->x) - (b->x);
	y = (a->y) - (b->y);
	z = (a->z) - (b->z);
}

void mVec3::addEq(mVec3 *a) {
	x += (a->x);
	y += (a->y);
	z += (a->z);
}
	
void mVec3::distanceFromsq(mVec3 *a, real *d) {
	*d = ((a->x-x)*(a->x-x) +
		  (a->y-y)*(a->y-y) +
		  (a->z-z)*(a->z-z));
}


// Neal McDonald 1/1/2005





