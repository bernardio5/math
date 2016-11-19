// Neal McDonald 2009



/* 		Our matrices are in column-major form, so:

							R,W			R,W			R,W			R,W
							---------------------------------------------
		0  1  2  3		|	0,0 -> 0	1,0 -> 4	2,0 -> 8	3,0 -> 12
		4  5  6  7		|	0,1 -> 1	1,1 -> 5	2,1 -> 9	3,1 -> 13
		8  9  10 11		|	0,2 -> 2	1,2 -> 6	2,2 -> 10	3,2 -> 14
		12 13 14 15		|	0,3 -> 3	1,3 -> 7	2,3 -> 11	3,3 -> 15


	*/

#ifndef MX4_TYPE
#define MX4_TYPE

#include "mVec3.h"


class mx4 {
public:
	real m_dM[16];

	mx4();
	virtual ~mx4();
	void copy(mx4 *it);

	// operations on m_dM
	void setToIdentity();
	void transpose();
	//invert the top left 3x3 sub-matrix. Blah.
	void invert();
	// m_dM += it
	void add(mx4 *it);
	// m_dM -= it
	void subtract(mx4 *it);
	// m_dM = m_dM * it
	void preMult(mx4 *it);
	// m_dM = it * m_dM
	void postMult(mx4 *it);
	// multiply each element in m_dM by r
	void scale(real r);

	//out = this * in
	void preMultSingle(mVec3 *in, mVec3 *out);
	void preMultMany(const int ct, mVec3 *in, mVec3 *out);

	//out = in * this
	void postMultSingle(mVec3 *in, mVec3 *out);

	// setting m_dM
	void setToRotation(const real angle, char axis);
	void setToRotation(const real angle, mVec3 *axis);
	void setToTranslation(mVec3 *p);
	void setToScale(mVec3 *p);

	void setToLookAt(mVec3 *eye, mVec3 *coi, mVec3 *up);
	void setToLook(mVec3 *eye, mVec3 *dir, mVec3 *up);
	void setToPerspective(	const real fov,
							const real aspect,
							const real znear,
							const real zfar);

	// moves and orients an object.
	void setToPosition(mVec3 *pos, mVec3 *pointing, mVec3 *up);

	void getPosition(mVec3 *v);
};

#endif

// Neal McDonald 1/1/2005