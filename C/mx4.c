// Neal McDonald 2009
// mx4.cpp: implementation of the mx4 class.
//
//////////////////////////////////////////////////////////////////////

#include "mx4.h"


// #include <cmath>



void mxCopy(mx4 *t, mx4 *it)
{
	int i;
	for (i=0; i<16; i++) {
		t->m_dM[i] = it->m_dM[i];
	}
}



void mxSetToIdentity(mx4 *t) {
	int i;
	for (i=0; i<16; i++) {
		t->m_dM[i] = 0.0;
	}
	t->m_dM[0] = t->m_dM[5] = t->m_dM[10] = t->m_dM[15] = 1.0;

}




void mxTranspose(mx4 *t) {
	real a; 
	a = t->m_dM[1];	t->m_dM[1] = t->m_dM[4];	t->m_dM[4] = a; 
	a = t->m_dM[2];	t->m_dM[2] = t->m_dM[8];	t->m_dM[8] = a; 
	a = t->m_dM[3];	t->m_dM[3] = t->m_dM[12];	t->m_dM[12] = a; 
	a = t->m_dM[6];	t->m_dM[6] = t->m_dM[9];	t->m_dM[9] = a; 
	a = t->m_dM[7];	t->m_dM[7] = t->m_dM[13];	t->m_dM[13] = a; 
	a = t->m_dM[11];	t->m_dM[11] = t->m_dM[14];t->m_dM[14] = a; 
}

void mxInvert(mx4 *t)
{
	real det;
	real a,b,c,d,e,f,g,h,i;
	
	det =	t->m_dM[0] * t->m_dM[5] * t->m_dM[10] -
			t->m_dM[0] * t->m_dM[6] * t->m_dM[9] +
			t->m_dM[1] * t->m_dM[6] * t->m_dM[8] -
			t->m_dM[1] * t->m_dM[4] * t->m_dM[10] +
			t->m_dM[2] * t->m_dM[4] * t->m_dM[9] -
			t->m_dM[2] * t->m_dM[5] * t->m_dM[8];

	if (det == (real)0.0)
		det = (real)1.0;

	a = (t->m_dM[5] * t->m_dM[10] - t->m_dM[9] * t->m_dM[6]) / det;
	b = -(t->m_dM[4] * t->m_dM[10] - t->m_dM[8] * t->m_dM[6]) / det;
	c = (t->m_dM[4] * t->m_dM[9] - t->m_dM[8] * t->m_dM[5]) / det;

	d = -(t->m_dM[1] * t->m_dM[10] - t->m_dM[9] * t->m_dM[2]) / det;
	e = (t->m_dM[0] * t->m_dM[10] - t->m_dM[8] * t->m_dM[2]) / det;
	f = -(t->m_dM[0] * t->m_dM[9] - t->m_dM[8] * t->m_dM[1]) / det;

	g = (t->m_dM[1] * t->m_dM[6] - t->m_dM[5] * t->m_dM[2]) / det;
	h = -(t->m_dM[0] * t->m_dM[6] - t->m_dM[4] * t->m_dM[2]) / det;
	i = (t->m_dM[0] * t->m_dM[5] - t->m_dM[4] * t->m_dM[1]) / det;

	t->m_dM[0] = a;
	t->m_dM[1] = b;
	t->m_dM[2] = c;

	t->m_dM[4] = d;
	t->m_dM[5] = e;
	t->m_dM[6] = f;

	t->m_dM[8] = g;
	t->m_dM[9] = h;
	t->m_dM[10] = i;
}





void mxAdd(mx4 *t, mx4 *it) {
	int i;

	for (i=0; i<16; ++i) {
		t->m_dM[i] += it->m_dM[i];
	}
}






void mxSubtract(mx4 *t, mx4 *it) {
	int i;

	for (i=0; i<16; ++i) {
		t->m_dM[i] -= it->m_dM[i];
	}

}






