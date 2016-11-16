// mQt.cpp: implementation of the mQt class.
//
//////////////////////////////////////////////////////////////////////

#include "mQt.h"


#include <cmath>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


mQt::mQt() {
}


mQt::~mQt() {
}



void mQt::copy(mQt *it) {
	q.copy(&(it->q)); 
	t.copy(&(it->t)); 
}



void mQt::setToIdentity() {
	q.setToIdentity(); 
	t.zero(); 
}




/*
void mQt::preMerge(mQt *it) {	// this = it(this)
	mVec3 a; 

	(it->q).moveSingle(&t, &a);
	t.eqAdd(&a, &(it->t)); 

	q.preMult(&(it->q));
}



void mQt::postMerge(mQt *it) {	// this = this(it)
	mVec3 a; 

	q.moveSingle(&(it->t), &a); 
	t.addEq(&a); 

	q.postMult(&(it->q));
}*/





void mQt::setQ(mQuat *q2) {
	q.copy(q2); 
}


void mQt::setQ(const real angle, mVec3 *axis) {
	q.setAngle(angle, axis);
}


void mQt::setT(mVec3 *p) {
	t.copy(p); 
}




void mQt::lerp(mQt *qt1, mQt *qt2, real u) {
	static real one = (real)1.0; 
	mVec3 a; 
	
	q.slerp(&(qt1->q), &(qt2->q), u); 
	t.copy(&(qt1->t)); 
	t.scale(u); 

	a.copy(&(qt2->t));
	a.scale(one-u); 
	t.addEq(&a); 
}

/*
// ops on vectors-- used by mVertList
void mQt::moveSingle(mVec3 *in, mVec3 *out) {
	q.moveSingle(in, out); 	
	out->addEq(&t); 
}


void mQt::moveMany(const int ct, mVec3 *in, mVec3 *out) {
	int i; 
	mVec3 *tp = &t; 
	q.moveMany(ct, in, out); 

	for (i=0; i<ct; ++i) {
		(out[i]).addEq(tp); 
	}
}
*/