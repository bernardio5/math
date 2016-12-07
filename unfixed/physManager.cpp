
/* Copyright (C) 2002 Greenhouse Software.  All Rights Reserved.
	see physManager.h for intro information about this code. 
	*/

#include "physManager.h"
#include "physForces.h"

#include <stdlib.h>

#include "profiler.h"

Define_Zone(physMgr_doTick);

// default init fn for phys Element. Huge, ain't it. 
physElement::physElement() {
	static real zero = (real)0.0;
	static real one = (real)1.0;
	p.zero(); 
	v.zero(); 
	f.zero(); 
	d.zero();
	u.set(zero, one, zero);
	op.zero(); 
	ov.zero(); 
	od.zero();
	ou.set(zero, one, zero);
	mass = one;	
	r = zero;
	birthday = zero;
	active = true;
	iTubeIdx = -1;
}

void physElement::copy(physElement *otherpart) {
	p.copy(&(otherpart->p));
	v.copy(&(otherpart->v));
	f.copy(&(otherpart->f));
	d.copy(&(otherpart->d));
	u.copy(&(otherpart->u));
	op.copy(&(otherpart->op));
	ov.copy(&(otherpart->ov));
	od.copy(&(otherpart->od));
	ou.copy(&(otherpart->ou));
	mass = otherpart->mass;
	r = otherpart->r;	
	birthday = otherpart->birthday;			
	active = otherpart->active; 
}





//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////











void physManager::doForces() {
	int i, j, k, cnt;
	real d;
	mVec3 vec; 
	physForce	**fList;

	// apply all the "reactionary" forces to each Element in turn
	// this is for things like gravity, friction. 
	fList = m_pForces[PHYS_FSTYLE_WIDE];
	cnt = m_iCounts[PHYS_FSTYLE_WIDE];
	for (k=0; k<cnt; ++k) {
		for (i=0; i<m_iPCount; ++i) {
			if (m_cElements[i].active) { 
				fList[k]->doIt(&(m_cElements[i]), m_dT, m_dDT);
			}
		}
	}
	
	cnt = m_iCounts[PHYS_FSTYLE_NSQUARED];
	if (cnt>0) {
		fList = m_pForces[PHYS_FSTYLE_NSQUARED];
		for (i=0; i<m_iPCount; ++i) {
			if (m_cElements[i].active) { 
				for (j=i+1; j<m_iPCount; j++) {
					// d = vector from j to which
					if (m_cElements[j].active) {
						vec.eqSub(&(m_cElements[i].p), &(m_cElements[j].p));
						vec.length3sq(&d);
						
						for (k=0; k<cnt; ++k) {
							fList[k]->doIt(i, j, m_cElements, d, m_dDT);
						}
					}
				}
			}
		}
	}
	
	// these forces know which Elements thay are to affect; 
	// pass the list of all parts to them. 
	fList = m_pForces[PHYS_FSTYLE_SINGULAR];
	cnt = m_iCounts[PHYS_FSTYLE_SINGULAR];
	for (k=0; k<cnt; ++k) {
		fList[k]->doIt(m_cElements, m_dT, m_dDT);
	}
}



void physManager::doConstraints() {
	int i, k, cnt;
	physForce **fList;

	// constraints, after integration, called once per part
	fList = m_pForces[PHYS_FSTYLE_CONST_WIDE];
	cnt = m_iCounts[PHYS_FSTYLE_CONST_WIDE];
	for (k=0; k<cnt; ++k) {
		for (i=0; i<m_iPCount; ++i) {
			if (m_cElements[i].active) { 
				fList[k]->doIt(&(m_cElements[i]), m_dT, m_dDT);
			}
		}
	}
	// constraints that affect single parts
	fList = m_pForces[PHYS_FSTYLE_CONST_SING];
	cnt = m_iCounts[PHYS_FSTYLE_CONST_SING];
	for (k=0; k<cnt; ++k) {
		fList[k]->doIt(m_cElements, m_dT, m_dDT);
	}
}




