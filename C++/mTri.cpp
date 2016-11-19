// Neal McDonald 2009

#include "mTri.h"


bool mTri::init(real *xsIn, real *ysIn) {
	int i;
	real a;

	for (i=0; i<3; ++i) {
		xs[i] = xsIn[i];
		ys[i] = ysIn[i];
	}

	degenerate = false;
	flipped = false; 
	a = area(xs, ys);
	// maybe something more clever, using side len?
	if (real_eq(a,0.0)) {
		degenerate=true;
	} else {
		if (a<0.0) {
			// swap 1 and 2 to fix vert order
			xs[2] = xsIn[1];
			ys[2] = ysIn[1];
			xs[1] = xsIn[2];
			ys[1] = ysIn[2];
			flipped = true;
		}
		segs[0].set(xs[0], ys[0], xs[1], ys[1]);
		segs[1].set(xs[1], ys[1], xs[2], ys[2]);
		segs[2].set(xs[2], ys[2], xs[0], ys[0]);
	}
	return degenerate;
}


bool mTri::init(mVec3 *t1, mVec3 *t2, mVec3 *t3) {
	real xcp[3], ycp[3];
	xcp[0] = t1->x;
	xcp[1] = t2->x;
	xcp[2] = t3->x;
	ycp[0] = t1->y;
	ycp[1] = t2->y;
	ycp[2] = t3->y;
	return init(xcp, ycp); 
}


void mTri::copy(mTri *it) {
	xs[0] = it->xs[0];
	xs[1] = it->xs[1];
	xs[2] = it->xs[2];
	ys[0] = it->ys[0];
	ys[1] = it->ys[1];
	ys[2] = it->ys[2];
	segs[0].copy(&(it->segs[0]));
	segs[1].copy(&(it->segs[1]));
	segs[2].copy(&(it->segs[2]));
}


real mTri::area(real *x, real *y) {
	return 0.5* (
			((x[1]-x[0])*(y[2]-y[0]))-
			((x[2]-x[0])*(y[1]-y[0]))
			);
}


real mTri::area2(real *x, real *y) {
	return (((x[1]-x[0])*(y[2]-y[0]))-
			((x[2]-x[0])*(y[1]-y[0]))
			);
}


// ////////////clip helpers
// discard second vertex of in; it's outside
void mTri::front(mSegment *in, mSegment *out) {
	real t1, t2, tkeep;
	int i;

	// intersect w/ sides
	// find point w/ lowest t on in
	tkeep=1.0;
	for (i=0; i<3; ++i){
		if (segs[i].intersect(in,&t1,&t2)) {
			if ((t2<tkeep)&&(t2>0.0)) {
				tkeep=t2;
			}
		}
	}
	out->copy(in);
	if (tkeep<1.0) { out->move2(tkeep); }
}


// discard first vertex of in; it's outside
void mTri::back(mSegment *in, mSegment *out) {
	int i; 
	real t1, t2, tkeep;

	// intersect w/ sides
	// find point w/ highest t on in
	tkeep=0.0;
	for (i=0; i<3; ++i){
		if (segs[i].intersect(in,&t1,&t2)) {
			if ((t2>tkeep)&&(t2<1.0)) {
				tkeep=t2;
			}
		}
	}
	out->copy(in);
	if (tkeep>0.0) { out->move1(tkeep); }
}


