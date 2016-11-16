
/* Neal McDonald 2009
 
	Excuse: 
		Bounding spheres make intersection EZ.
		Also visibility culling. 
		Also LOD control.
	These, though, technically, are, like, not spheres. 
	
	Operates on 3d float data only. 
	*/




#ifndef M_BBOX3_CLASS
#define M_BBOX3_CLASS


/////////////////////////////////
#include "mVec3.h"
/////////////////////////////////


typedef struct {
	real		xlo, xhi, ylo, yhi, zlo, zhi;		// extents
} mBBox3, *mBBox3Ptr; 


// Simple copy
void mBB3Copy(mBBox3 *it); 

// modify the BB to ensure that it contains one more point
void mBB3AddPt(mVec3 *data);

// TRUE if it intersects this BB, FALSE otherwise
bool mBB3Touches(mBBox3 *it); 

///////////////////
// ops on 3D points

// TRUE if data is within BB, FALSE otherwise
bool mBB3Contains(mVec3 *data); 

// TRUE if all data points within BB, FALSE otherwise
bool mBB3ContainsAll(const int ptCt, mVec3 *data); 

// TRUE if one of the data points within BB, FALSE otherwise
bool mBB3ContainsAny(const int ptCt, mVec3 *data); 


#endif // M_BBOX_CLASS

