

/* Neal McDonald 2009
 Excuse: 
 linear algebra library
 
 Our matrices are in column-major form, so:
 
 				R,W			R,W			R,W			R,W
 ---------------------------------------------
 0  1  2  3	|	0,0 -> 0	1,0 -> 4	2,0 -> 8	3,0 -> 12
 4  5  6  7	|	0,1 -> 1	1,1 -> 5	2,1 -> 9	3,1 -> 13
 8  9  10 11|	0,2 -> 2	1,2 -> 6	2,2 -> 10	3,2 -> 14
 12 13 14 15|	0,3 -> 3	1,3 -> 7	2,3 -> 11	3,3 -> 15
 
 */

#ifndef MX4_TYPE
#define MX4_TYPE 1

#import "mVec3.h" 



typedef struct {
	real m_dM[16];
} mx4; 


void mxCopy(mx4 *t, mx4 *it); 

// operations on m_dM
void mxSetToIdentity(mx4 *t);
void mxTranspose(mx4 *t);
//invert the top left 3x3 sub-matrix. Blah.
void mxInvert(mx4 *t);
// m_dM += it
void mxAdd(mx4 *t, mx4 *it);
// m_dM -= it
void mxSubtract(mx4 *t, mx4 *it);
// m_dM = m_dM * it
void mxPreMult(mx4 *t, mx4 *it);
// m_dM = it * m_dM
void mxPostMult(mx4 *t, mx4 *it); 
// multiply each element in m_dM by r
void mxScale(mx4 *t, real r);

//out = this * in   --- ok if out and in are the same object
//void mxPreMultSingle(mx4 *t, mVec3 *in, mVec3 *out);
//void mxPreMultMany(mx4 *t, int ct, mVec3 *in, mVec3 *out);

//out = in * this
//void mxPostMultSingle(mx4 *t, mVec3 *in, mVec3 *out);

// setting m_dM
void mxSetToRotation(mx4 *t,  real angle, int axis);
void mxSetToFreeRotation(mx4 *t,  real angle, mVec3 *axis); 
void mxSetToTranslation(mx4 *t, mVec3 *p);
void mxSetToScale(mx4 *t, mVec3 *p);

void mxSetToLookAt(mx4 *t, mVec3 *eye, mVec3 *coi, mVec3 *up);
void mxSetToLook(mx4 *t, mVec3 *eye, mVec3 *dir, mVec3 *up);
void mxSetToPerspective(mx4 *t, real fov,	
						real aspect, real znear, real zfar);

// moves and orients an object. 
void mxSetToPosition(mx4 *t, mVec3 *pos, mVec3 *pointing, mVec3 *up);


void mxApply(mx4 *t, mVec3 *inp, mVec3 *outp); 
void mxApply2(mx4 *t, mVec3 *inp);




#endif