/* Copyright (C) 2002 Greenhouse Software.  
	All Rights Reserved.

	This is the physics manager. 

	The physics manager runs the particle system
	for the game. It also contains
	the scripting mechanism for particle systems. 

	doTick does a simulation step for the psys
	timesteps that are too small do nothing; 
	otherwise there'd be a potential 
	divide by 0 error. 
  
	physMgr also owns a single particle, which is used
	to initialize the psysen. You can do whatever you like 
	with it; it's just a convenience. 

	*/


#ifndef PHYS_MANAGER_CLASS
#define PHYS_MANAGER_CLASS

///////////////////////////////////////////

#include "baseManager.h"

#include "errManager.h"
#include "fileManager.h"

#include "evnController.h"
#include "mVec3.h"
#include "mx4.h"
#include "mQuat.h"
#include "colManager.h"

///////////////////////////////////////////

#define PHYS_MAX_FORCES 500
#define PHYS_MAX_PARTS 500

// psys uses particles. almost no other use for particles. 

class physElement {
public:
	mVec3		p, v, f, d, u, op, ov, od, ou;
	real		mass, r, birthday;
	int			iTubeIdx;
	bool		active;

	physElement();
	void copy(physElement *p);
};

/* I'll admit this is pretty spooky & difficult.
	Forces need to be exerted in different ways, and 
	no, I haven't exhausted all the different ways forces
	could be exerted. 

	Mainly for the sake of execution speed-- if you have a spring,
	that's only affecting two particles, and it knows which two,
	so run it once and you're done. If you have an electrical charge,
	it affects all particles that have a charge-- you need distance 
	information for every particle pair. Don't run the spring force
	in the electrical force's loop, mmmkay?

	So, a force's style tells psys when and how to exert the force. 

	Not all "forces" move particles; some types serve as customizable 
	interfaces that read information from one or more particles. 
	For instance: the camera force reads the positions of 2 or 3 particles
	and returns a matrix. 
	For instance: the puppet force reads one particle and returns a mx

	*/

typedef enum {
	PHYS_FSTYLE_ERROR = 0,	
	PHYS_FSTYLE_WIDE,		
	// applied without arguments, to all parts
	// doIt fn takes pointer to part it should affect
	// O(nparts)

	PHYS_FSTYLE_NSQUARED,	
	// forces applied from all to all
	// doit takes ptrs to two target particles, dy, distance
	// O(nparts*nparts); use sparingly

	PHYS_FSTYLE_SINGULAR,	
	// same as "wide", but applied only to a couple parts.
	// doIt takes list of all parts, has indices
	// O(1) fast!
	
	PHYS_FSTYLE_CONST_SING,
	// constraint, after integrate, applied to individuals,
	// doIt takes list of all parts, has indices
	// O(1) fast!

	PHYS_FSTYLE_CONST_WIDE,	
	// constraint, after integration, applied to all
	// doIt fn takes pointer to part it should affect
	// O(nparts); 

	PHYS_FSTYLE_LAST
} physForceStyle;


class physForce {
public :
	physForce() {}
	~physForce() {}

	// init(); -- variable arguements list

	// switchboard functions that call the fns above

	// for _FRICTIONAL force styles
	virtual void doIt(physElement *a, const real b) {}

	// for _ELECTRICAL force styles
	virtual void doIt(const int p1, const int p2, physElement *ps, 
		const real distance, const real dt) {}

	// for _SPRING force styles
	virtual void doIt(physElement *a, const real t, const real dt) {}

	// to route input from evnMgr to forces
	virtual void takeIt(mVec3 *p) {} 
};




#define PHYS_MAX_WIDE 50
#define PHYS_MAX_NSQUARED 50
#define PHYS_MAX_SINGULAR 1000
#define PHYS_MAX_CONST_SING 1000
#define PHYS_MAX_CONST_WIDE 50

#define PHYS_MAX_OF_MAX PHYS_MAX_SINGULAR


class physManager : public baseManager {
private:
	fileManager			*m_cFileMgr;
	grManager			*m_cGrMgr;

	int					m_iPCount;				// current cts of parts/forces
	evnTime				m_dDT;
	evnTime	m_dT;

