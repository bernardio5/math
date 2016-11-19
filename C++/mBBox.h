#ifndef M_BBOX_CLASS
#define M_BBOX_CLASS

// Neal McDonald 2009

// Bounding Boxes

/////////////////////////////////
#include "mVec3.h"
/////////////////////////////////



class  mBBox {
public:
	real		xlo, xhi, zlo, zhi;
	mBBox(); 
	~mBBox();
	
	// Simple copy
	void copy(mBBox *it); 
	
	void clear(mVec3 *data); 

	// modify the BB to ensure that it contains one more point
	void addPt(mVec3 *data);
	
	// TRUE if it intersects this BB, FALSE otherwise
	bool touches(mBBox *it); 
	
	///////////////////
	// ops on 3D points

	// TRUE if data is within BB, FALSE otherwise
	bool contains(mVec3 *data); 
	
	// TRUE if all data points within BB, FALSE otherwise
	bool containsAll(const int ptCt, mVec3 *data); 
	
	// TRUE if one of the data points within BB, FALSE otherwise
	bool containsAny(const int ptCt, mVec3 *data); 
};



class  mBBox3 {
public:
	real		xlo, xhi, ylo, yhi, zlo, zhi;
	mBBox3(); 
	~mBBox3();
	
	// Simple copy
	void copy(mBBox3 *it); 
	
	// Sets this BB's diagonals
	void setP1P2(mVec3 *p1, mVec3 *p2);

	// TRUE if it intersects this BB, FALSE otherwise
	bool touches(mBBox3 *it); 

	///////////////////
	// ops on 3D points
	
	// TRUE if data is within BB, FALSE otherwise
	bool contains(mVec3 *data); 
	
	// TRUE if all data points within BB, FALSE otherwise
	bool containsAll(const int ptCt, mVec3 *data); 
	
	// TRUE if one of the data points within BB, FALSE otherwise
	bool containsAny(const int ptCt, mVec3 *data); 
};




#endif // M_BBOX_CLASS

