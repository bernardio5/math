// Neal McDonald 2009


#include "mx4.h"
#include <cmath>

mx4::mx4() {
	setToIdentity();
}


mx4::~mx4() {
}



void mx4::copy(mx4 *it)
{
	int i;
	for (i=0; i<16; i++) {
		m_dM[i] = it->m_dM[i];
	}
}



void mx4::setToIdentity() {
	int i;
	for (i=0; i<16; i++) {
		m_dM[i] = 0.0;
	}
	m_dM[0] = m_dM[5] = m_dM[10] = m_dM[15] = 1.0;

}




void mx4::transpose() {
	real a;
	a = m_dM[1];	m_dM[1] = m_dM[4];	m_dM[4] = a;
	a = m_dM[2];	m_dM[2] = m_dM[8];	m_dM[8] = a;
	a = m_dM[3];	m_dM[3] = m_dM[12];	m_dM[12] = a;
	a = m_dM[6];	m_dM[6] = m_dM[9];	m_dM[9] = a;
	a = m_dM[7];	m_dM[7] = m_dM[13];	m_dM[13] = a;
	a = m_dM[11];	m_dM[11] = m_dM[14];m_dM[14] = a;
}

void mx4::invert()
{
	real det;
	real a,b,c,d,e,f,g,h,i;

	det =	m_dM[0] * m_dM[5] * m_dM[10] -
			m_dM[0] * m_dM[6] * m_dM[9] +
			m_dM[1] * m_dM[6] * m_dM[8] -
			m_dM[1] * m_dM[4] * m_dM[10] +
			m_dM[2] * m_dM[4] * m_dM[9] -
			m_dM[2] * m_dM[5] * m_dM[8];

	if (det == (real)0.0)
		det = (real)1.0;

	a = (m_dM[5] * m_dM[10] - m_dM[9] * m_dM[6]) / det;
	b = -(m_dM[4] * m_dM[10] - m_dM[8] * m_dM[6]) / det;
	c = (m_dM[4] * m_dM[9] - m_dM[8] * m_dM[5]) / det;

	d = -(m_dM[1] * m_dM[10] - m_dM[9] * m_dM[2]) / det;
	e = (m_dM[0] * m_dM[10] - m_dM[8] * m_dM[2]) / det;
	f = -(m_dM[0] * m_dM[9] - m_dM[8] * m_dM[1]) / det;

	g = (m_dM[1] * m_dM[6] - m_dM[5] * m_dM[2]) / det;
	h = -(m_dM[0] * m_dM[6] - m_dM[4] * m_dM[2]) / det;
	i = (m_dM[0] * m_dM[5] - m_dM[4] * m_dM[1]) / det;

	m_dM[0] = a;
	m_dM[1] = b;
	m_dM[2] = c;

	m_dM[4] = d;
	m_dM[5] = e;
	m_dM[6] = f;

	m_dM[8] = g;
	m_dM[9] = h;
	m_dM[10] = i;
}

void mx4::add(mx4 *it) {
	int i;

	for (i=0; i<16; ++i) {
		m_dM[i] += it->m_dM[i];
	}
}



void mx4::subtract(mx4 *it) {
	int i;

	for (i=0; i<16; ++i) {
		m_dM[i] -= it->m_dM[i];
	}

}



void mx4::preMult(mx4 *it)
{
	int i, j, k, n, o, p;
	real a[16];

	n = 0;
	for (i=0; i<4; ++i) {
		for (j=0; j<4; ++j) {
			a[n] = 0.0;
			o = (i*4);
			p = j;
			for (k=0; k<4; k++) {
				a[n] += m_dM[o] * it->m_dM[p];
				++o;
				p+=4;
			}
			++n;
		}
	}

	for (i=0; i<16; ++i) {
		m_dM[i] = a[i];
	}
}








void mx4::postMult(mx4 *it)
{
	int i, j, k, n, o, p;
	real a[16];

	n = 0;
	for (i=0; i<4; ++i) {
		for (j=0; j<4; ++j) {
			a[n] = 0.0;
			o = (i*4);
			p = j;
			for (k=0; k<4; k++) {
				a[n] += it->m_dM[o] * m_dM[p];
				//a[n] += m_dM[o] * it->m_dM[p];
				++o;
				p+=4;
			}
			++n;
		}
	}

	for (i=0; i<16; ++i) {
		m_dM[i] = a[i];
	}
}


void mx4::scale(real r)
{
	for(int i=0;i<16;i++)
	{
		m_dM[i] *= r;
	}
}






