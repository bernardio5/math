
#include "mx4.h"


void mx4_copy(mx4 *to, mx4 *from) {
	int i;
	for (i=0; i<16; i++) {
		to->m_dM[i] = from->m_dM[i];
	}
}


void mx4_setToIdentity(mx4 *it) {
	int i;
	for (i=0; i<16; i++) {
		it->m_dM[i] = 0.0;
	}
	for (i=0; i<16; i+=5) { 
		it->m_dM[i] = 1.0;
	}
}


void mx4_setToXYZRotation(mx4 *it, const real angle, char axis) {
	mx4_setToIdentity(it);
	switch (axis) {
		case 'X':
		case 'x':
			it->m_dM[5] = (real)cos(angle);
			it->m_dM[6] = 0.0f - (real)sin(angle);
			it->m_dM[9] = (real)sin(angle);
			it->m_dM[10] = (real)cos(angle);
			break;
			
		case 'Y':
		case 'y':
			it->m_dM[0] = (real)cos(angle);
			it->m_dM[2] = (real)sin(angle);
			it->m_dM[8] = 0.0f - (real)sin(angle);
			it->m_dM[10] = (real)cos(angle);
			break;
			
		case 'Z':
		case 'z':
			it->m_dM[0] = (real)cos(angle);
			it->m_dM[1] = 0.0f - (real)sin(angle);
			it->m_dM[4] = (real)sin(angle);
			it->m_dM[5] = (real)cos(angle);
			break;
	}
}


void mx4_setToAxisRotation(mx4 *it, const real angle, mVec3 *axis) {
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
	
	mx4_setToIdentity(it);
	
	it->m_dM[0] = nx2;
	it->m_dM[1] = nxy-snz;
	it->m_dM[2] = nxz+sny;
	
	it->m_dM[4] = nxy+snz;
	it->m_dM[5] = ny2;
	it->m_dM[6] = nyz-snx;
	
	it->m_dM[8] = nxz-sny;
	it->m_dM[9] = nyz+snx;
	it->m_dM[10] = nz2;
}


void mx4_setToTranslation(mx4 *it, mVec3 *p) {
	mx4_setToIdentity(it);
	it->m_dM[3] = p->x;
	it->m_dM[7] = p->y;
	it->m_dM[11] = p->z;
}


void mx4_setToScale(mx4 *it, mVec3 *p) {
	mx4_setToIdentity(it);
	it->m_dM[0] = p->x;
	it->m_dM[5] = p->y;
	it->m_dM[10] = p->z;
}



void mx4_setToPerspective(mx4 *it, 
						  const real fov, const real aspect,
						  const real znear, const real zfar)	{
	real a = fov/2.0;
	real b = (real)cos(a);
	real c = (real)sin(a);
	real h = b/c;
	real w = h / aspect;
	
	it->m_dM[0] = 2.0 * znear / w;
	it->m_dM[1] = 0.0;
	it->m_dM[2] = 0.0;
	it->m_dM[3] = 0.0;
	
	it->m_dM[4] = 0.0;
	it->m_dM[5] = 2.0 * znear / h;
	it->m_dM[6] = 0.0;
	it->m_dM[7] = 0.0;
	
	it->m_dM[8] = 0.0;
	it->m_dM[9] = 0.0;
	it->m_dM[10] = zfar/(znear-zfar);
	it->m_dM[11] = -1.0;
	
	it->m_dM[12] = 0.0;
	it->m_dM[13] = 0.0;
	it->m_dM[14] = znear*zfar/(znear-zfar);
	it->m_dM[15] = 0.0;
}


void mx4_getPosition(mx4 *it, mVec3 *v)
{
	v->x = it->m_dM[3];
	v->y = it->m_dM[7];
	v->z = it->m_dM[11];
}


void mx4_transpose(mx4 *res, mx4 *src) {
	real a, m[16];
	int i;
	for (i=0; i<16; ++i) {
		m[i] = src->m_dM[i];
	}
	
	a = m[1];	m[1] = m[4];	m[4] = a;
	a = m[2];	m[2] = m[8];	m[8] = a;
	a = m[3];	m[3] = m[12];	m[12] = a;
	a = m[6];	m[6] = m[9];	m[9] = a;
	a = m[7];	m[7] = m[13];	m[13] = a;
	a = m[11];	m[11] = m[14];	m[14] = a;
	
	for (i=0; i<16; ++i) {
		res->m_dM[i] = m[i];
	}	
}