void physManager::doMove()
{
	int i;
	mVec3 v, a, x;
	physElement *p; 

	for (i=0; i<m_iPCount; ++i)
	{
		if (m_cElements[i].active)
		{
			p = &(m_cElements[i]); 
			(p->op).copy(&(p->p));
			(p->ov).copy(&(p->v));
			(p->od).copy(&(p->d));
			p->f.zero();
		}
	}

	doForces();
	
	// integration
	for (i=0; i<m_iPCount; ++i)
	{
		if (m_cElements[i].active)
		{
			p = &(m_cElements[i]);

			//Calculate the acceleration (A = F / M)
			a.eqScale(&(p->f), (real)1.0 / p->mass);

			//V' = V + A * dt
			v.eqScale(&a, m_dDT);
			v.addEq(&(p->v));

			//X' = X + V' * dt
			x.eqScale(&v, m_dDT);
			x.addEq(&(p->p));

			p->v.copy(&v);
			p->p.copy(&x);
/*
			p = &(m_cElements[i]);

			a.copy(&(p->f));
			invM = m_dDT/p->mass;
			a.scale(invM);
			(p->v).addEq(&a);

			v.copy(&(p->v));
			v.scale(m_dDT);

			(p->p).addEq(&v);
*/
		}
	}
	
	doConstraints();

	m_dT += m_dDT;
	++m_iSimTicks;
}











// we're going to put the force at m_cForces[s][m_iCounts[s]]. 
// but what entry of m_iMaps[s] can we use? r.
void physManager::getIndex(int *r, const physForceStyle s) {
	int i, max;
	short int *m;
	*r = -1; 

	if ((s<=PHYS_FSTYLE_ERROR) || (s>=PHYS_FSTYLE_LAST)) { return; }

	max = m_iLimits[s]; 
	m = m_iMaps[s]; 

	if (m_iCounts[s] >= m_iLimits[s]) { 
		modalPop("physMgr::no available forces"); 
	} else {
		// ya jolly well better find one
		i = 0; 
		while ((i<max) && (*r==-1)) {
			if (m[i]==-1) {
				*r = i;
			}	
			++i;
		}
	}
}


// take a style and an index into maps, and make the number to return.
// do lots of error checking in case you're handed crap. 
void physManager::spinIndex(const int index, const physForceStyle st, int *result) {
	int i; 

	i=-1; 
	if ((st>PHYS_FSTYLE_ERROR) && (st<PHYS_FSTYLE_LAST)) { 
		if ((index>-1) && (index < m_iCounts[st])) { 
			i = ( PHYS_MAX_OF_MAX * ((int)st) ) + index; 
		}
	}
	*result = i; 

}

// take a spun index and return which element of maps to use. 
// do lots of error checking in case you're handed crap. 
void physManager::unspinIndex(const int index, physForceStyle *st, int *result) {
	int i;
	physForceStyle j; 

	i = -1; 
	j = PHYS_FSTYLE_ERROR;

	if (index>=0) { 
		i = index % PHYS_MAX_OF_MAX;
		j = (physForceStyle)((index-i) /  PHYS_MAX_OF_MAX);
		// how many ways could you screw this up? 
		if ((j<PHYS_FSTYLE_ERROR) || (j>=PHYS_FSTYLE_LAST)) { 
			j = PHYS_FSTYLE_ERROR; 
		}
	}
	*result = i; 
	*st = j; 
}




//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////



physManager::physManager(errManager *theErrMgr,
						fileManager *theFileMgr
						){
	int i, j, x; 

	// init members from baseManager
	m_iMyType = MGR_PHYS;
	m_cErrMgr = theErrMgr;
	m_cFileMgr = theFileMgr;

	m_iPCount = 0; 


	m_cColMgr = new colManager(m_cErrMgr);

	if (m_cColMgr->notOkay()) {
		modalPop("failed to create the collision manager");
	}

	m_iLimits[PHYS_FSTYLE_ERROR] = 0; 
	m_iLimits[PHYS_FSTYLE_WIDE] = PHYS_MAX_WIDE; 
	m_iLimits[PHYS_FSTYLE_NSQUARED] = PHYS_MAX_NSQUARED; 
	m_iLimits[PHYS_FSTYLE_SINGULAR] = PHYS_MAX_SINGULAR; 
	m_iLimits[PHYS_FSTYLE_CONST_SING] = PHYS_MAX_CONST_SING; 
	m_iLimits[PHYS_FSTYLE_CONST_WIDE] = PHYS_MAX_CONST_WIDE; 

	for (i=0; i<PHYS_FSTYLE_LAST; ++i) { 
		m_iCounts[i] = 0; 
		m_iMaps[i] = new short int[ m_iLimits[i] ]; 
		x = m_iLimits[i]; 
		m_pForces[i] = new physForce * [x]; 
		for (j=0; j<m_iLimits[i]; ++j) { 
			m_iMaps[i][j] = -1; 
			m_pForces[i][j] = NULL; 
		}
	}

	chill(); 
}




