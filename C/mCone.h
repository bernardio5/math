
/* Neal McDonald 2009

 Excuse: 
		Bounding cones are good for 
		visibility culling of entities. 
 
		They are also used to describe cameras.

		If their radii are set to 0, they're 
		rays. 

		Transformations can also be extracted from them.
		This entails an "up" vector, and a default
		orientation (at origin, looking up +z, y=up).
		

	Usually, startRadius is 0. 
	"dr" is the ratio of radius to height--
	dr==1 => a right cone.

	dr==infinite => cone's inner angle is >180 
		in these cases, a lot of routines won't work. 



	There is a eyePoint, m_fSource.
	There is a view direction, m_fDir
	There is a center of interest, m_fCOI. 
	
	The formula for specifying a camera uses a point and a center of interest. 
	If you set dir, COI==p+dir. 
	If you set COI, dir==norm(COI-dir)
	If you set pos, COI stays the same and dir==norm(COI-dir)
	*/




#ifndef M_BCONE_CLASS
#define M_BCONE_CLASS

#import "mx4.h"
// #include "mBSphere.h"



typedef struct {
	mVec3		m_cSource,		// eyept
				m_cDir,			// look direction
				m_cUp,			// 
				m_cCOI;			// look target
	real		m_fStartRadius, 
				m_fDr, 
				m_fInner; 
	mx4			m_cMx;			// use to set camera
} mCone, *mConePtr; 

// This sets up the camera view cone basically because
// eye is assigned to m_cSource
void mcThreePoint(mConePtr c, mVec3 *eye, mVec3 *coi, mVec3 *up, real rad, real dr);
	
// Sets the position and computes the look-at for this cone
void mcSetPosition(mConePtr c, mVec3 *p); 

// Direction is set, COI is computed, look-at is computed
void mcSetDirection(mConePtr c, mVec3 *d);

// COI is set, dir is computed, look-at is computed
void mcSetCOI(mConePtr c, mVec3 *p);

// Position is set, look-at is computed
void mcSetUp(mConePtr c, mVec3 *p); 

// Up is computed by p - source, look-at is computed
void mcSetUpPt(mConePtr c, mVec3 *p); 

// Dr is set, inner angle is computed
void mcSetDr(mConePtr c, const real x);


// get the transformational mx that is the xform embodied by this cone. 
// void mcGetMx(mConePtr *c, mx4 *it) { it->copy(m_cMx); }  

// put the point at source+t*dir in res
void mcRayPt(mConePtr c, const real t, mVec3 *res); 

// quick op to return the t (as above) of the closest point
// on the center ray to a given point. A surprisingly useful op!
void mcClosestPtParam(mConePtr c, mVec3 *p, real *res);

// does this BC touch that BS?
//bool touches(mConePtr c, mBSphere *it); 
//bool contains(mConePtr c, mBSphere *it); 

// for 3D points
bool contains(mConePtr c, mVec3 *res); 
bool containsAll(mConePtr c, const int ptCt, mVec3 *data); 
bool containsAny(mConePtr c, const int ptCt, mVec3 *data); 

// bool intersects(mConePtr *c, mCone *it); 





#endif // M_BCONE_CLASS

