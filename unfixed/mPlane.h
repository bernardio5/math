

/* Neal McDonald 2009
	Excuse: 
		The mPlane has all kindsa uses. 

	1) A plane can represent simplified geometry. 
		Making intersection calculations fast.

	2) A plane can represent a transformation, 
		if there is a single vector in the plane that orients it. 

	3) If n is the plane's normal, and p is any point in the plane, 
		and c=n dot p, then a point x lies at distance c-(x dot n)
		from the plane. 

	*/

#ifndef M_PLANE_TYPE
#define M_PLANE_TYPE

#include "mx4.h" 


class mPlane {
private: 
	mVec3	n, p;	// defines the plane
	mVec3	d;		// orients the plane; allows plane to be xform. 
					//		default: if n==y, dir=-x, ow dir=y.
	bool	dNotSet; // if true, call makeUpD whenever n is set. 
	real	c;		// norm(n) dot p

	void makeUpD();	// generate a semi-logical d from n

public:

	mPlane(); 
	virtual ~mPlane();
	void copy(mPlane *it); 

	void setToIdentity();
	void setP(mVec3 *newp);
	void setN(mVec3 *newn); 
	void setD(mVec3 *newd);	// Bah Jove, that vectah is completleh... !

	// 3 points *can* define a plane-- or a line.  careful!
	void setFrom3(mVec3 *p1, mVec3 *p2, mVec3 *p3); 

	// use this fn to set an xForm's m_dM
	void getMx(mx4 *m);	

	// result>0 => above, <0 => below. 
	void distanceFrom(mVec3 *p2, real *result); 

};

#endif // M_PLANE_TYPE