physManager::~physManager() {
	wig(); 
	if (m_iPCount>0) {
		delete [] m_cElements;
	}

}




void physManager::initElements(const int mP) {
	int i, lmp;
	
	lmp = mP; 
	if (lmp>=PHYS_MAX_PARTS) {
		lmp = PHYS_MAX_PARTS; 
	}

	if (m_iPCount!=0) { 
		delete [] m_cElements; 
	}

	m_cElements = new physElement[lmp];
	m_dT= (real)0.0;
	m_dDT= (real)0.1;
	m_iSimTicks= 0;
	m_iPCount = lmp; 
	
	for (i=0; i<lmp; ++i) {
		m_cElements[i].copy(&m_cPart);
		m_cElements[i].active = false; 
	}
	chill(); 
}





void physManager::addElement(int *res, physElement *p) {
	int i; 

	*res = -1; 
	i=0; 
	while ((*res==-1)&&(i<m_iPCount)) {
		if (!(m_cElements[i].active)) {
			*res = i; 
		}
		++i;
	}

	if (*res!=-1) {
		m_cElements[*res].copy(p);
	} else {
		modalPop("failed addElement");
	}
}


void physManager::addElement(int *res) {
	int i; 

	*res = -1; 
	i=0; 
	while ((*res==-1)&&(i<m_iPCount)) {
		if (!(m_cElements[i].active)) {
			*res = i; 
		}
		++i;
	}

	if (*res!=-1) {
		m_cElements[*res].copy(&m_cPart);
	} else {
		modalPop("failed addElement");
	}
}




void physManager::getElementPos(const int which, mVec3 *vec) {
	if ((which>=0) && (which<m_iPCount)) {
		vec->copy(&(m_cElements[which].p));
	}
}


void physManager::setElementPos(const int which, mVec3 *vec) {
	if ((which>=0) && (which<m_iPCount)) {
		(m_cElements[which].p).copy(vec);
	}
}


void physManager::getElementVel(const int which, mVec3 *vec) {
	if ((which>=0) && (which<m_iPCount)) {
		vec->copy(&(m_cElements[which].v));
	}
}

void physManager::getElementDir(const int which, mVec3 *d)
{
	if ((which>=0) && (which<m_iPCount))
	{
		d->copy(&(m_cElements[which].d));
	}
}

void physManager::setElementDir(const int which, mVec3 *d)
{
	if ((which>=0) && (which<m_iPCount))
	{
		(m_cElements[which].d).copy(d);
	}
}

void physManager::getElementUp(const int which, mVec3 *u)
{
	if ((which>=0) && (which<m_iPCount))
	{
		u->copy(&(m_cElements[which].u));
	}
}

void physManager::setElementUp(const int which, mVec3 *u)
{
	if ((which>=0) && (which<m_iPCount))
	{
		(m_cElements[which].u).copy(u);
	}
}

void physManager::getElementMass(const int which, real *m)
{
	if ((which>=0) && (which<m_iPCount))
	{
		*m = m_cElements[which].mass;
	}
}

void physManager::setElementMass(const int which, real m)
{
	if ((which>=0) && (which<m_iPCount))
	{
		m_cElements[which].mass = m;
	}
}

void physManager::getElementRadius(const int which, real *r)
{
	if ((which>=0) && (which<m_iPCount))
	{
		*r = m_cElements[which].r;
	}
}

void physManager::setElementRadius(const int which, real r)
{
	if ((which>=0) && (which<m_iPCount))
	{
		m_cElements[which].r = r;
	}
}



void physManager::deleteElement(const int which) {
	if ((which>=0) && (which<m_iPCount)) {
		m_cElements[which].active = false;
	}
}



/////////////////////////////////////////////// force-handling


void physManager::deleteForce(const int which) {
	physForceStyle st; 
	int i, unspun, unmapped; 

	unspinIndex(which, &st, &unspun); 
	if (st==PHYS_FSTYLE_ERROR) { return; }

	unmapped = m_iMaps[st][unspun];
	if ((unmapped<0) || (unmapped>m_iCounts[st])) {
		modalPop("physmgr:: unmap error"); 
	}
	delete m_pForces[st][unmapped];

	// compact force list
	for (i=unmapped; i<(m_iCounts[st]-1); ++i) {
		m_pForces[st][i] = m_pForces[st][i+1]; 
	}
	m_pForces[st][(m_iCounts[st])] = NULL; 
	--(m_iCounts[st]); 

	// reset map entries
	for (i=0; i<m_iLimits[st]; ++i) {
		if (m_iMaps[st][i]>unmapped) {
			--(m_iMaps[st][i]);
		}
	}
	m_iMaps[st][unspun] = -1; 
}





