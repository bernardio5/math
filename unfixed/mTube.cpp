// Neal McDonald 2009
#include <cstdio>
#include "mTube.h"
#include <cmath>
#include <math.h>

void mTube::doBanking(mLongPath *center) { 
	int i; 
	mVec3 p1, p2, p3, d12, d23, x, span, turnHint; 
	real dt, t1, t2, t3, gapt, spanLen, rlen, tiltMag; 

	dt = M_ONE / MTUBE_CACHE_SIZE; 
	gapt = 0.02f;		// what a magical number. 
	tiltMag = 80.0f;

	// get centers and radii
	for (i=0; i<MTUBE_CACHE_SIZE; ++i) { 
		t2 = i * dt; 
		t1 = t2 - gapt; 
		if (t1<M_ZERO) { t1 += M_ONE; }
		t3 = t2 + gapt; 
		if (t3>M_ONE) { t3 -= M_ONE; }

		center->oneSample(t1, &p1); 
		center->oneSample(t2, &p2); 
		center->oneSample(t3, &p3); 

		d12.eqSub(&p2, &p1); 
		d23.eqSub(&p2, &p3); 
		d12.normalize(); 
		d23.normalize(); 
		turnHint.eqCross(&d12, &d23); 

		span.eqSub(&p3, &p1); 
		span.length3(&spanLen);
		if (spanLen<M_EPSILON) { spanLen = M_EPSILON; }
		turnHint.scale(M_ONE/spanLen); 
		turnHint.dotWith(&m_cYHat, &rlen); 
		rlen *= tiltMag; 

		// too much? 
		if (rlen>0.0f) { 
			if (rlen>M_HALFPI) { 
				rlen = M_HALFPI;
			}
		} else {
			if (rlen<-M_HALFPI) { 
				rlen = -M_HALFPI; 
			}
		}
		radiuses[i].z = rlen; 
	}
}




void mTube::init(int datact, mVec3 *p, mVec3 *r) {
	int i;
	real t;
	mVec3 del, thetas; 
	mLongPath	center, radii; 

	center.setup(datact, p, true); 
	radii.setup(datact, r, true); 

	// get centers and radii
	for (i=0; i<MTUBE_CACHE_SIZE; ++i) { 
		t = i * m_rDt; 
		center.oneSample(t, &(centers[i])); 
		radii.oneSample(t, &(radiuses[i])); 
		flatAng[i] = radiuses[i].z; 
	}

	// compute dirs
	for (i=0; i<MTUBE_CACHE_SIZE-1; ++i) { 
		del.copy(&centers[i+1]); 
		del.subEq(&(centers[i])); 
		del.length3(&(delLens[i])); 
		dirs[i].copy(&del); 
		dirs[i].scale(M_ONE/delLens[i]); 
	}
	// and one last time for the last one
		i = MTUBE_CACHE_SIZE-1;		
		del.copy(&centers[0]); 
		del.subEq(&(centers[i])); 
		del.length3(&(delLens[i])); 
		dirs[i].copy(&del); 
		dirs[i].scale(M_ONE/delLens[i]); 


	// compute the normals of the planes separating the segments
	ns[0].eqAdd(&(dirs[MTUBE_CACHE_SIZE-1]), &(dirs[0])); 
	ns[0].scale(0.5f); 
	for (i=1; i<MTUBE_CACHE_SIZE; ++i) {
		ns[i].eqAdd(&(dirs[i-1]), &(dirs[i])); 
		ns[i].scale(M_HALF); 
	}

	doBanking(&center);

}




// find the index of the particle closest to p
// golly! it's slow. Caaaareful!
void mTube::searcher(mVec3 *p, int *result)
{
	real mind, d;
	int which, i; 
	
	p->distanceFromsq(&(centers[0]), &mind); 
	which = 0; 
	for (i=0; i<MTUBE_CACHE_SIZE; ++i)
	{ 
		p->distanceFromsq(&(centers[i]), &d); 
		if (d<mind)
		{ 
			mind = d; 
			which = i; 
		}
	}
	*result = which; 
}




// find the index of the particle closest to p
// depend upon "result" for a good hint.
void mTube::updateSearcher(mVec3 *p, int *result, int span)
{
	real mind, d;
	int which, j, best, min, max;

	min = *result-span;
	max = *result+span;

	p->distanceFromsq(&(centers[*result]), &mind);
	which = min;
    best = *result;
	for(j=0;j<span*2;j++)
	{
		if (which<0)
		{
			which += MTUBE_CACHE_SIZE;
		}
		if (which>=MTUBE_CACHE_SIZE)
		{
			which -= MTUBE_CACHE_SIZE;
		}

		p->distanceFromsq(&(centers[which]), &d);
		if (d<mind)
		{
			best = which;
			mind = d;
		}
		which++;
	}
	*result = best;
	if(best==min)
	{
		*result -= span;
		updateSearcher(p, result, span);
	}
	if(best==max-1)
	{
		*result += span;
		updateSearcher(p, result, span);
	}
}