void mxPreMult(mx4 *t, mx4 *it)
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
				a[n] += t->m_dM[o] * it->m_dM[p];
				++o;
				p+=4; 
			}
			++n;
		}
	}

	for (i=0; i<16; ++i) {
		t->m_dM[i] = a[i];
	}
}








void mxPostMult(mx4 *t, mx4 *it)
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
				a[n] += it->m_dM[o] * t->m_dM[p];
				//a[n] += t->m_dM[o] * it->m_dM[p];
				++o;
				p+=4; 
			}
			++n;
		}
	}

	for (i=0; i<16; ++i) {
		t->m_dM[i] = a[i];
	}
}






void mxScale(mx4 *t, real r)
{
	for(int i=0;i<16;i++)
	{
		t->m_dM[i] *= r;
	}
}






void mxSetToRotation(mx4 *t, real angle, int axis)
{
	mxSetToIdentity(t);
	switch (axis) {
		case 1:
			t->m_dM[5] = (real)cos(angle);
			t->m_dM[6] = 0.0f - (real)sin(angle);
			t->m_dM[9] = (real)sin(angle);
			t->m_dM[10] = (real)cos(angle);
			break;

		case 2:
			t->m_dM[0] = (real)cos(angle);
			t->m_dM[2] = (real)sin(angle);
			t->m_dM[8] = 0.0f - (real)sin(angle);
			t->m_dM[10] = (real)cos(angle);
			break;

		case 3:
            t->m_dM[0] = (real)cos(angle);
			t->m_dM[1] = 0.0f - (real)sin(angle);
			t->m_dM[4] = (real)sin(angle);
			t->m_dM[5] = (real)cos(angle);
			break;
		
	}
}






void mxSetToFreeRotation(mx4 *t, real angle, mVec3 *axis) {
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

	mxSetToIdentity(t); 

	t->m_dM[0] = nx2;
	t->m_dM[1] = nxy-snz;
	t->m_dM[2] = nxz+sny;

	t->m_dM[4] = nxy+snz;
	t->m_dM[5] = ny2;
	t->m_dM[6] = nyz-snx;

	t->m_dM[8] = nxz-sny;
	t->m_dM[9] = nyz+snx;
	t->m_dM[10] = nz2;
}





void mxSetToTranslation(mx4 *t, mVec3 *p) {
	mxSetToIdentity(t);
	t->m_dM[3] = p->x;
	t->m_dM[7] = p->y;
	t->m_dM[11] = p->z;
}



void mxSetToScale(mx4 *t, mVec3 *p) {
	mxSetToIdentity(t);
	t->m_dM[0] = p->x;
	t->m_dM[5] = p->y;
	t->m_dM[10] = p->z;
}


/*
void mxSetPosition(mx4 *t, mVec3 *v)
{
	v->x = t->m_dM[3];
	v->y = t->m_dM[7];
	v->z = t->m_dM[11];
}
*/



void mxSetToLookAt(mx4 *t, mVec3 *eye, mVec3 *coi, mVec3 *up) {
	static real zero = (real)0.0; 
	mVec3 z, y, x; 
	real dot; 

	// zaxis = normal(Eye - coi)
	mvEqSub(&z, eye, coi); 
	mvNormalize(&z); 
	
	// xaxis = normal(cross(Up, zaxis))
	mvEqCross(&x, up, &z); 
	mvNormalize(&x); 
	
	// yaxis = cross(zaxis, xaxis)
	mvEqCross(&y, &z, &x); 

	// at this point, you're just guessing.
	t->m_dM[0] = x.x; 
	t->m_dM[1] = x.y; 
	t->m_dM[2] = x.z; 
	mvDotWith(&x, eye, &dot);
	t->m_dM[3] = -dot; 

	t->m_dM[4] = y.x; 
	t->m_dM[5] = y.y; 
	t->m_dM[6] = y.z; 
	mvDotWith(&y, eye, &dot);
	t->m_dM[7] = -dot; 

	t->m_dM[8]  = z.x; 
	t->m_dM[9]  = z.y; 
	t->m_dM[10] = z.z; 
	mvDotWith(&z, eye, &dot);
	t->m_dM[11] = -dot; 

	t->m_dM[12] = zero; 
	t->m_dM[13] = zero; 
	t->m_dM[14] = zero; 
	t->m_dM[15] = 1.0f;
}



