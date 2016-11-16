

// Neal McDonald 2002



#include "mBSphere.h"
#include <cmath>  


mBSphere::mBSphere() {
	m_cCenter.set((real)0.0, (real)0.0, (real)0.0);
	m_fRadius = (real)1.0;
}


mBSphere::~mBSphere(){
}


void mBSphere::copy(mBSphere *it) { 
	m_fRadius = it->m_fRadius; 
	m_fRadSq = it->m_fRadSq; 
	m_cCenter.copy(&(it->m_cCenter)); 
}


// reset
void mBSphere::set(mVec3 *center, const real radius) {
	m_cCenter.copy(center); 
	m_fRadius = radius;
	m_fRadSq = radius*radius; 
}





void mBSphere::compute(int pointCount, mVec3 *data) {
	static real zero = (real)0.0; 
	static real one = (real)1.0;
	mVec3 m; 
	real d, maxd; 
	int i; 

	m_cCenter.zero(); 
	m_fRadius = zero;

	m.zero(); 
	for (i=0; i<pointCount; ++i) {
		m.addEq(&(data[i])); 
	}
	if (pointCount>0) {
		d = one / pointCount; 
		m.scale(d); 
		m_cCenter.copy(&m); 
	}

	maxd = zero; 
	for (i=0; i<pointCount; ++i) {
		m.distanceFromsq(&(data[i]), &d); 
		if (d>maxd) { maxd = d; }
	}
	m_fRadius = (real)(sqrt(maxd));
	m_fRadSq = maxd; 
}





void mBSphere::addPt(mVec3 *data) {
	real d; 

	m_cCenter.distanceFromsq(data, &d); 
	if (d>m_fRadSq) { 
		m_fRadSq = d; 
		m_fRadius = (real)(sqrt(d)); 
	}

}






// enlarge self to contain it, here
void mBSphere::combine(mBSphere *it) {
	real dlen, span; 
	mVec3	d; 

	d.eqSub(&(it->m_cCenter), &m_cCenter); 
	d.length3(&dlen); 

	if ((dlen+it->m_fRadius) < m_fRadius) { return; }
	// (it was inside this)

	if ((dlen+m_fRadius) < it->m_fRadius) { 
		// this is inside it
		m_cCenter.copy(&(it->m_cCenter));
		m_fRadius =  it->m_fRadius;
		return;
	}

	if (dlen<((real)0.00001)) { dlen = (real)1.0; }
	else { dlen = ((real)1.0)/dlen; }
	d.scale(dlen); 

	span = dlen + it->m_fRadius + m_fRadius;  
	span *= (real)0.5;
	dlen = span - m_fRadius; 
	d.scale(dlen); 
	m_cCenter.addEq(&d); 
	m_fRadius = span; 
	m_fRadSq = span * span;
}







// determine whether that it is touching us again
bool mBSphere::touches(mBSphere *it) {
	real dlen, span; 

	m_cCenter.distanceFrom(&(it->m_cCenter), &dlen); 
	span = it->m_fRadius + m_fRadius;  

	if (dlen < span) { return true; }
	return false; 
}





bool mBSphere::contains(mBSphere *it) {
	real dlen, span; 

	m_cCenter.distanceFrom(&(it->m_cCenter), &dlen); 
	span = it->m_fRadius + dlen;

	if (span < m_fRadius) { return true; }
	return false; 
}


bool mBSphere::contains(mVec3 *data) {
	real dlen; 
	m_cCenter.distanceFromsq(data, &dlen); 
	if (dlen <= m_fRadSq) { return true; }
	return false; 

}


bool mBSphere::containsAll(const int ptCt, mVec3 *data) {
	int i=0; 

	for (i=0; i<ptCt; ++i) {
		if (!contains(&(data[i]))) { return false; }
	}
	return true; 
}


bool mBSphere::containsAny(const int ptCt, mVec3 *data) {
	int i=0; 

	for (i=0; i<ptCt; ++i) {
		if (contains(&(data[i]))) { return true; }
	}
	return false; 

}
