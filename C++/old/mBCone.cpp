

/* Copyright (C) 2002 Greenhouse Software.  All Rights Reserved.
	see mBCone.h for intro information about this code. 

	Implementation notes.

	*/

#include "mBCone.h"
#include <math.h>

mBCone::mBCone() {
	m_cSource.set((real)0.0, (real)0.0, (real)(-20.0));
	m_cDir.set(   (real)0.0, (real)0.0, (real)1.0); 
	m_cCOI.set(   (real)0.0, (real)0.0, (real)0.0); 
	m_cUp.set(    (real)0.0, (real)1.0, (real)0.0); 
	m_fStartRadius = (real)0.0;
	m_fDr = (real)0.0;
	m_fInner = (real)atan(m_fDr); 

	m_cMx.setToLookAt(&m_cSource, &m_cCOI, &m_cUp); 
}

mBCone::~mBCone() {
} 





void mBCone::threePoint(mVec3 *eye, mVec3 *coi, mVec3 *up, 
						real rad, real dr) {
	m_cSource.copy(eye); 
	m_cCOI.copy(coi);
	m_cDir.eqSub(coi, eye); 
	m_cUp.eqSub(up, eye); 

	m_cDir.normalize(); 
	m_cUp.normalize(); 

	m_fStartRadius = rad;
	m_fDr = dr;
	m_fInner = (real)atan(m_fDr); 

	m_cMx.setToLookAt(&m_cSource, &m_cCOI, &m_cUp); 

}



void mBCone::setPosition(mVec3 *p) {
	m_cSource.copy(p); 
	m_cDir.eqSub(&m_cCOI, &m_cSource); 
	m_cDir.normalize(); 
	m_cMx.setToLookAt(&m_cSource, &m_cCOI, &m_cUp); // but use COI
}




void mBCone::setDirection(mVec3 *d) {
	m_cDir.copy(d); 
	m_cCOI.eqAdd(&m_cSource, &m_cDir);
	m_cDir.normalize(); 
	m_cMx.setToLookAt(&m_cSource, &m_cCOI, &m_cUp); 
}




void mBCone::setCOI(mVec3 *p) {
	m_cCOI.copy(p); 
	m_cDir.eqSub(&m_cCOI, &m_cSource); 
	m_cDir.normalize(); 
	m_cMx.setToLookAt(&m_cSource, &m_cCOI, &m_cUp); 
}



void mBCone::setUp(mVec3 *p) {
	m_cUp.copy(p); 
	m_cUp.normalize(); 
	m_cMx.setToLookAt(&m_cSource, &m_cCOI, &m_cUp); 
}



void mBCone::setUpPt(mVec3 *p) {
	m_cUp.eqSub(p, &m_cSource); 
	m_cUp.normalize(); 
	m_cMx.setToLookAt(&m_cSource, &m_cCOI, &m_cUp); 
}



void mBCone::setDr(const real dr) {
	m_fDr = dr;
	m_fInner = (real)atan(m_fDr); 
}



void mBCone::getPosition(mVec3 *p) {
	p->copy(&m_cSource);
}

void mBCone::getDirection(mVec3 *d) {
	d->copy(&m_cDir);
}

void mBCone::getCOI(mVec3 *p) {
	p->copy(&m_cCOI);
}

void mBCone::getUp(mVec3 *u) {
	u->copy(&m_cUp);
}

void mBCone::getRadius(real *r, real *dr) {
	*r = m_fStartRadius;
	*dr = m_fDr;
}

void mBCone::getMx(mx4 *it) {
	it->copy(&m_cMx); 
}







void mBCone::rayPt(const real t, mVec3 *res) {
//	res->copy(&m_cDir); 
//	res->scale(t); 
//	res->addEq(&m_cSource);

	res->x = m_cSource.x + (t*m_cDir.x); 
	res->y = m_cSource.y + (t*m_cDir.y); 
	res->z = m_cSource.z + (t*m_cDir.z); 
}


// dot prod of direction with (vector from source to p) == 
// distance (along dir) of (closest pt on center ray) to p 
void mBCone::closestPtParam(mVec3 *p, real *res) {
	*res = m_cDir.x * (p->x - m_cSource.x) +
			m_cDir.y * (p->y - m_cSource.y) +
			m_cDir.z * (p->z - m_cSource.z);
}






bool mBCone::touches(mBSphere *it) {
	float t, d, coneRadius; 
	mVec3	c, r; 

	c.copy(&(it->m_cCenter));
	closestPtParam(&c, &t);
	rayPt(t, &r);
	c.distanceFrom(&r, &d); 
	// d now dist to center of BS

	d -= it->m_fRadius / (float)cos(m_fInner); 
	// d now dist to closest edge of BS

	coneRadius = m_fStartRadius + (t*m_fDr); 

	if (d < coneRadius) { return true; }
	return false; 
}




bool mBCone::contains(mBSphere *it) {
	float t, d, coneRadius; 
	mVec3	c, r; 

	// start by getting the distance from the center
	// of the ray to the center of the sphere
	c.copy(&(it->m_cCenter));
	closestPtParam(&c, &t);
	rayPt(t, &r);
	c.distanceFrom(&r, &d); 
	// d now dist to center of BS

	d += it->m_fRadius / (float)cos(m_fInner); 
	// d now == dist to far side of BS

	coneRadius = m_fStartRadius + (t*m_fDr); 

	if (d < coneRadius) { return true; }
	return false; 
}



// for 3D points

bool mBCone::contains(mVec3 *data) {
	float t, d, coneRadius; 
	mVec3	r; 
	closestPtParam(data, &t);
	rayPt(t, &r);
	r.distanceFrom(data, &d);
	coneRadius = m_fStartRadius + (t*m_fDr); 
	if (d < coneRadius) { return true; }
	return false; 

}




bool mBCone::containsAll(const int ptCt, mVec3 *data) {
	int i=0; 

	for (i=0; i<ptCt; ++i) {
		if (!contains(&(data[i]))) { return false; }
	}
	return true; 
}

	
bool mBCone::containsAny(const int ptCt, mVec3 *data) {
	int i=0; 

	for (i=0; i<ptCt; ++i) {
		if (contains(&(data[i*3]))) { return true; }
	}
	return false; 

}



// I'm sleeeepy!
bool mBCone::intersects(mBCone *it) {
	return false; 
}

