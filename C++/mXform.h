
/* 	mXforms generate transform matrices that vary continuously.

	This mx is is the mx4 "m_cR," R for Results. 


	Every puppet has a mXform that serves as its 
	position/orientation mx. 

	Animatable models will have one additional mXform
	associated with each of its joints. 
		
	These joint xForms will be in a tree. 
		
 mxForms used to be a class tree.
 now they are one structure and a lot of convenience functions
 
	Implementation details:
	
	mXforms have a "version" number, which is 
	the last time they were updated. So do mVerts.
	This is to allow you from having to do the same
	calculation twice. 


	m_cR is where the xForm's current mx is stored. 
	m_cM is a scratch area-- m_cR = m_cM * parent->m_cR; 
  

	*/




#ifndef M_XFORM_CLASS
#define M_XFORM_CLASS

/////////////////////////////////

#include "mVec3.h" 
#include "mx4.h" 
#include "mBCone.h"
#include "mBBox.h"
#include "mPlane.h"
#include "mLongPath.h"
#include "physManager.h" 
#include "timeManager.h"
#include "grEnv.h"

#include "xfPeriodic.h"

/////////////////////////////////

/*******
This is a virtual class that defines an interface.
*/

class mXform
{
protected:
	physManager *m_cPhysMgr;
	mXform	*m_cParent;		// xforms informing this one

	int		m_iMVersion;	// incremented when M changes 
	int		m_iRVersion;	// incremented each time m_dR changes
public:
	mVec3   m_cP, m_cD,  m_cU; 
	mx4		m_cM;			// local xform
	mx4		m_cR;			// output-- combination of parent and M
							// used by "move" and "moveMany"	

	mXform(); 
	virtual ~mXform();

	//Common interface
	void setParent(mXform *parent);
	mXform *getParent();
	void setMVersion(int v);
	int getMVersion();
	void setRVersion(int v);
	int getRVersion();

	// ops on mVec3's 
	void move(mVec3 *in, mVec3 *out);
	void moveMany(const int ct, mVec3 *in, mVec3 *out);

	// update m_cR to reflect whatever else is going on
	virtual void update();
	virtual mXform *clone();
};


///////////////////////
// mXform Subclasses //
///////////////////////

// Orientation toward the current camera
//If there is a parent transform, it will use its position.
//Otherwise, it will use the one supplied.
class mXformBillboard: public mXform
{
protected:
	entManager *m_pEntMgr;
	mVec3 m_cRight;
public:
	//These are the methods this subclass overrides, defining behaviors for itself.
	mXformBillboard(entManager *em, mVec3 *pos);
	~mXformBillboard();

	mXform *clone();
	void update();
};



// p from p1 pos, d from p2-p1, scaled along y to the distance between the two points
class mXformDynamicStrut: public mXform
{
protected:
	physManager *m_cPhysMgr;
	int		m_iP1, m_iP2;
public:
	//These are the methods this subclass overrides, defining behaviors for itself.
	mXformDynamicStrut(physManager *pm, const int p1, const int p2);
	~mXformDynamicStrut();

	mXform *clone();
	void update();
};

// p from p1.p, dir from p1.d, scaled along y according to how fast it is travelling
class mXformInverseLorentz: public mXform
{
protected:
	physManager *m_cPhysMgr;
	int		m_iP1;
public:
	//These are the methods this subclass overrides, defining behaviors for itself.
	mXformInverseLorentz(physManager *pm, const int p1);
	~mXformInverseLorentz();

	mXform *clone();
	void update();
};

//scales by the amount specified
class mXformScaler: public mXform
{
protected:
	mVec3 m_cScale;
public:
	//These are the methods this subclass overrides, defining behaviors for itself.
	mXformScaler(mVec3 *scale);
	~mXformScaler();

	mXform *clone();
	void update();
};

// p from part pos, d from part dir, u from part u
class mXformScanner1: public mXform
{
protected:
	physManager *m_cPhysMgr;
public:
	int		m_iP1;

	//These are the methods this subclass overrides, defining behaviors for itself.
	mXformScanner1(physManager *pm, const int p1);
	~mXformScanner1();

	mXform *clone();
	void update();
};

