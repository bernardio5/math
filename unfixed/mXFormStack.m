
#include "mXFormStack.h"


#include <cmath>


void mXFormStack::push() {
	if (xformTop+1>=xformSz) {
		expandXforms();
	}
	xforms[xformTop+1].copy(&(xforms[xformTop]));
	++xformTop;
}


void mXFormStack::updateXform() {
	mx4 a, b, c;
	mVec3 t, s; 
	t.set(tx, ty, 0.0); 
	a.setToTranslation(&t);
	b.setToRotation((r*57.29577951308232), 'z');
	s.set(sx,sy,1.0); 
	c.setToScale(&s);
	a.preMult(&b); 
	a.preMult(&c); 
	xforms[xformTop].copy(&a);
}

void mXFormStack::translate(real x, real y) {
	tx+=x; ty+=y; 
	updateXform(); 
}

void mXFormStack::rotate(real deg) {
	r+=deg; 
	updateXform(); 
}

void mXFormStack::scale(real x, real y) {
	sx*=x; sy*=y; 
	updateXform(); 
}

void mXFormStack::pop() {
	if (xformTop>0) {
		--xformTop;
	}
}

void mXFormStack::map(real xin, real yin, real *xout, real *yout) {
	a[0] = (((b[0])*m[0])+((b[1])*m[1])+m[3]);
	a[1] = (((b[0])*m[0])+((b[1])*m[1])+m[3]);
	a[2] = (((b[2])*m[0])+((b[3])*m[1])+m[3]);
	a[3] = (((b[2])*m[0])+((b[3])*m[1])+m[3]);
}


void mXFormStack::expandXforms() {
	int a;
	mx4 *nx;

	a = xformSz*2;
	nx = new mx4[a];
	if (!nx) { return; }

	memcpy(nx, xforms, sizeof(mx4)*xformSz);
	delete [] xforms;
	xforms = nx;
	xformSz = a;
}


// Neal McDonald 1/1/2005