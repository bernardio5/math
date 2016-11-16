
/* Copyright (C) 2002 Greenhouse Software.  
	All Rights Reserved.
	Excuse: 
		Bounding cones are good for 
		visibility culling of entities. 

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


/////////////////////////////////

#include "mVec3.h"
#include "mx4.h"
#include "mBSphere.h"

/////////////////////////////////



class mBCone {
private:
	mVec3		m_cSource, 
				m_cDir, 
				m_cUp, 
				m_cCOI;
	real		m_fStartRadius,
				m_fDr,
				m_fInner; 
	mx4			m_cMx;

public:
	mBCone(); 
	~mBCone();
	
	// This sets up the camera view cone basically because
	// eye is assigned to m_cSource
	void threePoint(mVec3 *eye, mVec3 *coi, mVec3 *up, real rad, real dr);
	
	// Sets the position and computes the look-at for this cone
	void setPosition(mVec3 *p); 

	// Direction is set, COI is computed, look-at is computed
	void setDirection(mVec3 *d);

	// COI is set, dir is computed, look-at is computed
	void setCOI(mVec3 *p);

	// Position is set, look-at is computed
	void setUp(mVec3 *p); 

	// Up is computed by p - source, look-at is computed
	void setUpPt(mVec3 *p); 

	// Dr is set, inner angle is computed
	void setDr(const real x);

	// NOT IMPLEMENTED!!!!
	void transform(mx4 *it); 
	
	// Simple accessors
	void getPosition(mVec3 *p); 
	void getDirection(mVec3 *p); 
	void getCOI(mVec3 *p); 
	void getUp(mVec3 *p); 
	void getRadius(real *start, real *dr); 


	// get the transformational mx that is the xform embodied by this cone. 
	void getMx(mx4 *it); 

	// put the point at source+t*dir in res
	void rayPt(const real t, mVec3 *res); 
	
	// quick op to return the t (as above) of the closest point
	// on the center ray to a given point. A surprisingly useful op!
	void closestPtParam(mVec3 *p, real *res);

	// does this BC touch that BS?
	bool touches(mBSphere *it); 
	bool contains(mBSphere *it); 

	// for 3D points
	bool contains(mVec3 *res); 
	bool containsAll(const int ptCt, mVec3 *data); 
	bool containsAny(const int ptCt, mVec3 *data); 

	bool intersects(mBCone *it); 

};




#endif // M_BCONE_CLASS