// p from p1 pos, d from p2-p1, u=y 
class mXformScanner2: public mXform
{
protected:
	physManager *m_cPhysMgr;
public:
	int		m_iP1, m_iP2;

	//These are the methods this subclass overrides, defining behaviors for itself.
	mXformScanner2(physManager *pm, const int p1, const int p2);
	~mXformScanner2();

	mXform *clone();
	void update();
};

// p from p1, d from p2-p1, u from p3-p1
class mXformScanner3: public mXform
{
protected:
	physManager *m_cPhysMgr;
public:
	int		m_iP1, m_iP2, m_iP3;

	//These are the methods this subclass overrides, defining behaviors for itself.
	mXformScanner3(physManager *pm, const int p1, const int p2, const int p3);
	~mXformScanner3();

	mXform *clone();
	void update();
};

//Starts at the specified place, moves along a specified spline at the specified speed, and orients along the spline
class mXformSplineMover: public mXform
{
protected:
	mLongPath	m_pPath;
	real		m_rSpeed;
	real		m_rStart;
	timeManager *m_cTimeMgr;
public:
	//These are the methods this subclass overrides, defining behaviors for itself.
	mXformSplineMover(mLongPath *path, const real speed, const real start, timeManager *tm);
	~mXformSplineMover();

	mXform *clone();
	void update();
};


//Starts at the specified place, moves along a specified spline at the specified speed, and orients along the spline
class mXformTubeMover: public mXform
{
protected:
	mTube		*m_pTube;
	mVec3		m_cSpeed, m_cStart;
	timeManager *m_cTimeMgr;
public:
	//These are the methods this subclass overrides, defining behaviors for itself.
	mXformTubeMover(mTube *tube, mVec3 *params, mVec3 *dparams, timeManager *tm);
	~mXformTubeMover();

	mXform *clone();
	void update();
};

//Starts at the specified place, moves along a specified spline at the specified speed, and orients along the spline
class mXformTubeMoverTest: public mXform
{
protected:
	mTube		*m_pTube;
	mVec3		m_cSpeed, m_cStart;
	timeManager *m_cTimeMgr;
	int			section;
public:
	//These are the methods this subclass overrides, defining behaviors for itself.
	mXformTubeMoverTest(mTube *tube, mVec3 *params, mVec3 *dparams, timeManager *tm);
	~mXformTubeMoverTest();

	mXform *clone();
	void update();
};





// p from p1 pos, d from p2-p1, scaled along y to the distance between the two points
class mXformStaticStrut: public mXform
{
protected:
	mVec3	m_cPos, m_cPos2;
public:
	//These are the methods this subclass overrides, defining behaviors for itself.
	mXformStaticStrut(mVec3 *pos, mVec3 *pos2);
	~mXformStaticStrut();

	mXform *clone();
	void update();
};

//stays put at the point specified.
class mXformStatic: public mXform
{
public:
	//These are the methods this subclass overrides, defining behaviors for itself.
	mXformStatic(mVec3 *p, mVec3 *d, mVec3 *u);
	~mXformStatic();

	mXform *clone();
	void update();
};


//spins about the X axis.
class mXformSpinX: public mXform
{
protected:
	timeManager	*m_cTimeMgr;
	real		speed;
public:
	//Event manager, speed of rotation
	mXformSpinX(timeManager *tm, real s);
	~mXformSpinX();

	mXform *clone();
	void update();
};

//spins about the Y axis.
class mXformSpinY: public mXform
{
protected:
	timeManager	*m_cTimeMgr;
	real		speed;
public:
	//Event manager, speed of rotation
	mXformSpinY(timeManager *tm, real s);
	~mXformSpinY();

	mXform *clone();
	void update();
};

//spins about the Z axis.
class mXformSpinZ: public mXform
{
protected:
	sndAnalysis *m_cAnalysis;
	sndBeatName	m_iBeatName;
	real		speed;
public:
	//Event manager, speed of rotation
	mXformSpinZ(sndAnalysis *sm, const sndBeatName n,  real s);
	~mXformSpinZ();

	mXform *clone();
	void update();
};

///////////////////////
// SPRITE TRANSFORMS //
///////////////////////

