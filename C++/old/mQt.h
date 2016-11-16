

/* (c) Greenhouse Software 2002
	Excuse: 
		The cutie class embodies transformations
		constisting of a rotation (via mQuat) plus translation. 
		So, no scaling, but faster, and good interpolation
		
		Rotation done first, then translation. 
	*/

#ifndef M_QT_TYPE
#define M_QT_TYPE

#include "mVec3.h" 
#include "mQuat.h" 


class mQt {
public:
	mQuat q;
	mVec3 t;

	mQt(); 
	virtual ~mQt();
	void copy(mQt *it); 

	void setToIdentity();
/*
	void preMerge(mQt *it);		// this = it(this)
	void postMerge(mQt *it);	// this = this(it)
*/
	void setQ(mQuat *q2);
	void setQ(const real angle, mVec3 *axis); 
	void setT(mVec3 *p);

	void lerp(mQt *qt1, mQt *qt2, real t); 
/*
	// ops on vectors-- used by mVertList
	void moveSingle(mVec3 *in, mVec3 *out);
	void moveMany(const int ct, mVec3 *in, mVec3 *out);
*/
};

#endif // M_QT_TYPE