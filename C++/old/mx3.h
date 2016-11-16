

/* Excuse: 
		linear algebra library

		Our matrices are in column-major form, so:

							R,W			R,W			R,W		
							---------------------------------
		0  1  2  		|	0,0 -> 0	1,0 -> 3	2,0 -> 6	
		3  4  5  		|	0,1 -> 1	1,1 -> 4	2,1 -> 7
		6  7  8 		|	0,2 -> 2	1,2 -> 5	2,2 -> 8								
							
	*/

#ifndef MX3_TYPE
#define MX3_TYPE

#include "mVec3.h" 


class mx3 {
public:
	real m_dM[9];

	mx3();
	virtual ~mx3();
	void copy(mx3 *it); 

	// operations on m_dM
	void setToIdentity();
	void transpose();

	// m_dM += it
	void add(mx3 *it);
	// m_dM -= it
	void subtract(mx3 *it);
	// m_dM = m_dM * it
	void preMult(mx3 *it);
	// m_dM = it * m_dM
	void postMult(mx3 *it); 
	// multiply each element in m_dM by r
	void scale(real r);

	//out = this * in
	void preMultSingle(mVec3 *in, mVec3 *out);
	void preMultMany(const int ct, mVec3 *in, mVec3 *out);
	
	//out = in * this
	void postMultSingle(mVec3 *in, mVec3 *out);

	// setting m_dM
	void setToRotation(const real angle);
	void setToTranslation(mVec3 *p);
	void setToScale(mVec3 *p);

};

#endif // MX4_TYPE