//SPRITES ONLY: Makes a sprite stay put at the specified coördinate.
class mXformSpriteStatic: public mXform
{
protected:
	mVec3	loc;
public:
	mXformSpriteStatic(mVec3 *pos);
	~mXformSpriteStatic();

	mXform *clone();
	void update();
};

class mXformSpriteBounceY: public mXform
{
protected:
	sndAnalysis *m_cAnalysis;
	sndBeatName	m_iBeatName;
	int			m_iDistance;		//how far the sprite will bounce
public:
	//Event manager, speed of rotation
	mXformSpriteBounceY(sndAnalysis *sa, const sndBeatName n, int distance);
	~mXformSpriteBounceY();

	mXform *clone();
	void update();
};

class mXformSpriteLERP: public mXform
{
protected:
	int		m_iXCenter;
	int		m_iYCenter;
	mVec3	lerpLine; // line along which to interpolate.
	real	*m_pReal;
public:
	mXformSpriteLERP(const int xAnchor, const int yAnchor, mVec3 *line, real *value);
	~mXformSpriteLERP();

	mXform *clone();
	void update();
};

class mXformSpriteWorldToMap: public mXform
{
protected:
	physManager		*m_cPhysMgr;
	int				m_iPartIdx;
	int				m_iMapWidth;
	int				m_iMapHeight;
	mBBox2			m_cLvlExtents;

public:
	mXformSpriteWorldToMap(physManager *pm, const int particleIdx, int mapWidth, int mapHeight, mBBox2 *lvlExtents);
	~mXformSpriteWorldToMap();

	mXform *clone();
	void update();
};

class mXformSpriteWorldToScreen: public mXform
{
protected:
	grEnv			*m_cGrEnv;
	physManager		*m_cPhysMgr;
	int				m_iPartIdx;
public:
	mXformSpriteWorldToScreen(grEnv *gr, physManager *pm, const int particleIdx);
	~mXformSpriteWorldToScreen();

	mXform *clone();
	void update();
};


// use to make a sprite that moves along a vector
// in response to the value of an integer
class mXformSpriteSlider: public mXform
{
protected:
	int				*m_pIndex; 
	int				m_ix, m_iy, m_idx, m_idy; 
public:
	mXformSpriteSlider(int *idx, int x, int y, int dx, int dy);
	~mXformSpriteSlider() {}

	mXform *clone();
	void update();
};



class mXformPTranslate: public mXform
{
protected:
	mVec3		m_cP, m_cV;
	xfPeriodic	m_cPrd;
	
public:
	mXformPTranslate(mVec3 *p, mVec3 *v, xfPeriodic *prd);
	~mXformPTranslate() {}

	mXform *clone();
	void update();
};

class mXformPRotate: public mXform
{
protected:
	mVec3		m_cRot, m_cV;
	xfPeriodic	m_cPrd;
	
public:
	mXformPRotate(mVec3 *rot, mVec3 *v, xfPeriodic *prd);
	~mXformPRotate() {}

	mXform *clone();
	void update();
};

class mXformPScale: public mXform
{
protected:
	mVec3		m_cP, m_cV;
	xfPeriodic	m_cPrd;
	
public:
	mXformPScale(mVec3 *p, mVec3 *v, xfPeriodic *prd);
	~mXformPScale() {}

	mXform *clone();
	void update();
};

class mXformPMoveTo: public mXform
{
protected:
	mVec3		m_cP1, m_cP2, m_cV;
	xfPeriodic	m_cPrd;
	
public:
	mXformPMoveTo(mVec3 *p1, mVec3 *p2, mVec3 *v, xfPeriodic *prd);
	~mXformPMoveTo() {}

	mXform *clone();
	void update();
};


///////////////////////////
// RIGID BODY TRANSFORMS //
///////////////////////////
/*
//RIGID BODIES ONLY: Makes a puppet translate and rotate to the appropriate place in space, given a rigid body.
class mXformRigidBodyScanner: public mXform
{
protected:
	physManager *m_cPhysMgr;
	mx4		translationMx;
public:
	int		m_iRigidBody;

	//These are the methods this subclass overrides, defining behaviors for itself.
	mXformRigidBodyScanner(physManager *pm, const int body);
	~mXformRigidBodyScanner();

	mXform *clone();
	void update();
};
*/


#endif // M_TRANSFORM_CLASS