
// Neal McDonald 2009

#include "mVec3.h"

//#include <cmath>
#include <math.h>




void mvSeti(mVec3 *t, int a, int b, int c) {
	t->x=(real)a; t->y=(real)b; t->z=(real)c;
}

void mvSetf(mVec3 *t, real a, real b, real c) {
	t->x=a; t->y=b; t->z=c;
}


void mvCopy(mVec3 *t, mVec3 *it) {
	t->x = it->x;
	t->y = it->y;
	t->z = it->z;
}






void mvDotWith(mVec3 *t, mVec3 *it, real *res) {
	*res =  (it->x * t->x) + (it->y * t->y) + (it->z * t->z);
}

void mvDistanceFrom(mVec3 *t, mVec3 *it, real *res) {
	*res = (real)sqrt( (t->x - (it->x))*(t->x - (it->x))+
						(t->y - (it->y))*(t->y - (it->y))+
						(t->z - (it->z))*(t->z - (it->z)) ); 
}

void mvDistanceFromsq(mVec3 *t, mVec3 *it, real *res) {
	*res = ((t->x - (it->x))*(t->x - (it->x))+
			(t->y - (it->y))*(t->y - (it->y))+
			(t->z - (it->z))*(t->z - (it->z)) ); 
}



void mvLength3(mVec3 *t, real *res) {
	*res = (real)sqrt( (t->x*t->x)+(t->y*t->y)+(t->z*t->z) );
}
void mvLength2(mVec3 *t, real *res) {
	*res = (real)sqrt( (t->x*t->x)+(t->y*t->y) );
}
void mvLength3sq(mVec3 *t, real *res) {
	*res = ( (t->x*t->x)+(t->y*t->y)+(t->z*t->z) );
}
void mvLength2sq(mVec3 *t, real *res) {
	*res = ( (t->x*t->x)+(t->y*t->y) );
}

void mvScale(mVec3 *t, real s) {
	t->x*=s;   t->y*=s;   t->z*=s;
}
void mvZero(mVec3 *t) {
	t->x=M_ZERO; t->y=M_ZERO; t->z=M_ZERO;
}
void mvNegate(mVec3 *t) {
	t->x=-t->x;   t->y=-t->y;   t->z=-t->z; 
}

void mvNormalize(mVec3 *t) {
	real len; 
	mvLength3(t, &len); 
	if (len<M_EPSILON) { len=M_ONE; }
	len = M_ONE/len;
	mvScale(t, len); 
}

void mvRandom(mVec3 *t, real r) {
	real_rand(&(t->x)); 
	real_rand(&(t->y)); 
	real_rand(&(t->z)); 

	t->x = (t->x-M_HALF) * r; 
	t->y = (t->y-M_HALF) * r; 
	t->z = (t->z-M_HALF) * r; 

}

void mvAddNoise(mVec3 *t, real amount)
{
	real xR, yR, zR;
	real_rand(&xR);
	real_rand(&yR); 
	real_rand(&zR);

	t->x += ((xR*M_TWO)-M_ONE) * amount;
	t->y += ((yR*M_TWO)-M_ONE) * amount;
	t->z += ((zR*M_TWO)-M_ONE) * amount;

}
/////////////////////////////////////////////////////////////////////




void mvEqAdd(mVec3 *t, mVec3 *a, mVec3 *b) {
	t->x = (a->x) + (b->x); 
	t->y = (a->y) + (b->y); 
	t->z = (a->z) + (b->z); 
}
void mvEqSub(mVec3 *t, mVec3 *a, mVec3 *b) {
	t->x = (a->x) - (b->x); 
	t->y = (a->y) - (b->y); 
	t->z = (a->z) - (b->z); 
}
void mvEqCross(mVec3 *t, mVec3 *a, mVec3 *b) {
	t->x = ((a->y)*(b->z)) - ((a->z)*(b->y));
	t->y = M_ZERO - ( ((a->x)*(b->z)) - ((a->z)*(b->x)) );
	t->z = ((a->x)*(b->y)) - ((a->y)*(b->x));
}
void mvEqConv(mVec3 *t, mVec3 *a, mVec3 *b) {
	t->x = (a->x) * (b->x); 
	t->y = (a->y) * (b->y); 
	t->z = (a->z) * (b->z); 
}
void mvEqScale(mVec3 *t, mVec3 *a, real b) {
	t->x = (a->x) * b; 
	t->y = (a->y) * b; 
	t->z = (a->z) * b; 
}







void mvAddEq(mVec3 *t, mVec3 *a) {
	t->x += (a->x); 
	t->y += (a->y); 
	t->z += (a->z); 
}
void mvSubEq(mVec3 *t, mVec3 *a) {
	t->x -= (a->x); 
	t->y -= (a->y); 
	t->z -= (a->z); 
}
void mvCrossEq(mVec3 *t, mVec3 *b) {
	real c1, c2, c3;
	c1 = (t->y*(b->z)) - (t->z*(b->y));
	c2 = M_ZERO- ( (t->x*(b->z)) - (t->z*(b->x)) );
	c3 = (t->x*(b->y)) - (t->y*(b->x));
	t->x=c1; t->y=c2; t->z=c3;
}
void mvConvEq(mVec3 *t, mVec3 *a) {
	t->x *= (a->x); 
	t->y *= (a->y); 
	t->z *= (a->z); 
}

void mvRotateX(mVec3 *t, real r)
{
	real oldY = t->y;
	real oldZ = t->z;

	t->y = oldY*(real)cos(r) - oldZ*(real)sin(r);
	t->z = oldY*(real)sin(r) + oldZ*(real)cos(r);
}

void mvRotateY(mVec3 *t, real r)
{
	real oldX = t->x;
	real oldZ = t->z;

	t->x = oldX*(real)cos(r) + oldZ*(real)sin(r);
	t->z = -oldX*(real)sin(r) + oldZ*(real)cos(r);
}

void mvRotateZ(mVec3 *t, real r)
{
	real oldX = t->x;
	real oldY = t->y;

	t->x = oldX*(real)cos(r) - oldY*(real)sin(r);
	t->y = oldX*(real)sin(r) + oldY*(real)cos(r);
}
/*
void mvRotateX(mVec3 *t, mVec3 *v, const real r)
{
	t->x = v->x;
	t->y = v->y*(real)cos(r) - v->z*(real)sin(r);
	t->z = v->y*(real)sin(r) + v->z*(real)cos(r);
}

void mvRotateY(mVec3 *t, mVec3 *v, const real r)
{
	t->x = v->x*(real)cos(r) + v->z*(real)sin(r);
	t->y = v->y;
	t->z = -v->x*(real)sin(r) + v->z*(real)cos(r);
}

void mvRotateZ(mVec3 *t, mVec3 *v, const real r)
{
	t->x = v->x*(real)cos(r) - v->y*(real)sin(r);
	t->y = v->x*(real)sin(r) + v->y*(real)cos(r);
	t->z = v->z;
}
*/








