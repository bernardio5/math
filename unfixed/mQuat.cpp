// Neal McDonald 2009

#include "mQuat.h"


#include <cmath>

mQuat::mQuat()
{
	setToIdentity(); 
}

mQuat::mQuat(const real win, const real xin, const real yin, const real zin)
{
	w = win; 
	x = xin; 
	y = yin; 
	z = zin;
}

mQuat::~mQuat() {}

void mQuat::addEq(mQuat *it)
{
	w = w + it->w;
	x = x + it->x;
	y = y + it->y;
	z = z + it->z;
	//normalize();
}

void mQuat::copy(mQuat *it)
{
	w = it->w;
	x = it->x;
	y = it->y;
	z = it->z;
	//normalize();
}


void mQuat::setToIdentity()
{
	w = M_ONE;
	x = M_ZERO;
	y = M_ZERO;
	z = M_ZERO;
}

void mQuat::set(const real win, mVec3 *axis)
{
	w = win; 
	x = axis->x;
	y = axis->y;
	z = axis->z;
}

void mQuat::setAngle(const real theta, mVec3 *axis)
{
	mVec3 tmpAxis;

	tmpAxis.copy(axis);
	tmpAxis.scale((real)sin(theta/2.0));

	set((real)cos(theta/2.0), &tmpAxis);
}

void mQuat::normalize()
{
	real mag;

	magnitude(&mag);

	if(mag <= M_EPSILON)
		mag = M_ONE;

	w /= mag;
	x /= mag;
	y /= mag;
	z /= mag;
}

void mQuat::magnitude(real *mag)
{
	*mag = (real)sqrt(w*w + x*x + y*y + z*z);
}

void mQuat::conjugate()
{
	x = -x; 
	y = -y; 
	z = -z; 
}

// this = this * it
void mQuat::preMult(mQuat *it)
{
	real a, b, c, d; 

	a = w * it->w - x * it->x - y * it->y - z * it->z;
	b = w * it->x + x * it->w + y * it->z - z * it->y;
	c = w * it->y + y * it->w + z * it->x - x * it->z;
	d = w * it->z + z * it->w + x * it->y - y * it->x;
	w=a; x=b; y=c; z=d;
//	normalize();
}

// this = it * this
void mQuat::postMult(mQuat *it)
{
	real a, b, c, d;

	a = it->w * w - it->x * x - it->y * y - it->z * z;
	b = it->w * x + it->x * w + it->y * z - it->z * y;
	c = it->w * y + it->y * w + it->z * x - it->x * z;
	d = it->w * z + it->z * w + it->x * y - it->y * x;
	w=a; x=b; y=c; z=d;
//	normalize();
}

void mQuat::vecPreMult(mVec3 *vec)
{
	real a, b, c, d;

	a = -(x*vec->x + y*vec->y + z*vec->z);
	b = w*vec->x + y*vec->z - z*vec->y;
	c = w*vec->y + z*vec->x - x*vec->z;
	d = w*vec->z + x*vec->y - y*vec->x;
	
	w=a; x=b; y=c; z=d;	
}

void mQuat::vecPostMult(mVec3 *vec)
{
	real a, b, c, d;

	a = -(x*vec->x + y*vec->y + z*vec->z);
	b = w*vec->x + z*vec->y - y*vec->z;
	c = w*vec->y + x*vec->z - z*vec->x;
	d = w*vec->z + y*vec->x - x*vec->y;

	w=a; x=b; y=c; z=d;	
}

void mQuat::scale(real r)
{
	w = w * r;
	x = x * r;
	y = y * r;
	z = z * r;
}

void mQuat::setMx(mx4 *it)
{
	it->setToIdentity(); 

	it->m_dM[0] = (real)(1.0 - 2.0*(y*y + z*z));
	it->m_dM[1] = (real)(2.0*(x*y - z*w));
	it->m_dM[2] = (real)(2.0*(x*z + w*y));

	it->m_dM[4] = (real)(2.0*(x*y + w*z));
	it->m_dM[5] = (real)(1.0 - 2.0*(x*x + z*z));
	it->m_dM[6] = (real)(2.0*(y*z - w*x));

	it->m_dM[8] = (real)(2.0*(x*z - w*y));
	it->m_dM[9] = (real)(2.0*(y*z + w*x));
	it->m_dM[10] = (real)(1.0 - 2.0*(x*x + y*y));
}

