
/* Neal McDonald 2009
 
	Excuse: 
		Bounding spheres make intersection EZ.
		Also visibility culling. 
		Also LOD control.
	These, though, technically, are, like, not spheres. 
	
	Operates on 3d float data only. 
	*/




#ifndef M_BBOX2_CLASS
#define M_BBOX2_CLASS


/////////////////////////////////
#include "mVec3.h"
/////////////////////////////////


typedef struct {
	real		xlo, xhi, zlo, zhi;		// extents
} mBBox2, *mBBox2Ptr; 


// Simple copy
void mBB2Copy(mBBox2 *it); 

// modify the BB to ensure that it contains one more point
void mBB2AddPt(mVec3 *data);

// TRUE if it intersects this BB, FALSE otherwise
bool mBB2Touches(mBBox2 *it); 

///////////////////
// ops on 3D points

// TRUE if data is within BB, FALSE otherwise
bool mBB2Contains(mVec3 *data); 

// TRUE if all data points within BB, FALSE otherwise
bool mBB2ContainsAll(const int ptCt, mVec3 *data); 

// TRUE if one of the data points within BB, FALSE otherwise
bool mBB2ContainsAny(const int ptCt, mVec3 *data); 


#endif // M_BBOX_CLASS

