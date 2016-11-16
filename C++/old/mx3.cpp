// mx3.cpp: implementation of the mx3 class.
//
//////////////////////////////////////////////////////////////////////

#include "mx3.h"


#include <cmath>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


mx3::mx3() {
	setToIdentity(); 
}


mx3::~mx3() {
}



void mx3::copy(mx3 *it)
{
	int i;
	for (i=0; i<9; i++) {
		m_dM[i] = it->m_dM[i];
	}
}



void mx3::setToIdentity() {
	int i;
	for (i=0; i<9; i++) {
		m_dM[i] = 0.0f;
	}
	m_dM[0] = m_dM[4] = m_dM[8] = 1.0f;

}




void mx3::transpose() {
	real a; 
	a = m_dM[1];	m_dM[1] = m_dM[3];	m_dM[3] = a; 
	a = m_dM[2];	m_dM[2] = m_dM[6];	m_dM[6] = a; 
	a = m_dM[5];	m_dM[5] = m_dM[7];	m_dM[7] = a; 
}


void mx3::add(mx3 *it) {
	int i;

	for (i=0; i<9; ++i) {
		m_dM[i] += it->m_dM[i];
	}
}



void mx3::subtract(mx3 *it) {
	int i;

	for (i=0; i<9; ++i) {
		m_dM[i] -= it->m_dM[i];
	}

}



void mx3::preMult(mx3 *it)
{
	int i, j, k, n, o, p;
	real a[9];

	n = 0; 
	for (i=0; i<3; ++i) {
		for (j=0; j<3; ++j) {
			a[n] = 0.0;
			o = (i*3);
			p = j;
			for (k=0; k<3; k++) { 
				a[n] += m_dM[o] * it->m_dM[p];
				++o;
				p+=3; 
			}
			++n;
		}
	}

	for (i=0; i<9; ++i) {
		m_dM[i] = a[i];
	}
}








void mx3::postMult(mx3 *it)
{
	int i, j, k, n, o, p;
	real a[9];

	n = 0; 
	for (i=0; i<3; ++i) {
		for (j=0; j<3; ++j) {
			a[n] = 0.0;
			o = (i*3);
			p = j;
			for (k=0; k<3; k++) { 
				a[n] += it->m_dM[o] * m_dM[p];
				//a[n] += m_dM[o] * it->m_dM[p];
				++o;
				p+=3; 
			}
			++n;
		}
	}

	for (i=0; i<9; ++i) {
		m_dM[i] = a[i];
	}
}


void mx3::scale(real r) {
	for(int i=0;i<9;i++) {
		m_dM[i] *= r;
	}
}






void mx3::preMultSingle(mVec3 *in, mVec3 *out)
{
	out->x = (in->x * m_dM[0]) + (in->y * m_dM[1]) +  m_dM[2];
	out->y = (in->x * m_dM[3]) + (in->y * m_dM[4])  + m_dM[5];

}

void mx3::postMultSingle(mVec3 *in, mVec3 *out)
{
	out->x = (in->x * m_dM[0]) + (in->y * m_dM[3]) + m_dM[6];
	out->y = (in->x * m_dM[1]) + (in->y * m_dM[4]) + m_dM[7];
}






void mx3::preMultMany(const int ct, mVec3 *in, mVec3 *out) {
	int k;
	real x, y, z;
	mVec3 *p, *q;
	real m00, m01, m02; 
	real m10, m11, m12; 

	m00 = m_dM[0]; 
	m01 = m_dM[1]; 
	m02 = m_dM[2]; 

	m10 = m_dM[4]; 
	m11 = m_dM[5]; 
	m12 = m_dM[6]; 

	p = in;
	q = out; 
	for (k=0; k<ct; ++k) {
		x = p->x;
		y = p->y;
		z = p->z; 
		++p; 
		q->x = x * m00 + y * m01 + m02;
		q->y = x * m10 + y * m11 + m12;
		++q;
	}
}







void mx3::setToRotation(real angle) {
	setToIdentity();
	m_dM[0] = (real)cos(angle);
	m_dM[1] = 0.0f - (real)sin(angle);
	m_dM[3] = (real)sin(angle);
	m_dM[4] = (real)cos(angle);
}

void mx3::setToTranslation(mVec3 *p) {
	setToIdentity();
	m_dM[2] = p->x;
	m_dM[5] = p->y;
}

void mx3::setToScale(mVec3 *p) {
	setToIdentity();
	m_dM[0] = p->x;
	m_dM[4] = p->y;
}