void physManager::doInput(const int which, mVec3 *p) {
	physForceStyle st; 
	int unspun, i; 

	unspinIndex(which, &st, &unspun); 
	if (st==PHYS_FSTYLE_ERROR) { return; }

	i = m_iMaps[st][unspun]; 

	if ((i>-1) && (i<m_iCounts[st])) {
		 m_pForces[st][i]->takeIt(p); 
	}
}




void physManager::doTick(const evnTime t, const evnTime dt)
{
	//Profiler information
	Profile_Scope(physMgr_doTick);
	// don't run backwards, and don't run a step 
	// so tiny you can't even tell it happened. 
	m_dDT = dt/(real)2.0;  
	m_dT = t;  
	doMove();
	m_dDT = dt;
	doMove();
}





void physManager::addForce(int *result, const physForceStyle s, physForce *f) {
	int i, c; 
	*result = -1; 
	getIndex(&i, s); 
	if (i!=-1) {
		c = m_iCounts[s]; 
		m_iMaps[s][i] = c; 
		m_pForces[s][c] = f;
		++m_iCounts[s]; 
		spinIndex(i, s, result); 
	} else {
		delete f; 
	}
}




void physManager::addCheater(int *result, const int p1) { 
	physCheater *it = new physCheater(); 
	it->init(p1); 
	addForce(result, PHYS_FSTYLE_SINGULAR, it); 
}

void physManager::addCruftAttractor(int *result, const int part, mVec3 *target, real strength)
{ 
	physCruftAttractor *it = new physCruftAttractor(); 
	it->init(part, target, strength); 
	addForce(result, PHYS_FSTYLE_SINGULAR, it); 
}


void physManager::addDriveTire(int *result, const int p, const real tireFriction, const real radius, const real str)
{
	physDriveTire *it = new physDriveTire(); 
	it->init(m_cColMgr, p, tireFriction, radius, str);
	addForce(result, PHYS_FSTYLE_SINGULAR, it); 
}


void physManager::addFloater(int *result, const int p1, const int p2, mVec3 *dir) { 
	physFloater *it = new physFloater(); 
	it->init(p1, p2, dir); 
	addForce(result, PHYS_FSTYLE_CONST_SING, it); 
}

void physManager::addFloory(int *result, const int p, const real r)
{
	physFloory *it = new physFloory(); 
	it->init(m_cColMgr, p, r); 
	addForce(result, PHYS_FSTYLE_SINGULAR, it); 
}

void physManager::addFriction(int *result)
{
	physFriction *it = new physFriction(); 
	it->init(); 
	addForce(result, PHYS_FSTYLE_WIDE, it); 
}

void physManager::addGravity(int *result, const real g)	{
	physGravity *it = new physGravity(); 
	it->init(g); 
	addForce(result, PHYS_FSTYLE_WIDE, it); 
}

void physManager::addHelium(int *result, const int p1, const real rate)
{
	physHelium *it = new physHelium(); 
	it->init(p1, rate); 
	addForce(result, PHYS_FSTYLE_CONST_SING, it); 
}


void physManager::addPoser(int *result, const int p, mVec3 *pos, mVec3 *vel) {
	physPoser *it = new physPoser(); 
	it->init(p, pos, vel); 
	addForce(result, PHYS_FSTYLE_CONST_SING, it); 
}



void physManager::addRivet(int *result, const int p) {
	physRivet *it = new physRivet(); 
	it->init(p); 
	addForce(result, PHYS_FSTYLE_CONST_SING, it); 
}

void physManager::addSpring(int *result, const int p1, const int p2, const real str, const real restLen) {
	physSpring *it = new physSpring(); 
	it->init(p1, p2, str, restLen); 
	addForce(result, PHYS_FSTYLE_SINGULAR, it); 
}

void physManager::addWally(int *result, const int p, const real r) {
	physWally *it = new physWally(); 
	it->init(m_cColMgr, p, r); 
	addForce(result, PHYS_FSTYLE_SINGULAR, it); 
}