void mxSetToLook(mx4 *t, mVec3 *eye, mVec3 *dir, mVec3 *up) {
	mVec3 coi; 
	
	mvEqAdd(&coi, eye, dir); 
	mxSetToLookAt(t, eye, &coi, up);
}





void mxSetToPerspective(mx4 *t, real fov,	// in y
							real aspect,	
							real znear,
							real zfar)	{ 
	static real two = (real)2.0; 
	static real zero = (real)0.0; 
	real a = fov/two; 
	real b = (real)cos(a); 
	real c = (real)sin(a); 

	real h = b/c;
	real w = h / aspect;

	t->m_dM[0] = two * znear / w; 
	t->m_dM[1] = zero; 
	t->m_dM[2] = zero; 
	t->m_dM[3] = zero; 

	t->m_dM[4] = zero; 
	t->m_dM[5] = two * znear / h; 
	t->m_dM[6] = zero; 
	t->m_dM[7] = zero; 

	t->m_dM[8] = zero; 
	t->m_dM[9] = zero; 
	t->m_dM[10] = zfar/(znear-zfar); 
	t->m_dM[11] = (real)(-1.0); 

	t->m_dM[12] = zero; 
	t->m_dM[13] = zero; 
	t->m_dM[14] = znear*zfar/(znear-zfar); 
	t->m_dM[15] = zero; 
}




// much like the setToLook*'s, but !
// sets the mx to be a positioning mx. 
void mxSetToPosition(mx4 *t, mVec3 *pos, mVec3 *pointing, mVec3 *up) {
	static real zero = (real)0.0; 
	mVec3 z, y, x; 

	// zaxis = dir
	mvCopy(&z, pointing); 
	mvNormalize(&z); 
	
	// xaxis = normal(cross(Up, zaxis))
	mvEqCross(&x, up, &z); 
	mvNormalize(&x); 
	
	// yaxis = cross(zaxis, xaxis)
	mvEqCross(&y, &z, &x); 

	// at this point, you're just guessing.
	t->m_dM[0] = x.x; 
	t->m_dM[1] = y.x; 
	t->m_dM[2] = z.x; 
	t->m_dM[3] = pos->x;

	t->m_dM[4] = x.y; 
	t->m_dM[5] = y.y; 
	t->m_dM[6] = z.y; 
	t->m_dM[7] = pos->y; 

	t->m_dM[8]  = x.z; 
	t->m_dM[9]  = y.z; 
	t->m_dM[10] = z.z; 
	t->m_dM[11] = pos->z;

	t->m_dM[12] = zero; 
	t->m_dM[13] = zero; 
	t->m_dM[14] = zero; 
	t->m_dM[15] = 1.0f;
}




void mxApply(mx4 *t, mVec3 *inp, mVec3 *outp) {
    outp->x = (t->m_dM[0]* inp->x) + (t->m_dM[1]* inp->y) + (t->m_dM[2]* inp->z) + (t->m_dM[3]);
    outp->y = (t->m_dM[4]* inp->x) + (t->m_dM[5]* inp->y) + (t->m_dM[6]* inp->z) + (t->m_dM[7]);
    outp->z = (t->m_dM[8]* inp->x) + (t->m_dM[9]* inp->y) + (t->m_dM[10]* inp->z) + (t->m_dM[11]);
}




void mxApply2(mx4 *t, mVec3 *inp) {
    mVec3 spare; 
    mxApply(t, inp, &spare); 
    mvCopy(inp, &spare); 
}

