void mx4_invert(mx4 *res, mx4 *src) {
	real det;
	real a,b,c,d,e,f,g,h,i, m[16];
	int j;
	
	for (j=0; j<16; ++j) {
		m[j] = src->m_dM[j];
	}
	
	det =	m[0] * m[5] * m[10] -
			m[0] * m[6] * m[9] +
			m[1] * m[6] * m[8] -
			m[1] * m[4] * m[10] +
			m[2] * m[4] * m[9] -
			m[2] * m[5] * m[8];

	if (det == (real)0.0) {
		det = (real)1.0;
	}
	
	a = (m[5] * m[10] - m[9] * m[6]) / det;
	b = -(m[4] * m[10] - m[8] * m[6]) / det;
	c = (m[4] * m[9] - m[8] * m[5]) / det;

	d = -(m[1] * m[10] - m[9] * m[2]) / det;
	e = (m[0] * m[10] - m[8] * m[2]) / det;
	f = -(m[0] * m[9] - m[8] * m[1]) / det;

	g = (m[1] * m[6] - m[5] * m[2]) / det;
	h = -(m[0] * m[6] - m[4] * m[2]) / det;
	i = (m[0] * m[5] - m[4] * m[1]) / det;

	m[0] = a;
	m[1] = b;
	m[2] = c;

	m[4] = d;
	m[5] = e;
	m[6] = f;

	m[8] = g;
	m[9] = h;
	m[10] = i;
	
	for (j=0; j<16; ++j) {
		res->m_dM[j] = m[j];
	}
}


void mx4_add(mx4 *res, mx4 *a, mx4 *b) {
	int i;
	for (i=0; i<16; ++i) {
		res->m_dM[i] = a->m_dM[i] + b->m_dM[i];
	}
}


void mx4_subtract(mx4 *res, mx4 *a, mx4 *b) {
	int i;
	for (i=0; i<16; ++i) {
		res->m_dM[i] = a->m_dM[i] - b->m_dM[i];
	}
}


void mx4_mult(mx4 *res, mx4 *a, mx4 *b) {
	int i, j, k, n, o, p;
	real c[16];
	n = 0;
	for (i=0; i<4; ++i) {
		for (j=0; j<4; ++j) {
			c[n] = 0.0;
			o = (i*4);
			p = j;
			for (k=0; k<4; k++) {
				c[n] += a->m_dM[o] * b->m_dM[p];
				++o;
				p+=4;
			}
			++n;
		}
	}
	for (i=0; i<16; ++i) {
		res->m_dM[i] = c[i];
	}
}


void mx4_scale(mx4 *res, mx4 *a, real r) {
	for(int i=0;i<16;i++)
	{
		res->m_dM[i] = r * (a->m_dM[i]);
	}
}



void mx4_preMultSingle(mVec3 *res, mx4 *m, mVec3 *a) {
	real x, y, z;
	x = (a->x * m->m_dM[0]) + (a->y * m->m_dM[1]) + (a->z * m->m_dM[2]) + m->m_dM[3];
	y = (a->x * m->m_dM[4]) + (a->y * m->m_dM[5]) + (a->z * m->m_dM[6]) + m->m_dM[7];
	z = (a->x * m->m_dM[8]) + (a->y * m->m_dM[9]) + (a->z * m->m_dM[10]) + m->m_dM[11];
	mVec3_set(res, x, y, z);
}

void mx4_postMultSingle(mVec3 *res, mVec3 *a, mx4 *m) {
	real x, y, z;
	x = (a->x * m->m_dM[0]) + (a->y * m->m_dM[4]) + (a->z * m->m_dM[8]) + m->m_dM[12];
	y = (a->x * m->m_dM[1]) + (a->y * m->m_dM[5]) + (a->z * m->m_dM[9]) + m->m_dM[13];
	z = (a->x * m->m_dM[2]) + (a->y * m->m_dM[6]) + (a->z * m->m_dM[10]) + m->m_dM[14];
	mVec3_set(res, x, y, z);
}


void mx4_preMultMany(mVec3 *res, mx4 *it, mVec3 *src, const int ct) {
/*	int k;
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
*/
}




// Neal McDonald 1/1/2005