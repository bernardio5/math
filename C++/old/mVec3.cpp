
/* (c) Greenhouse Software 2002
	Excuse: 
		vector arithmetic implementation

	*/

#include "mVec3.h"
#include "mx4.h"

#include <cmath>
#include <math.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mVec3::mVec3() {
}

mVec3::~mVec3() {
}

mVec3::mVec3(const real a, const real b, const real c) {
	x=a; y=b; z=c;
}


void mVec3::set(const int a, const int b, const int c) {
	x=(real)a; y=(real)b; z=(real)c;
}

void mVec3::set(const real a, const real b, const real c) {
	x=a; y=b; z=c;
}


void mVec3::copy(mVec3 *it) {
	x = it->x;
	y = it->y;
	z = it->z;
}






void mVec3::dotWith(mVec3 *it, real *res) {
	*res =  (it->x * x) + (it->y * y) + (it->z * z);
}

void mVec3::distanceFrom(mVec3 *it, real *res) {
	*res = (real)sqrt( (x - (it->x))*(x - (it->x))+
						(y - (it->y))*(y - (it->y))+
						(z - (it->z))*(z - (it->z)) ); 
}

void mVec3::distanceFromsq(mVec3 *it, real *res) {
	*res = ((x - (it->x))*(x - (it->x))+
			(y - (it->y))*(y - (it->y))+
			(z - (it->z))*(z - (it->z)) ); 
}



void mVec3::length3(real *res) {
	*res = (real)sqrt( (x*x)+(y*y)+(z*z) );
}
void mVec3::length2(real *res) {
	*res = (real)sqrt( (x*x)+(y*y) );
}
void mVec3::length3sq(real *res) {
	*res = ( (x*x)+(y*y)+(z*z) );
}
void mVec3::length2sq(real *res) {
	*res = ( (x*x)+(y*y) );
}

void mVec3::scale(const real s) {
	x*=s;   y*=s;   z*=s;
}
void mVec3::zero() {
	static real zero = (real)0.0; 
	x=zero; y=zero; z=zero;
}
void mVec3::negate() {
	x=-x;   y=-y;   z=-z; 
}

void mVec3::normalize() {
	static real epsilon = (real)0.0000001; 
	static real one = (real)1.0; 
	real len; 
	length3(&len); 
	if (len<epsilon) { len=one; }
	len = one/len;
	scale(len); 
}

void mVec3::random(const real r) {
	const real ahalf = (real)0.5; 
	real_rand(&x); 
	real_rand(&y); 
	real_rand(&z); 

	x = (x-ahalf) * r; 
	y = (y-ahalf) * r; 
	z = (z-ahalf) * r; 

}

void mVec3::addNoise(const real amount)
{
	real xR, yR, zR;
	const real one = (real)1.0;
	const real two = (real)2.0;
	real_rand(&xR);
	real_rand(&yR); 
	real_rand(&zR);

	x += ((xR*two)-one) * amount;
	y += ((yR*two)-one) * amount;
	z += ((zR*two)-one) * amount;

}
/////////////////////////////////////////////////////////////////////




void mVec3::eqAdd(mVec3 *a, mVec3 *b) {
	x = (a->x) + (b->x); 
	y = (a->y) + (b->y); 
	z = (a->z) + (b->z); 
}
void mVec3::eqSub(mVec3 *a, mVec3 *b) {
	x = (a->x) - (b->x); 
	y = (a->y) - (b->y); 
	z = (a->z) - (b->z); 
}
void mVec3::eqCross(mVec3 *a, mVec3 *b) {
	static real zero = (real)0.0; 
	x = ((a->y)*(b->z)) - ((a->z)*(b->y));
	y = zero - ( ((a->x)*(b->z)) - ((a->z)*(b->x)) );
	z = ((a->x)*(b->y)) - ((a->y)*(b->x));
}
void mVec3::eqConv(mVec3 *a, mVec3 *b) {
	x = (a->x) * (b->x); 
	y = (a->y) * (b->y); 
	z = (a->z) * (b->z); 
}
void mVec3::eqScale(mVec3 *a, real b) {
	x = (a->x) * b; 
	y = (a->y) * b; 
	z = (a->z) * b; 
}







void mVec3::addEq(mVec3 *a) {
	x += (a->x); 
	y += (a->y); 
	z += (a->z); 
}
void mVec3::subEq(mVec3 *a) {
	x -= (a->x); 
	y -= (a->y); 
	z -= (a->z); 
}
void mVec3::crossEq(mVec3 *b) {
	static real zero = (real)0.0; 
	
	real c1, c2, c3;
	c1 = (y*(b->z)) - (z*(b->y));
	c2 = zero- ( (x*(b->z)) - (z*(b->x)) );
	c3 = (x*(b->y)) - (y*(b->x));
	x=c1; y=c2; z=c3;
}
void mVec3::convEq(mVec3 *a) {
	x *= (a->x); 
	y *= (a->y); 
	z *= (a->z); 
}

void mVec3::rotateX(const real r)
{
	real oldY = y;
	real oldZ = z;

	y = oldY*(real)cos(r) - oldZ*(real)sin(r);
	z = oldY*(real)sin(r) + oldZ*(real)cos(r);
}

void mVec3::rotateY(const real r)
{
	real oldX = x;
	real oldZ = z;

	x = oldX*(real)cos(r) + oldZ*(real)sin(r);
	z = -oldX*(real)sin(r) + oldZ*(real)cos(r);
}

void mVec3::rotateZ(const real r)
{
	real oldX = x;
	real oldY = y;

	x = oldX*(real)cos(r) - oldY*(real)sin(r);
	y = oldX*(real)sin(r) + oldY*(real)cos(r);
}

void mVec3::rotateX(mVec3 *v, const real r)
{
	x = v->x;
	y = v->y*(real)cos(r) - v->z*(real)sin(r);
	z = v->y*(real)sin(r) + v->z*(real)cos(r);
}

void mVec3::rotateY(mVec3 *v, const real r)
{
	x = v->x*(real)cos(r) + v->z*(real)sin(r);
	y = v->y;
	z = -v->x*(real)sin(r) + v->z*(real)cos(r);
}

void mVec3::rotateZ(mVec3 *v, const real r)
{
	x = v->x*(real)cos(r) - v->y*(real)sin(r);
	y = v->x*(real)sin(r) + v->y*(real)cos(r);
	z = v->z;
}