// given p and a line segment, find the points on the planes at the end of the
// segment that are on the line || to the segment and through p. 
void mTube::getDiskPoints(mVec3 *p, int section, mVec3 *dp1, mVec3 *dp2) {
	mVec3 n, d1; 
	real dot1, dot2, u; 
	int next; 

	next = section+1; 
	if (next>=MTUBE_CACHE_SIZE) { next = 0; }

	// find the intersection with disk1
	n.copy(&(ns[section])); 
	d1.eqSub(&(centers[section]), p); 
	n.dotWith(&d1, &dot1); 
	n.dotWith(&(dirs[section]), &dot2); 
	if (dot2>0.0001f) {
		u = dot1/dot2;
	} else { 
		u=0.0f; 
	}

	dp1->copy(&(dirs[section]));
	dp1->scale(u); 
	dp1->addEq(p); 

	// find for disk 2
	n.copy(&(ns[next])); 
	d1.eqSub(&(centers[next]), p); 
	n.dotWith(&d1, &dot1); 
	n.dotWith(&(dirs[section]), &dot2); 
	if (dot2>0.0001f) {
		u = dot1/dot2;
	} else { 
		u=0.0f; 
	}

	dp2->copy(&(dirs[section]));
	dp2->scale(u); 
	dp2->addEq(p); 
}






// given index of pt closest to p, find the two points
// most appropriate to use as the current line segment
// this is the segment with the best local parameter. 
void mTube::getPair(mVec3 *p, int ind, int *prev, int *next, real *localt) {
	int i1, i2, i3; 
	real t1, t2, u, tot;  
	mVec3 p1, p2, d1, d2, dp1, dp2; 

	i1 = ind-1; 
	if (i1<0)
	{
		i1 = MTUBE_CACHE_SIZE-1;
	}
	i2 = ind;
	i3 = ind+1; 
	if (i3>=MTUBE_CACHE_SIZE)
	{
		i3 = 0;
	}


	// 
	getDiskPoints(p, i1, &dp1, &dp2); 
	d1.eqSub(p, &dp1); 
	d2.eqSub(&dp2, &dp1); 
	d1.length3(&u); 
	d2.length3(&tot); 
	if (tot>M_EPSILON) {
		t1 = u/tot; 
	} else {
		t1 = M_ZERO; 
	}

	// 
	getDiskPoints(p, i2, &dp1, &dp2); 
	d1.eqSub(p, &dp1); 
	d2.eqSub(&dp2, &dp1); 
	d1.length3(&u); 
	d2.length3(&tot); 
	if (tot>0.0001f) {
		t2 = u/tot; 
	} else {
		t2 = 0.0; 
	}

	if ((0.0f<=t1) && (t1<M_ONE)) {
		*prev = i1; 
		*next = i2; 
		*localt = t1; 
	} else if ((0.0f<=t2) && (t2<M_ONE)) {
		*prev = i2; 
		*next = i3; 
		*localt = t2; 
	} else {
		// big trouble. try and fake your way out. 
		*prev = i2; 
		*next = i3; 
		*localt = t2; 
	}

}



// given the index oc this segments and the next, 
//  generate center, dir, up, right
void mTube::getBasis(int prev, int next, real localt, 
					 mVec3 *center, mVec3 *dir, mVec3 *up, mVec3 *right, mVec3 *rs, real *flat) {
	real it; 
	mVec3 x, yhat; 

	it = M_ONE-localt; 

	center->copy(&(dirs[prev])); 
	center->scale(localt * delLens[prev]); 
	center->addEq(&(centers[prev])); 

	dir->copy(&(dirs[prev])); 
	dir->scale(it); 
	x.copy(&(dirs[next])); 
	x.scale(localt); 
	dir->addEq(&x);
	dir->normalize(); 

	yhat.set(M_ZERO,M_ONE,M_ZERO); 
	right->eqCross(dir, &yhat); 
	right->normalize(); // hey! fails for right==0

	up->eqCross(right, dir);

	rs->copy(&(radiuses[prev])); 
	rs->scale(it); 
	x.copy(&(radiuses[next])); 
	x.scale(localt); 
	rs->addEq(&x);					 

	*flat = (it * flatAng[prev]) + (localt * flatAng[next]); 
}



// given a good section estimate, detmn the params that correspond to p.
// params.xyz=(t, theta, d). d is world-coords dist from tube center; 
// not affected by tube radii changes. 
// dir is set by getBasis. centerp is the point on the center of the tube. 
// shellp is the projection of d onto the tube wall; requires use of 
// tube radii info. 
void mTube::mapCompletion(mVec3 *p, mVec3 *params, int *section) {
	int prev, next; 
	real t, localt, theta, d, ar, au, flat; 
	mVec3 a, centerp, up, right, rs, dir; 

	getPair(p, *section, &prev, &next, &localt);
	*section = next;

	getBasis(prev, next, localt, &centerp, &dir, &up, &right, &rs, &flat); 
	t = (prev * m_rDt) + (localt * m_rDt); 

	// a is the vector from the closest center point to p; get d
	a.eqSub(p, &centerp); 

	// find theta
	a.dotWith(&up, &au); 

	a.length3(&d); 
	if (d<M_EPSILON) { d = M_EPSILON; }
	a.scale(M_ONE/d);
	a.dotWith(&right, &ar); 
	theta = (real)acos(ar); 
	if (au< M_ZERO) {
		theta = M_TWOPI - theta;
	} 
	theta += rs.z; 

	params->set(t, theta, d);
}