// neither are in; does it cross?
// 16 cases, for the number of intersections possible:
// 0, 1, 2, 3 points, 0-3 lines
bool mTri::outsider(mSegment *in, mSegment *out) {
	int pointHits, pointsHit[3], lineHits, res, i;
	bool result;
	real d, ta, tb, tbs[3], pointParam;
	real a1, b1, a2, b2;

	pointHits=0;
	for (i=0; i<3; ++i) {
		// intersect with corners
		pointsHit[i] = 0;
		in->closestPoint(xs[i], ys[i], &tb, &d);
		if (d<REAL_GAMMA) {
			++pointHits;
			pointsHit[i] = 1;
			pointParam = tb;
			// only save one; if there are 2, you don't need either
		}
	}

	lineHits=0;
	// test line 1-- or don't, if either point was hit
	if ((pointsHit[0]==0) && (pointsHit[1]==0)){
		if (in->intersect(&(segs[0]),&ta,&tb)) {
			if ((0.0<ta)&&(ta<1.0)&&
				(0.0<tb)&&(tb<1.0)) {
				tbs[lineHits]=ta;
				++lineHits;
			}
		}
	}
	// test line 2
	if ((pointsHit[1]==0) && (pointsHit[2]==0)){
		if (in->intersect(&(segs[1]),&ta,&tb)) {
			if ((0.0<ta)&&(ta<1.0)&&
				(0.0<tb)&&(tb<1.0)) {
				tbs[lineHits]=ta;
				++lineHits;
			}
		}
	}
	// test line 3
	if ((pointsHit[2]==0) && (pointsHit[0]==0)){
		if (in->intersect(&(segs[2]),&ta,&tb)) {
			if ((0.0<ta)&&(ta<1.0)&&
				(0.0<tb)&&(tb<1.0)) {
				tbs[lineHits]=ta;
				++lineHits;
			}
		}
	}

	res = pointHits*10+lineHits;
	result = false;

	if (res==02) {						// clean two-line hit, untricky
		result = true;
		if (tbs[1]<tbs[0]) { 
			ta=tbs[0]; tbs[0]=tbs[1]; tbs[1]=ta; 
		}
		out->copy(in);
		out->getPoint(tbs[0], &a1, &b1);
		out->getPoint(tbs[1], &a2, &b2);
		out->set(a1,b1,a2,b2);
	}
	if (res==11) { 						// one point one line
		tbs[1] = pointParam;
		result = true;
		if (tbs[1]<tbs[0]) { ta=tbs[0]; tbs[0]=tbs[1]; tbs[1]=ta; }
		out->copy(in);
		out->getPoint(tbs[0], &a1, &b1);
		out->getPoint(tbs[1], &a2, &b2);
		out->set(a1,b1,a2,b2);
	}
/*		case 00: result = false; break;	// clean miss
		case 01: result = false; break; // no points & one line: error
		case 03: result = false; break; // three lines: error

		case 10: result = false; break; // one point no lines: glance
		case 12: result = false; break; // one point two lines; won't happen
		case 13: result = false; break; // three lines? RU degenerate?

		case 20: result = false; break; // 2pts no lines: glance
		case 21: result = false; break; // 2pts one line: glance w/ error
		case 22: result = false; break; // 2pts two lines: glance w/ errors
		case 23: result = false; break; // three lines: error

		case 30: result = false; break; // 3 pts: errors
		case 31: result = false; break;
		case 32: result = false; break;
		case 33: result = false; break;
*/
	return result;
}
//  //////////////////////// end clip helpers


bool mTri::clip(mSegment *in, mSegment *out) {
	char in1, in2;
	bool result;

	in1 = inside(in->x1, in->y1);
	in2 = inside(in->x2, in->y2);

	// both in; keep the whole thing.
	result = false;
	if ((in1==3) && (in2==3)) {
		out->copy(in);
		result = true;
	} else {
		// one in, keep half
		if (in1==3) {
			front(in,out);
			result = true;
		} else if (in2==3) {
			back(in,out);
			result = true;
		} else {
		// both out, but maybe it crosses
			result = outsider(in, out);
		}
	}
	return result;
}


char mTri::inside(real x, real y) {
	int i;
	real a, xsb[3], ysb[3];
	for (i=0; i<3; ++i) {
		xsb[i] = xs[i];
		ysb[i] = ys[i];

		// point hits take precedence
		if ((real_eq(x, xs[i])) && (real_eq(y,ys[i]))) {
			return 1;
		}
	}
	// on-line or outside takes precedence
	xsb[2]=x; ysb[2]=y;
	a = area2(xsb, ysb);
	if (real_lt(a,0.0)) { return 0; }
	if (real_eq(a,0.0)) { return 2; }

	xsb[2]=xs[2]; ysb[2]=ys[2];
	xsb[0]=x; ysb[0]=y;
	a = area2(xsb, ysb);
	if (real_lt(a,0.0)) { return 0; }
	if (real_eq(a,0.0)) { return 2; }

	xsb[0]=xs[0]; ysb[0]=ys[0];
	xsb[1]=x; ysb[1]=y;
	a = area2(xsb, ysb);
	if (real_lt(a,0.0)) { return 0; }
	if (real_eq(a,0.0)) { return 2; }

	// still here? you're in!
	return 3;
}


void mTri::getParam(real x, real y, real *u, real *v) {
	mSegment d, e;
	real bx, by, cx, cy, t;
	bool dontcare;
	bx = x - (xs[1]-xs[0]);
	by = y - (ys[1]-ys[0]);
	cx = x - (xs[2]-xs[0]);
	cy = y - (ys[2]-ys[0]);
	d.set(x, y, bx, by);
	e.set(x, y, cx, cy);
	dontcare = segs[0].intersect(&d, u, &t); 
	dontcare = segs[1].intersect(&e, v, &t); 
}


void mTri::getPoint(real u, real v, real *x, real *y) {
	real ax, ay, bx, by;
	segs[0].getPoint(u, &ax, &ay); 
	segs[1].getPoint(v, &bx, &by); 
	*x = ax + bx - xs[0];
	*y = ay + by - ys[0];
}



// Neal McDonald 1/1/2005


