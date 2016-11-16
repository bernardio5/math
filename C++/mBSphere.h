
/* Neal McDonald 2009
	Excuse: 
		Bounding spheres make intersection EZ.
		Also visibility culling. 
		Also LOD control.
	
	Operates on 3d float data only. 
	*/




#ifndef M_BSPHERE_CLASS
#define M_BSPHERE_CLASS

#include "mx4.h" 


class  mBSphere {
private:
	real		m_fRadSq;
public:
	real		m_fRadius;
	mVec3		m_cCenter; 

	mBSphere(); 
	~mBSphere();
	void copy(mBSphere *it); 
	
	// Resets this, and radius-squared is computed
	void set(mVec3 *center, const real radius);

	// Compute the BS that contains these points
	void compute(const int pointCount, mVec3 *data);
	
	// NOT IMPLEMENTED
	void transform(mx4 *it); 

	// Modify the BS to insure that it contains one more point
	void addPt(mVec3 *data);

	// Enlarge BS to contain a second BS
	void combine(mBSphere *it);

	// Determine whether "it" is touching us again - basic intersection test
	bool touches(mBSphere *it); 
	
	// TRUE if BS is entirely contained within this, FALSE otherwise
	bool contains(mBSphere *it);
	
	// ops on 3D points
	bool contains(mVec3 *data); 
	bool containsAll(const int ptCt, mVec3 *data); 
	bool containsAny(const int ptCt, mVec3 *data); 

};




#endif // M_BSPHERE_CLASS

