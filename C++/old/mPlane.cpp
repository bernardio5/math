/* Copyright (C) 2002 Greenhouse Software.  
	All Rights Reserved.
	*/


#include "mPlane.h"

#include <cmath>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


mPlane::mPlane() {
	static real zero = (real)0.0; 
	static real one = (real)1.0;
	p.set(zero, zero, zero); 
	n.set(zero, one, zero);
	d.set(-one, zero, zero);
	dNotSet = true; 
}


mPlane::~mPlane() {
}




void mPlane::makeUpD() {
	static real epsilon = (real)0.000001; 
	static real zero = (real)0.0; 
	static real one = (real)1.0;
	real len;
	mVec3 a;

	n.length3(&len); 
	if (len < epsilon) {
		n.set(zero, one, zero);
		d.set(-one, zero, zero);
	}
	n.normalize(); 

	if ((n.x*n.x)<epsilon) { 
		// n in plane between y and z
		if ((n.y*n.y)<epsilon) {
			// n==z axis
			d.set(zero, one, zero); 
		} else {
			d.set(-one, zero, zero); 
		}
	} else {
		// n not in yz plane
		a.set(zero, one, zero); 
		d.eqCross(&n, &a); 
	}
}





void mPlane::getMx(mx4 *m) {
	mVec3 coi; 

	coi.eqAdd(&p, &d);
	m->setToLookAt(&p, &coi, &n); 
}



void mPlane::copy(mPlane *it) {
	p.copy(&(it->p)); 
	n.copy(&(it->p));
	d.copy(&(it->d));
	c = it->c;
	dNotSet = it->dNotSet;
}







void mPlane::setToIdentity() {
	static real zero = (real)0.0; 
	static real one = (real)1.0;
	p.set(zero, zero, zero); 
	n.set(zero, one, zero);
	d.set(-one, zero, zero);
}


void mPlane::setP(mVec3 *pp) {
	p.copy(pp); 
	n.dotWith(&p, &c); 
}


void mPlane::setN(mVec3 *nn) {
	n.copy(nn);
	n.normalize();
	n.dotWith(&p, &c); 
	if (dNotSet) { makeUpD(); }
}


void mPlane::setD(mVec3 *nd) {
	d.copy(nd);
	dNotSet = false; 
}


void mPlane::setFrom3(mVec3 *p1, mVec3 *p2, 
					  mVec3 *p3) {
	static real epsilon = (real)0.000001; 
	static real one = (real)1.0; 
	mVec3 d1, d2; 
	real len;

	d1.eqSub(p2, p1); 
	d1.length3(&len); 
	if (len<epsilon) { return; } // p1=p2
	d1.scale(one/len); 

	d2.eqSub(p3,p1); 
	d2.length3(&len); 
	if (len<epsilon) { return; } // p1=p3
	d2.scale(one/len); 

	p.copy(p1);
	n.eqCross(&d1, &d2); 
	n.length3(&len); 
	if (len<epsilon) { return; } // colinear

	d.copy(&d1); 
	dNotSet = false; 

	n.dotWith(&p, &c); 
}


void mPlane::distanceFrom(mVec3 *v, real *result) {
	real d; 
	n.dotWith(v, &d); 
	d = c - d; 
	*result = d; 
}