void mTube::map(mVec3 *p, mVec3 *params, int *section) {
	searcher(p, section); 
	mapCompletion(p, params, section); 
}


void mTube::updateMap(mVec3 *p, mVec3 *params, int *section) {
	updateSearcher(p, section, 32); 
	mapCompletion(p, params, section); 
}





// use t to find prev, next indices-- also localt. 
void mTube::getLocals(real t, real *localt, int *prev, int *next) {
	real mt; 
	mt = t - (real)floor(t); 
	*prev = (int)((floor)(mt / m_rDt)); 
	*localt = (mt - ((*prev)*m_rDt)) / m_rDt;
	*next = (*prev)+1; 
	if (*next >= MTUBE_CACHE_SIZE) { *next = 0; }
}


// set scale to be the fraction of d that shellp is moved 
// due to the tube being flattened. 
void mTube::computeDent(real flat, real theta, real *scale) { 
/*	real w2, a, delta;

	w2 = (3.0f * PI) - w1; 
	if ((w1<theta) && (theta<w2)) {
		if (theta<(1.5f*PI)) {
			delta = (1.5f*PI) -  theta; 
		} else {
			delta = theta - (1.5f*PI); 
		}
		a = (real)sin(w1-PI); 
		*scale = a/((real)cos(delta)); 
	} else {
		// theta not in track range; nevermind */
		*scale = M_ONE;
//	}
}






void mTube::sliderAdjuster(real localt, int prev, mVec3 *p ) {
	mVec3 d, dp1, dp2; 
	
	getDiskPoints(p, prev, &dp1, &dp2); 

	d.eqSub(&dp2, &dp1); 
	d.scale(localt); 
	p->eqAdd(&dp1, &d); 
}




// params set p. params.xyz=(t, theta, d). p.z is distance from the center of the tube;
// it is not affected by tube radius variation. 
// n MUST be affected by radii variations; therefore you will see 
// a cross-product to generate it
//  the vector d s just a lookup. 
void mTube::getPointNormal(mVec3 *params, mVec3 *p, mVec3 *n, mVec3 *dir, mVec3 *shellp) {
	mVec3 centerp, rs, a, b, c, du, dr, up, right, cdir, theta12; 
	int prev, next; 
	real localt, theta, disp, phiScale1, phiScale2, flatspot, clocalt; 

	getLocals(params->x, &localt, &prev, &next); 
	getBasis(prev, next, localt, &centerp, dir, &up, &right, &rs, &flatspot);
	theta = params->y - rs.z; 
	disp = params->z;

	// determine whether theta is in the flat range.
	computeDent(flatspot, theta, &phiScale1); 
	computeDent(flatspot, theta+0.1f, &phiScale2); 
	
	p->copy(&centerp); 
	du.copy(&up); 
	du.scale((real)sin(theta) * phiScale1 * disp); 
	p->addEq(&du); 
	dr.copy(&right); 
	dr.scale((real)cos(theta) * phiScale1 * disp); 
	p->addEq(&dr); 
	sliderAdjuster(localt, prev, p); 	

	// we have p; now compute n. 
	// n requires 3 shellpoints

	a.copy(&centerp); 
	du.copy(&up); 
	du.scale((real)sin(theta) * phiScale1 * rs.y); 
	a.addEq(&du); 
	dr.copy(&right); 
	dr.scale((real)cos(theta) * phiScale1 * rs.x); 
	a.addEq(&dr); 
	sliderAdjuster(localt, prev, &a); 	

	b.copy(&centerp); 
	up.scale(rs.y * (real)sin(theta+0.1f) * phiScale2); 
	b.addEq(&up); 
	right.scale(rs.x * (real)cos(theta+0.1f) * phiScale2); 
	b.addEq(&right); 
	sliderAdjuster(localt, prev, &b); 	

	// what an ass pain the third shellpt is!
	getLocals((params->x+0.01f), &clocalt, &prev, &next);  
	getBasis(prev, next, clocalt, &centerp, &cdir, &up, &right, &rs, &flatspot);
	theta = params->y - rs.z; 
	computeDent(flatspot, theta, &phiScale1); 
	c.copy(&centerp); 
	up.scale((real)sin(theta) * phiScale1 * rs.y); 
	c.addEq(&up); 
	right.scale((real)cos(theta) * phiScale1 * rs.x); 
	c.addEq(&right); 
	sliderAdjuster(clocalt, prev, &c); 	

	// cross-prod your way to n
	b.subEq(&a); 
	c.subEq(&a); 
	n->eqCross(&b, &c); 
	n->normalize(); 

	shellp->copy(&a); 
}











