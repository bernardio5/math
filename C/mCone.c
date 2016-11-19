

/* Neal McDonald 2009

	*/

#include "mCone.h"
#include <math.h>

void mcThreePoint(mConePtr c, mVec3 *eye, mVec3 *coi, mVec3 *up, real rad, real dr) {
	mvCopy(&(c->m_cSource), eye); 
	mvCopy(&(c->m_cCOI), coi);
	mvEqSub(&(c->m_cDir), coi, eye); 
	mvEqSub(&(c->m_cUp), up, eye); 

	mvNormalize(&(c->m_cDir)); 
	mvNormalize(&(c->m_cUp)); 

	c->m_fStartRadius = rad;
	c->m_fDr = dr;
	c->m_fInner = (real)atan(dr); 

	mxSetToLookAt(&(c->m_cMx), eye, coi, up); 

}



void mcSetPosition(mConePtr c, mVec3 *p) {
	mvCopy(&(c->m_cSource), p); 
	mvEqSub(&(c->m_cDir), &(c->m_cCOI), &(c->m_cSource)); 
	mvNormalize(&(c->m_cDir)); 
	mxSetToLookAt(&(c->m_cMx), &(c->m_cSource), &(c->m_cCOI), &(c->m_cUp));
}




void mcSetDirection(mConePtr c, mVec3 *d) {
	mvCopy(&(c->m_cDir), d); 
	mvEqAdd(&(c->m_cCOI), &(c->m_cSource), &(c->m_cDir)); 
	mvNormalize(&(c->m_cDir)); 
	mxSetToLookAt(&(c->m_cMx), &(c->m_cSource), &(c->m_cCOI), &(c->m_cUp));
}




void mcSetCOI(mConePtr c, mVec3 *p) {
	mvCopy(&(c->m_cCOI), p); 
	mvEqSub(&(c->m_cDir), &(c->m_cCOI), &(c->m_cSource)); 
	mvNormalize(&(c->m_cDir)); 
	mxSetToLookAt(&(c->m_cMx), &(c->m_cSource), &(c->m_cCOI), &(c->m_cUp));
}



void mcSetUp(mConePtr c, mVec3 *p) {
	mvCopy(&(c->m_cUp), p); 
	mvNormalize(&(c->m_cUp)); 
	mxSetToLookAt(&(c->m_cMx), &(c->m_cSource), &(c->m_cCOI), &(c->m_cUp));
}



void mcSetUpPt(mConePtr c, mVec3 *p) {
	mvEqSub(&(c->m_cUp), p, &(c->m_cSource)); 
	mvNormalize(&(c->m_cUp)); 
	mxSetToLookAt(&(c->m_cMx), &(c->m_cSource), &(c->m_cCOI), &(c->m_cUp));
}



void mcSetDr(mConePtr c, const real dr) {
	c->m_fDr = dr;
	c->m_fInner = (real)atan(dr); 
}







void mcRayPt(mConePtr c, const real t, mVec3 *res) {
//	res->copy(&m_cDir); 
//	res->scale(t); 
//	res->addEq(&m_cSource);

	res->x = (c->m_cSource).x + (t*(c->m_cDir.x)); 
	res->y = (c->m_cSource).y + (t*(c->m_cDir.y)); 
	res->z = (c->m_cSource).z + (t*(c->m_cDir.z)); 
}


// dot prod of direction with (vector from source to p) == 
// distance (along dir) of (closest pt on center ray) to p 
void mcClosestPtParam(mConePtr c, mVec3 *p, real *res) {
	*res = c->m_cDir.x * (p->x - c->m_cSource.x) +
			c->m_cDir.y * (p->y - c->m_cSource.y) +
			c->m_cDir.z * (p->z - c->m_cSource.z);
}



/*


bool touches(mConePtr *c, mBSphere *it) {
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




bool contains(mConePtr *c, mBSphere *it) {
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
*/

/*
// for 3D points

bool contains(mConePtr *c, mVec3 *data) {
	float t, d, coneRadius; 
	mVec3	r; 
	closestPtParam(data, &t);
	rayPt(t, &r);
	r.distanceFrom(data, &d);
	coneRadius = m_fStartRadius + (t*m_fDr); 
	if (d < coneRadius) { return true; }
	return false; 

}




bool containsAll(mConePtr *c, const int ptCt, mVec3 *data) {
	int i;

	for (i=0; i<ptCt; ++i) {
		if (!contains(&(data[i]))) { return false; }
	}
	return true; 
}

	
bool containsAny(mConePtr *c, const int ptCt, mVec3 *data) {
	int i; 

	for (i=0; i<ptCt; ++i) {
		if (contains(&(data[i*3]))) { return true; }
	}
	return false; 

}*/

/*

// I'm sleeeepy!
bool intersects(mConePtr *c, mBCone *it) {
	return false; 
}
 
 */