void mx4::preMultSingle(mVec3 *in, mVec3 *out)
{
	out->x = (in->x * m_dM[0]) + (in->y * m_dM[1]) + (in->z * m_dM[2]) + m_dM[3];
	out->y = (in->x * m_dM[4]) + (in->y * m_dM[5]) + (in->z * m_dM[6]) + m_dM[7];
	out->z = (in->x * m_dM[8]) + (in->y * m_dM[9]) + (in->z * m_dM[10]) + m_dM[11];

}

void mx4::postMultSingle(mVec3 *in, mVec3 *out)
{
	out->x = (in->x * m_dM[0]) + (in->y * m_dM[4]) + (in->z * m_dM[8]) + m_dM[12];
	out->y = (in->x * m_dM[1]) + (in->y * m_dM[5]) + (in->z * m_dM[9]) + m_dM[13];
	out->z = (in->x * m_dM[2]) + (in->y * m_dM[6]) + (in->z * m_dM[10]) + m_dM[14];

}






void mx4::preMultMany(const int ct, mVec3 *in, mVec3 *out) {
	int k;
	real x, y, z;
	mVec3 *p, *q;
	real m00, m01, m02, m03;
	real m10, m11, m12, m13;
	real m20, m21, m22, m23;

	m00 = m_dM[0];
	m01 = m_dM[1];
	m02 = m_dM[2];
	m03 = m_dM[3];

	m10 = m_dM[4];
	m11 = m_dM[5];
	m12 = m_dM[6];
	m13 = m_dM[7];

	m20 = m_dM[8];
	m21 = m_dM[9];
	m22 = m_dM[10];
	m23 = m_dM[11];

	p = in;
	q = out;
	for (k=0; k<ct; ++k) {
		x = p->x;
		y = p->y;
		z = p->z;
		++p;
		q->x = x * m00 + y * m01 + z * m02 + m03;
		q->y = x * m10 + y * m11 + z * m12 + m13;
		q->z = x * m20 + y * m21 + z * m22 + m23;
		++q;
	}
}







void mx4::setToRotation(real angle, char axis)
{
	setToIdentity();
	switch (axis) {
		case 'X':
		case 'x':
			m_dM[5] = (real)cos(angle);
			m_dM[6] = 0.0f - (real)sin(angle);
			m_dM[9] = (real)sin(angle);
			m_dM[10] = (real)cos(angle);
			break;

		case 'Y':
		case 'y':
			m_dM[0] = (real)cos(angle);
			m_dM[2] = (real)sin(angle);
			m_dM[8] = 0.0f - (real)sin(angle);
			m_dM[10] = (real)cos(angle);
			break;

		case 'Z':
		case 'z':
			m_dM[0] = (real)cos(angle);
			m_dM[1] = 0.0f - (real)sin(angle);
			m_dM[4] = (real)sin(angle);
			m_dM[5] = (real)cos(angle);
			break;

	}
}


void mx4::setToRotation(real angle, mVec3 *axis)
{
	real nlen2 = 1.0f/(axis->x*axis->x+axis->y*axis->y+axis->z*axis->z);
	real nlen = (real)sqrt(nlen2);

	real s = (real)sin(angle);
	real c = (real)cos(angle);
	real ic = 1.0f-c;

	real snx = s * axis->x * nlen;
	real sny = s * axis->y * nlen;
	real snz = s * axis->z * nlen;

	real nxy = axis->x * axis->y * nlen2 * ic;
	real nxz = axis->x * axis->z * nlen2 * ic;
	real nyz = axis->y * axis->z * nlen2 * ic;

	real nx2 = (axis->x * axis->x * nlen2 * ic) + c;
	real ny2 = (axis->y * axis->y * nlen2 * ic) + c;
	real nz2 = (axis->z * axis->z * nlen2 * ic) + c;

	setToIdentity();

	m_dM[0] = nx2;
	m_dM[1] = nxy-snz;
	m_dM[2] = nxz+sny;

	m_dM[4] = nxy+snz;
	m_dM[5] = ny2;
	m_dM[6] = nyz-snx;

	m_dM[8] = nxz-sny;
	m_dM[9] = nyz+snx;
	m_dM[10] = nz2;
}





void mx4::setToTranslation(mVec3 *p) {
	setToIdentity();
	m_dM[3] = p->x;
	m_dM[7] = p->y;
	m_dM[11] = p->z;
}

void mx4::setToScale(mVec3 *p) {
	setToIdentity();
	m_dM[0] = p->x;
	m_dM[5] = p->y;
	m_dM[10] = p->z;
}

void mx4::getPosition(mVec3 *v)
{
	v->x = m_dM[3];
	v->y = m_dM[7];
	v->z = m_dM[11];
}