void mQuat::slerp(mQuat *a, mQuat *b, const real t)
{ 
	real omega, cosom, sinom, sclp, sclq; 
	mQuat p, q; 

	p.copy(a); 
	q.copy(b); 

	cosom = (p.w*q.w) + (p.x*q.x) + (p.y*q.y) + (p.z*q.z);
	if ((M_ONE+cosom) > M_EPSILON) { 
		if ((M_ONE-cosom) > M_EPSILON) { 
			omega = (real)acos(cosom); 
			sinom = (real)sin(omega); 
			sclp = (real)(sin((M_ONE-t)*omega) / sinom); 
			sclq = (real)(sin(t*omega) / sinom); 
		} else {
			sclp = M_ONE - t; 
			sclq = t; 
		}
		w = (sclp * p.w) + (sclq * q.w);  
		x = (sclp * p.x) + (sclq * q.x);  
		y = (sclp * p.y) + (sclq * q.y);  
		z = (sclp * p.z) + (sclq * q.z);  
	} else {
		w =	 p.z;
		x = -p.y; 
		y =  p.x;
		z = -p.w;
		sclp = (real)sin((M_HALF-t)*M_PI); 
		sclq = (real)sin(t * M_PI); 
		x = (sclp * p.x) + (sclq * x);  
		y = (sclp * p.y) + (sclq * y);  
		z = (sclp * p.z) + (sclq * z);  
	}
}

void mQuat::setFromMatrix(const mx4 *m)
{
	real tr, s;
	//[0,0], [1,1], [2,2]
	tr = m->m_dM[0] + m->m_dM[5] + m->m_dM[10];
	if(tr >= 0)
	{
		s = (real)sqrt(tr + 1);
		w = 0.5f * s;
		s = 0.5f / s;
		//x = (m[2,1] - m[1,2]) * s;
		x = (m->m_dM[9] - m->m_dM[6]) * s;
		//y = (m[0,2] - m[2,0]) * s;
		y = (m->m_dM[2] - m->m_dM[8]) * s;
		//z = (m[1,0] - m[0,1]) * s;
		z = (m->m_dM[4] - m->m_dM[1]) * s;
	}
	else
	{
		int i = 0;
		//if(m[1,1] > m[0,0])
		if(m->m_dM[5] > m->m_dM[0])
			i = 1;
		//if(m[2,2] > m[i,i))
		if(m->m_dM[10] > m->m_dM[i/4+i])
			i = 2;
		switch (i)
		{
			case 0:
				//s = sqrt((m[0,0] - (m[1,1] + m[2,2])) + 1);
				s = (real)sqrt((m->m_dM[0] - m->m_dM[5] + m->m_dM[10]) + 1);
				x = 0.5f * s;
				s = 0.5f / s;
				//y = (m[0,1] + m[1,0]) * s;
				y = (m->m_dM[1] + m->m_dM[4]) * s;
				//z = (m[2,0] + m[0,2]) * s;
				z = (m->m_dM[8] + m->m_dM[2]) * s;
				//w = (m[2,1] - m[1,2]) * s;
				w = (m->m_dM[9] - m->m_dM[6]) * s;
				break;
			case 1:
				//s = sqrt((m[1,1] - (m[2,2] + m[0,0])) + 1);
				s = (real)sqrt((m->m_dM[5] - (m->m_dM[10] + m->m_dM[0])) + 1);
				y = 0.5f * s;
				s = 0.5f / s;
				//z = (m[1,2] + m[2,1]) * s;
				z = (m->m_dM[6] + m->m_dM[9]) * s;
				//x = (m[0,1] + m[1,0]) * s;
				x = (m->m_dM[1] + m->m_dM[4]) * s;
				//w = (m[0,2] - m[2,0]) * s;
				w = (m->m_dM[2] - m->m_dM[8]) * s;
				break;
			case 2:
				//s = sqrt((m[2,2] - (m[0,0] + m[1,1])) + 1);
				s = (real)sqrt((m->m_dM[10] - (m->m_dM[0] + m->m_dM[5])) +1);
				z = 0.5f * s;
				s = 0.5f / s;
				//x = (m[2,0] + m[0,2]) * s;
				x = (m->m_dM[8] + m->m_dM[2]) * s;
				//y = (m[1,2] + m[2,1]) * s;
				y = (m->m_dM[6] + m->m_dM[9]) * s;
				//w = (m[1,0] - m[0,1]) * s;
				w = (m->m_dM[4] - m->m_dM[1]) * s;
		}
	}
}

//out = this * v * ~this
void mQuat::QVRotate(mVec3 *in, mVec3 *out)
{
	mQuat conj, tempQ;

	tempQ.copy(this);

	conj.copy(this);
	conj.conjugate();

	tempQ.vecPreMult(in);
	tempQ.preMult(&conj);

	out->x = tempQ.x;
	out->y = tempQ.y;
	out->z = tempQ.z;
}