	physElement			*m_cElements;
	physForce			**m_pForces[PHYS_MAX_WIDE];

	int					m_iLimits[PHYS_FSTYLE_LAST]; // max allowed number of each force type
	int					m_iCounts[PHYS_FSTYLE_LAST]; // number existing of each force type
	short int			*m_iMaps[PHYS_FSTYLE_LAST];	 // maps indices to entries in force arrays,
	colManager		*m_cColMgr;
													 // so force arrays can be compacted
	void doForces();
	void doConstraints();
	void doMove();
	
	void getIndex(int *result, const physForceStyle type); 
	void spinIndex(const int index, const physForceStyle st, int *spun); 
	void unspinIndex(const int spun, physForceStyle *st, int *unspun);

	// helper function for all force creation member fns. 
	void addForce(int *result, const physForceStyle s, physForce *f); 
public:

	int		m_iSimTicks;
	bool	m_bActive; // whether each entMgr doTick should operate

	// this is an example particle
	// set it & use it to init a psys
	physElement	m_cPart;


	// nPsys = the number of psys to create. 
	physManager(errManager *theErrMgr,
				fileManager *theFileManager);
	virtual ~physManager();


	// take a max part count and use m_cPart to 
	//		init the current psys. 
	void initElements(const int mP);

	// uses m_cPart, adds a copy, returns
	// the part's index or -1.
	void addElement(int *res, physElement *p);
	void addElement(int *res);

	void getElementPos(const int which, mVec3 *p);
	void setElementPos(const int which, mVec3 *p);
	
	void getElementVel(const int which, mVec3 *v);

	void getElementDir(const int which, mVec3 *d);
	void setElementDir(const int which, mVec3 *d);

	void getElementUp(const int which, mVec3 *u);
	void setElementUp(const int which, mVec3 *u);

	void getElementMass(const int which, real *m);
	void setElementMass(const int which, real m);

	void getElementRadius(const int which, real *r);
	void setElementRadius(const int which, real r);

	void deleteElement(const int which);

	// operations on forces
	void deleteForce(const int which); 

	// route input to force. 
	void doInput(const int which, mVec3 *p); 

	// making forces. the new and easy way. 
	// documentation for force types is in physFoces.h
	void addCheater(int *result, const int p1); 
	void addCruftAttractor(int *result, const int part, mVec3 *target, real strength);
	void addDriveTire(int *result, const int p, const real tireFriction, const real radius, const real str);
	void addFloater(int *result, const int p1, const int p2, mVec3 *dir); 
	void addFloory(int *result, const int p, const real r);
	void addFriction(int *result);
	void addGravity(int *result, const real g);
	void addHelium(int *result, const int p1, const real rate);
	void addPoser(int *result, const int p1, mVec3 *p, mVec3 *v);
	void addRivet(int *result, const int p1); 
	void addSpring(int *result, const int p1, const int p2, const real str, const real restLen);
	void addWally(int *result, const int p, const real r);

	void addTubeConstraint(int *result, const int elemIdx, const real widthRadius, const real heightRadius);

	// collision manager interface
	void setCollisionRoomSet(mRoomSet *set); 

	void beginRoomFloors(const int roomx, const int roomy, const int numTris); 
	void addFloorToRoom(const int roomx, const int roomy, 
				mVec3 *p1, mVec3 *p2, mVec3 *p3); 
	void addFloorToRoom(const int roomx, const int roomy, 
				mVec3 *p1, mVec3 *p2, mVec3 *p3, mVec3 *n); 

	void beginRoomWalls(const int roomx, const int roomy, const int numTris); 
	void addWallToRoom(const int roomx, const int roomy, 
				mVec3 *p1, mVec3 *p2, mVec3 *p3); 
	void addWallToRoom(const int roomx, const int roomy, 
				mVec3 *p1, mVec3 *p2, mVec3 *p3, mVec3 *n); 

	void addTube(mTube *t); 

	void doTick(const evnTime t, const evnTime dt); 

	void command(const int argc, char *argv[]);
};


#endif // PHYS_MANAGER_CLASS