/*
void mx4::setToLookAt(mVec3 *eye, mVec3 *coi, mVec3 *up) {
	static real zero = (real)0.0;
	mVec3 z, y, x;
	real dot;

	// zaxis = normal(Eye - coi)
	z.eqSub(eye, coi);
	z.normalize();

	// xaxis = normal(cross(Up, zaxis))
	x.eqCross(up, &z);
	x.normalize();

	// yaxis = cross(zaxis, xaxis)
	y.eqCross(&z, &x);

	// at this point, you're just guessing.
	m_dM[0] = x.x;
	m_dM[1] = x.y;
	m_dM[2] = x.z;
	x.dotWith(eye, &dot);
	m_dM[3] = -dot;

	m_dM[4] = y.x;
	m_dM[5] = y.y;
	m_dM[6] = y.z;
	y.dotWith(eye, &dot);
	m_dM[7] = -dot;

	m_dM[8]  = z.x;
	m_dM[9]  = z.y;
	m_dM[10] = z.z;
	z.dotWith(eye, &dot);
	m_dM[11] = -dot;

	m_dM[12] = zero;
	m_dM[13] = zero;
	m_dM[14] = zero;
	m_dM[15] = 1.0f;
}
*/

/*
void mx4::setToLook(mVec3 *eye, mVec3 *dir, mVec3 *up) {
	static real zero = (real)0.0;
	mVec3 z, y, x;
	real dot;

	// zaxis = dir
	z.copy(dir);
	z.normalize();

	// xaxis = normal(cross(Up, zaxis))
	x.eqCross(up, &z);
	x.normalize();

	// yaxis = cross(zaxis, xaxis)
	y.eqCross(&z, &x);

	// at this point, you're just guessing.
	m_dM[0] = x.x;
	m_dM[1] = x.y;
	m_dM[2] = x.z;
	x.dotWith(eye, &dot);
	m_dM[3] = -dot; // we found the world's lamest bug here

	m_dM[4] = y.x;
	m_dM[5] = y.y;
	m_dM[6] = y.z;
	y.dotWith(eye, &dot);
	m_dM[7] = -dot;

	m_dM[8]  = z.x;
	m_dM[9]  = z.y;
	m_dM[10] = z.z;
	z.dotWith(eye, &dot);
	m_dM[11] = -dot;

	m_dM[12] = zero;
	m_dM[13] = zero;
	m_dM[14] = zero;
	m_dM[15] = 1.0f;
}
*/



void mx4::setToPerspective(	const real fov,	// in y
							const real aspect,
							const real znear,
							const real zfar)	{
	// does what DX8 docs say the DX8 routine does,
	// only, docs apparently in error. :)
	static real two = (real)2.0;
	static real zero = (real)0.0;
	real a = fov/two;
	real b = (real)cos(a);
	real c = (real)sin(a);

	real h = b/c;
	real w = h / aspect;

	m_dM[0] = two * znear / w;
	m_dM[1] = zero;
	m_dM[2] = zero;
	m_dM[3] = zero;

	m_dM[4] = zero;
	m_dM[5] = two * znear / h;
	m_dM[6] = zero;
	m_dM[7] = zero;

	m_dM[8] = zero;
	m_dM[9] = zero;
	m_dM[10] = zfar/(znear-zfar);
	m_dM[11] = (real)(-1.0);

	m_dM[12] = zero;
	m_dM[13] = zero;
	m_dM[14] = znear*zfar/(znear-zfar);
	m_dM[15] = zero;
}



/*
// much like the setToLook*'s, but !
// sets the mx to be a positioning mx.
void mx4::setToPosition(mVec3 *pos, mVec3 *pointing, mVec3 *up) {
	static real zero = (real)0.0;
	mVec3 z, y, x;

	// zaxis = dir
	z.copy(pointing);
	z.normalize();

	// xaxis = normal(cross(Up, zaxis))
	x.eqCross(up, &z);
	x.normalize();

	// yaxis = cross(zaxis, xaxis)
	y.eqCross(&z, &x);

	// at this point, you're just guessing.
	m_dM[0] = x.x;
	m_dM[1] = y.x;
	m_dM[2] = z.x;
	m_dM[3] = pos->x;

	m_dM[4] = x.y;
	m_dM[5] = y.y;
	m_dM[6] = z.y;
	m_dM[7] = pos->y;

	m_dM[8]  = x.z;
	m_dM[9]  = y.z;
	m_dM[10] = z.z;
	m_dM[11] = pos->z;

	m_dM[12] = zero;
	m_dM[13] = zero;
	m_dM[14] = zero;
	m_dM[15] = 1.0f;


}
*/

// Neal McDonald 1/1/2005