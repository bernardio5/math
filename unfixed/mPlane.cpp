// Neal McDonald 2009

#include "mPlane.h"

#include <cmath>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


mPlane::mPlane() {
	p.set(M_ZERO, M_ZERO, M_ZERO); 
	n.set(M_ZERO, M_ONE, M_ZERO);
	d.set(-M_ONE, M_ZERO, M_ZERO);
	dNotSet = true; 
}


mPlane::~mPlane() {
}




void mPlane::makeUpD() {
	real len;
	mVec3 a;

	n.length3(&len); 
	if (len < M_EPSILON) {
		n.set(M_ZERO, M_ONE, M_ZERO);
		d.set(-M_ONE, M_ZERO, M_ZERO);
	}
	n.normalize(); 

	if ((n.x*n.x)<M_EPSILON) { 
		// n in plane between y and z
		if ((n.y*n.y)<M_EPSILON) {
			// n==z axis
			d.set(M_ZERO, M_ONE, M_ZERO); 
		} else {
			d.set(-M_ONE, M_ZERO, M_ZERO); 
		}
	} else {
		// n not in yz plane
		a.set(M_ZERO, M_ONE, M_ZERO); 
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
	p.set(M_ZERO, M_ZERO, M_ZERO); 
	n.set(M_ZERO, M_ONE, M_ZERO);
	d.set(-M_ONE, M_ZERO, M_ZERO);
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
	mVec3 d1, d2; 
	real len;

	d1.eqSub(p2, p1); 
	d1.length3(&len); 
	if (len<M_EPSILON) { return; } // p1=p2
	d1.scale(M_ONE/len); 

	d2.eqSub(p3,p1); 
	d2.length3(&len); 
	if (len<M_EPSILON) { return; } // p1=p3
	d2.scale(M_ONE/len); 

	p.copy(p1);
	n.eqCross(&d1, &d2); 
	n.length3(&len); 
	if (len<M_EPSILON) { return; } // colinear

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
