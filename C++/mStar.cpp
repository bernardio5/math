// Neal McDonald 2009


#include <memory.h>
#include "mStar.h"


mStar::mStar() {
	maxPt = 0; 
	init(0.0,0.0);
}


mStar::~mStar() {
	if (0<maxPt) {
		delete [] rs;
		delete [] as;
	}
}

void mStar::init(real cx, real cy) {
	xc = cx; 
	yc = cy;
	lastPt=0;
	expand(10); 
}

void mStar::expand(int toWhat) {
	int a;
	real *nrs, *nas;

	if (toWhat<=maxPt) { return; }

	a = toWhat;
	nrs = new real[a];
	nas = new real[a];
	if (!nas) { maxPt=0; }
	else {
		if (maxPt!=0) { 
			memcpy(nrs, rs, sizeof(real)*maxPt);
			memcpy(nas, as, sizeof(real)*maxPt);
			delete [] rs;
			delete [] as;
		}
		rs = nrs;
		as = nas;
		maxPt = toWhat;
	}
}


void mStar::cart2Cyl(real x, real y, real *r, real *ang) {
	real dx, dy, len, a;

	dx = x-xc;
	dy = y-yc;
	len = sqrt(dx*dx+dy*dy);
	if (REAL_GAMMA<len) {
		dx /= len;
		a = acos(dx);
		if (dy < 0.0) { a = TWOPI - a; }
	} else {
		a = -1.1;
	}

	*r = len;
	*ang = a;
}


void mStar::cyl2Cart(real r, real ang, real *x, real *y) {
	real dx, dy;
	dx = r * cos(ang);
	dy = r * sin(ang);
	*x = xc + dx;
	*y = yc + dy;
}


int mStar::whichTri(real a) {
	int i, res;
	res = -1;

	if (lastPt>3) { 	
		for (i=0; i<lastPt-1; ++i) {
			if ((as[i]<=a) && (a<as[i+1])) {
				res = i;
			}
		}
		if ((as[lastPt-1]<=a) && (a<(as[0]+TWOPI))) {
			res = lastPt-1;
		}
	}
	return res;
}


bool mStar::triParam(int which, real r, real ang, real *u, real *lr) {
	real a1, a2, da, r1, r2, dr;

	if (lastPt<3) { return false; }
	if (which<0) { return false; }
	if (which>=lastPt) { return false; }


	a1 = as[which];
	r1 = rs[which];
	if (which<lastPt-1) {
		a2 = as[which+1];
		r2 = rs[which+1];
	} else {
		a2 = as[0];
		r2 = rs[0];
	}
	da = a2-a1;

	if (da<REAL_GAMMA) { return false; }
	if ((r1<REAL_GAMMA) && (r2<REAL_GAMMA)) { return false; }

	*u = (ang-a1)/da;
	dr = r2 - r1;
	*lr = r1 + (dr * (*u));
	return true;
}


void mStar::reset() {
	real smVal, sp;
	int i, j, smallest;

	if (lastPt<3) { return; }
	// swap sort by angle
	for (i=0; i<lastPt-1; ++i) {
		smallest = i;
		smVal = as[smallest];
		for (j=i+1; j<lastPt; ++j) {
			if (as[j]<smVal) {
				smallest=j;
				smVal = as[smallest];
			}
		}
		// swap xs, ys, angles
		if (smallest!=i) {
			sp = rs[i]; rs[i] = rs[smallest]; rs[smallest] = sp;
			sp = as[i]; as[i] = as[smallest]; as[smallest] = sp;
		}
	}
	rs[lastPt] = rs[0];
	as[lastPt] = as[0] + TWOPI;
}


void mStar::addPointCyl(real r, real a) {
	int i, j;
	if ((maxPt-2)<=lastPt) { 
		expand(maxPt*2); 
		if (maxPt==0) { return; }
	}

	if (lastPt==0) { 
		as[0]=a; rs[0]=r; 
	} else {
		i=0; 
		while ((i<lastPt-1) && (as[i]>a)) { ++i; }
		if ((as[lastPt-1]<a) && (a<(as[0]+TWOPI))) { i=lastPt; }
		if (as[i]==a) {
			// replacing r
			rs[i] = r;
		} else {
			// inserting r and a
			for (j=lastPt+1; j>i; --j) {
				as[j]=as[j-1];
				rs[j]=rs[j-1];
			}
			as[i]=a;
			rs[i]=r;
		}
	}
	++lastPt;
}


void mStar::addPointCart(real x, real y) {
	real r, a;
	cart2Cyl(x,y,&r,&a); 
	addPointCyl(r,a);
}


void mStar::setR(int i, real r) {
	if ((0<=i)&&(i<lastPt)) {
		rs[i]=r;
	}
}


void mStar::getR(int i, real *r) {
	if ((0<=i)&&(i<lastPt)) {
		*r=rs[i];
	}
}
void mStar::setA(int i, real a) {
	if ((0<=i)&&(i<lastPt)) {
		as[i]=a;
	}
}
void mStar::getA(int i, real *a) {
	if ((0<=i)&&(i<lastPt)) {
		*a=as[i];
	}
}


