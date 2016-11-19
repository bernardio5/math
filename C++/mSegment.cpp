// Neal McDonald 2009


#include "math/mSegment.h"

#define EPSILON 0.000000001

mSegment::mSegment(real a1, real b1, real a2, real b2) {
	set(a1,b1,a2,b2);
}

void mSegment::copy(mSegment *it) {
	set(it->x1,it->y1,it->x2,it->y2);
}

void mSegment::set(real a1, real b1, real a2, real b2) {
	x1=a1; x2=a2; y1=b1; y2=b2;
	dx=x2-x1; dy=y2-y1;
	length = sqrt(dx*dx+dy*dy);
}

void mSegment::reverse() {
	real c;
	c=x1; x1=x2; x2=c;
	c=y1; y1=y2; y2=c;
	dx= -dx; dy=-dy;
}

void mSegment::getPoint(real t, real *x, real *y) {
	*x = x1 + t * dx;
	*y = y1 + t * dy;
}

void mSegment::closestPoint(real x, real y, real *t, real *distance) {
	real dxp, dyp, len; 
	dxp = x-x1; 
	dyp = y-y1;
	len = (dxp*dx) + (dyp*dy); 
	*t = len / (length*length); 
	dxp -= len * dx / length; 
	dyp -= len * dy / length; 
	*distance = sqrt(dxp*dxp+dyp*dyp); 
}

bool mSegment::pointOn(real x, real y) { 
	real t, d; 
	closestPoint(x, y, &t, &d); 
	if (t<0.0) { return false; }
	if (t>1.0) { return false; }
	if (d>EPSILON) { return false; }
	return true; 
}

void mSegment::move1(real t) {
	real a1, b1, a2, b2;
	getPoint(t, &a1, &b1);
	a2 = x2; b2 = y2;
	set(a1,b1,a2,b2);
}

void mSegment::move2(real t) {
	real a1, b1, a2, b2;
	a1 = x1; b1 = y1;
	getPoint(t, &a2, &b2);
	set(a1,b1,a2,b2);
}

void mSegment::trim(real t, bool keepLowEnd) {
	if (keepLowEnd) {
		move2(t);
	} else {
		move1(t);
	}
}

void mSegment::split(real t, mSegment *it) {
	it->copy(this);
	move2(t);
	it->move1(t);
}

bool mSegment::outside(real x, real y) {
	real d1, d2;
	d1 = sqrt((x1-x)*(x1-x)+(y1-y)*(y1-y));
	d2 = sqrt((x2-x)*(x2-x)+(y2-y)*(y2-y));
	if ((d1>length)&&(d2>length)) {
		return false;
	}
	return true; 
}

bool mSegment::intersect(mSegment *it, real *t1, real *t2) {
	real a, b, c, d, e, f, top, bottom; 
	a=dx; b=-(it->dx); c=(it->x1)-x1; 
	d=dy; e=-(it->dy); f=(it->y1)-y1; 

	bottom = (e*a) - (b*d);
	if (fabs(bottom)<REAL_GAMMA) { return false; }
	top = (a*f) - (c*d);
	*t2 = top / bottom; 

	if (fabs(a)>fabs(d)) { 
		if (fabs(a)<REAL_GAMMA) { return false; }
		*t1 = (c - (*t2 * b))/a; 
	} else {
		if (fabs(d)<REAL_GAMMA) { return false; }
		*t1 = (f - (*t2 * e))/d; 
	}

/*	a = x1 + ((*t1)*dx); 
	b = y1 + ((*t1)*dy); 
	c = it->x1 + ((*t2)*it->dx); 
	d = it->y1 + ((*t2)*it->dy); 
	e = sqrt((a-c)*(a-c)+(b-d)*(b-d));
	if (e>EPSILON) { 
		e=0.0; 
	}
*/	return true; 
}

int mSegment::above(real x, real y) { 
	real bx, by, cross; 
	bx = x - x1; 
	by = y - y1; 
	cross = dx*by - dy*bx; 

	if (cross*cross<EPSILON) { return 0; }
	if (cross<0.0) { return -1; }
	else { return 1; }
}

// Neal McDonald 1/1/2005