void physManager::addTubeConstraint(int *result, const int elemIdx, const real widthRadius, const real heightRadius)
{
	physTubeConstraint *it = new physTubeConstraint();
	it->init(m_cColMgr, elemIdx, widthRadius, heightRadius);
	addForce(result, PHYS_FSTYLE_CONST_SING, it);
}














typedef enum {
	PHYS_SCR_ERROR = 0,

	// operations on the global Element
	PHYS_SCR_SET_POS,		// x, y, z
	PHYS_SCR_SET_VEL,		// vx, vy, vz
	PHYS_SCR_SET_MOVE,		// move style string
	PHYS_SCR_SET_MASS,		// mass

	// psys initializers
	PHYS_SCR_INIT,				// nParts 
	PHYS_SCR_SET_TIME,			// no args
	PHYS_SCR_ADD_PART,			// no args
	PHYS_SCR_ADD_FORCE,			// forceType, many args? 
	PHYS_SCR_DO_TICK,			// forceType, many args? 

	PHYS_SCR_LAST_COMMAND
} physScrCommand;


void physManager::command(int argc, char *argv[]) {
	int i;
	physScrCommand c;
	evnTime dt; 

	char commands[][MAX_COMMAND_NAME_LENGTH] = { 
		"setPos", "setVel", "setMove", "setMass", 
		"init", "setTime", "addPart", "addForce", 
		"doTick" };

	parse(argv[0], &i, commands, PHYS_SCR_LAST_COMMAND-1); 
	c = (physScrCommand)i; 

	switch (c) {
	case PHYS_SCR_SET_POS: 
		if (argc>3) {
			m_cPart.p.x = (real)atof(argv[1]); 
			m_cPart.p.y = (real)atof(argv[2]); 
			m_cPart.p.z = (real)atof(argv[3]); 
		}
		break;
	case PHYS_SCR_SET_VEL: 
		if (argc>3) {
			m_cPart.v.x = (real)atof(argv[1]); 
			m_cPart.v.y = (real)atof(argv[2]); 
			m_cPart.v.z = (real)atof(argv[3]); 
		}
		break;
	case PHYS_SCR_SET_MASS: 
		if (argc>1) {
			m_cPart.mass = (real)atof(argv[1]); 
		}
		break;
	case PHYS_SCR_INIT: 
		if (argc>1) { 
			i = atoi(argv[1]); // max parts
			initElements(i);
		}
		break;
	case PHYS_SCR_SET_TIME: 
		if (argc>1) {
			m_dT = (evnTime)(atof(argv[1])); 
		}
		break;
	case PHYS_SCR_ADD_PART:
		addElement(&i, &m_cPart);
		// unfortunately lost data here
		break;
	case PHYS_SCR_DO_TICK: 
		if (argc>1) {
			dt = (evnTime)atof(argv[1]); 
			doTick(dt, 0.5); 
		}
		break;
	default:
		modalPop("physMgr::script:unimplemented command"); 
		modalPop(argv[0]); 
		break;
	}	
}


///////////// collision manager interface

void physManager::setCollisionRoomSet(mRoomSet *set){
	m_cColMgr->SetRoomSet(set); 
}


void physManager::beginRoomFloors(const int roomx, const int roomy, const int numTris) {
	m_cColMgr->beginRoomFloors(roomx, roomy, numTris); 
}

void physManager::addFloorToRoom(const int roomx, const int roomy, 
				mVec3 *p1, mVec3 *p2, mVec3 *p3){
	m_cColMgr->addFloorToRoom(roomx, roomy, p1, p2, p3); 
}
void physManager::addFloorToRoom(const int roomx, const int roomy, 
				mVec3 *p1, mVec3 *p2, mVec3 *p3, mVec3 *n){
	m_cColMgr->addFloorToRoom(roomx, roomy, p1, p2, p3, n); 
}


void physManager::beginRoomWalls(const int roomx, const int roomy, const int numTris) {
	m_cColMgr->beginRoomWalls(roomx, roomy, numTris); 
}
void physManager::addWallToRoom(const int roomx, const int roomy, 
				mVec3 *p1, mVec3 *p2, mVec3 *p3){
	m_cColMgr->addWallToRoom(roomx, roomy, p1, p2, p3); 
}
void physManager::addWallToRoom(const int roomx, const int roomy, 
				mVec3 *p1, mVec3 *p2, mVec3 *p3, mVec3 *n){
	m_cColMgr->addWallToRoom(roomx, roomy, p1, p2, p3, n); 
}


void physManager::addTube(mTube *t) {
	m_cColMgr->addTube(t); 
}