void mStar::squeeze(real degree) {
	int i;
	real del, r1, r2;
	for (i=0; i<lastPt; ++i) {
		del = cos(as[i] - direction);
		r1 = rs[i];
		r2 = r1 * del;
		rs[i] = ((1.0-degree)*r1) + (degree*r2);
	}
}


void mStar::spin(real radians) {
	int i;
	for (i=0; i<lastPt; ++i) {
		 as[i] += radians;
	}
	reset();
}


void mStar::round(real degree) {
	int i;
	real aveR;

	aveR = 0.0;
	for (i=0; i<lastPt; ++i) {
		aveR += rs[i];
	}
	aveR /= lastPt;

	for (i=0; i<lastPt; ++i) {
		rs[i] = ((1.0-degree)*rs[i]) + (degree*aveR);
	}
}


void mStar::smooth(real degree) {
	int i;
	real prev, sum, next;

	for (i=0; i<lastPt; ++i) {
		if (i==0) { prev = rs[lastPt-1]; }
		else { prev = rs[i-1]; }
		if (i==(lastPt-1)) { next = rs[0]; }
		else { next = rs[i+1]; }

		sum = (rs[i] + prev + next)/3.0;

		rs[i] = ((1.0-degree)*rs[i]) + (degree*sum);
	}
}


void mStar::grow(real factor) {
	int i;
	for (i=0; i<lastPt; ++i) {
		rs[i] *= factor;
	}
}


void mStar::moveTo(real dx, real dy) {
	xc += dx;
	yc += dy;
}


bool mStar::unoverlapWith(mStar *other, int steps, real gap) {
	int i, j, first;
	real r, a, x, y, sep, dr;
	bool notDone; 

	// can't unoverlap if the center is overlapped.
	if (other->inside(xc,yc)) { return false; }

	x = xc - other->xc; 
	y = yc - other->yc; 
	sep = 0.95 * sqrt(x*x+y*y); 

	// determine which edges to move.
	for (i=0; i<(other->lastPt); ++i) {
		r = other->rs[i];
		a = other->as[i];
		other->cyl2Cart(r, a, &x, &y); 
		cart2Cyl(x, y, &r, &a); 
		j = whichTri(a);
		// mark the r's that could need to be shortened
		if (0.0<rs[j]) { rs[j]=0.0-rs[j]; }
		if (j<lastPt-1) {
			if (0.0<rs[j+1]) { rs[j+1]=0.0-rs[j+1]; }
		} else {
			if (0.0<rs[0]) { rs[0]=0.0-rs[0]; }
		}
	}

	// shrink rays that are marked
	for (i=0; i<=lastPt; ++i) {
		r = 0.0 - rs[i];
		if (r>0.0) {
			if (r>sep) { r=sep; }
			dr = r/steps; 
			a = as[i];
			first=0; 
			notDone = true; 
			for (j=0; j<steps; ++j) {
				if (notDone) { 
					r = dr * j; 
					cyl2Cart(r, a, &x, &y);
					if (other->inside(x, y)) {
						first = j;
					} else { 
						notDone = false; 
					}
				}
			}
			rs[i] = (dr*first); 
		}
	}
	
	return true;
}


bool mStar::setWithin(mStar *other) {
	int i;
	real x, y, r;
	bool notDone;

	if (!other->inside(xc,yc)) { return false; }

	for (i=0; i<lastPt; ++i) {
		r = rs[i];
		notDone = false; 
		while (notDone) {
			cyl2Cart(r,as[i], &x,&y);
			if (!other->inside(x, y)) {
				r*=0.9;
				if (r<REAL_GAMMA) {
					return false;
				} 
			} else {
				rs[i]=r;
				notDone=true;
			}
		}
	}
	return true;
}



void mStar::blend(mStar *a, mStar *b) {
	int i,j;
	j=a->lastPt;
	if ((b->lastPt)<j) { j=b->lastPt; }
	if (maxPt<j) { expand(j); }
	for (i=0; i<j; ++i) {
		as[i]=0.5*((a->as[i])+(b->as[i]));
		rs[i]=0.5*((a->rs[i])+(b->rs[i]));
	}
	reset();
}


bool mStar::inside(real x, real y) {
	mTri w;
	real r, u, lr, ang;
	int i;
	bool result = false;

	cart2Cyl(x, y, &r, &ang);

	if (-1.0<ang) {
		i = whichTri(r);
		if (triParam(i, r, ang, &u, &lr)) {
			if (r<lr) { result = true; }
		}
	}
	return result;
}



void mStar::getCenter(real *x, real *y) {
	*x=xc; *y=yc; 
}


void mStar::getSides(int max, int *ct, real *xs, real *ys) {
	int i,j;
	real x,y;

	if (max<lastPt) { j=max; }
	else { j=lastPt; }
	for (i=0; i<j; ++i) {
		cyl2Cart(rs[i],as[i],&x,&y);
		xs[i]=x;
		ys[i]=y;
	}
	*ct=j;
}


bool mStar::getTri(int which, mTri *it) {
	real xs[3], ys[3];
	bool result;

	if ((lastPt-1)<which) { result=false; }
	else { result=true; } 

	xs[0]=xc;
	ys[0]=yc;
	cyl2Cart(rs[which],as[which],&(xs[1]),&(ys[1]));
	cyl2Cart(rs[which+1],as[which+1],&(xs[2]),&(ys[2]));
	it->init(xs, ys);
	return result;
}

