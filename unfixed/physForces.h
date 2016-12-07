
/* 	Copyright (C) 2002 Greenhouse Software.  All Rights Reserved.
	see physForces.h for intro information about this code. 
	
*/


#include "stdDefines.h"
#include "mVec3.h"
#include "physManager.h"
#include "colManager.h" 


//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

// this force tries to make its Element (p1) act like a car.
// a car has an accelerator that is pushed down more or less, 
// and a steering wheel that is turned to the left or right. 
// it also has an engine of power "str", and it has an orientation
// of "carDir". 
// takeInput takes a vector, c. c.x is the angle of the steering wheel 
// with respect to the car body in radians-- left is negative. 
// c.y is how far down the accelerator is pushed, 0==not, 1==floored
// and c.z is how far above the pavement the car is. if c.z!=0, 
// then we can't steer or accelerate. 
class physDriveTire : public physForce {
private:
	colManager *m_cColMgr;
	int		m_iElementIdx;
	real	speed, str, steering, accelerator; 
	real	m_rTirefriction;
	real	m_rRadius;
	real	contactThreshhold;

public:
	physDriveTire() {}
	~physDriveTire() {}

	void init(colManager *cm, const int p, const real tireFriction, const real radius, const real s);
	void doIt(physElement *elements, const real t, const real dt);
	void takeIt(mVec3 *c);
};




// cheater forces are applied to single Elements (part p1).
// calls to takeCheater set the force that will be applied 
// at the next time step. The length of the force should be 
// <50. I think. Let me know. 
class physCheater : public physForce {
private:
	int p1; 
	mVec3 f; 
	bool m_bDirty;
public:
	physCheater() {}
	~physCheater() {}

	void init(const int p1);
	void doIt(physElement *p, const real t, const real dt);

	void takeIt(mVec3 *f); 

};



// This is cruft, but it does not actually attract cruft (thank goodness for that!)
// inverse square of distance attractor force.
// Great on toast!
class physCruftAttractor : public physForce
{
private:
	real	m_rStrength;	//strength of attractive force.
	int		p1;
	mVec3	target;
public:
	physCruftAttractor() {}
	~physCruftAttractor() {}

	void init(const int p, mVec3 *tar, real strength);
	void doIt(physElement *p, const real t, const real dt);
	void takeIt(mVec3 *newTar);
};


// this force takes two Elements and makes p2 follow p1 around. 
// all forces applied to p2 are ignored, and its position will always 
// be set == (p1's postition) + d
class physFloater : public physForce {
private:
	int p1, p2; 
	mVec3 d; 
public:
	physFloater() {}
	~physFloater() {}

	void init(const int p1, const int p2, mVec3 *d);
	void doIt(physElement *p, const real t, const real dt);
};



// a floory prevents its Element (p1) from going through floor collision 
// geometry. You need one floory per Element. 

class physFloory : public physForce
{
private:
	colManager *cm; 
	int	p1; 
	real radius; 
public:
	physFloory() {}
	~physFloory() {}

	void init(colManager *m, const int p1, const real r);
	void doIt(physElement *p, const real t, const real dt);
};



// one of these forces will scale all Element velocities by str
// each time step. So, 0.9 makes everything look like it's dragging
// through molasses, 1.0 has no effect, and 1.1 makes everything
// speed up until it freaks. You only need one.
class physFriction : public physForce
{
private:
	real m_rDragCoefficient;
	real m_rRho;
public:
	physFriction() {}
	~physFriction() {}

	void init();
	void doIt(physElement *p, const real t, const real dt);
};



// one of these forces will cause all Elements to be accelerated
// by (0, str, 0). if str<0, things fall down. if str>0, things fall up.
// You only need one.
class physGravity : public physForce {
private:
	real str; 
public:
	physGravity() {}
	~physGravity() {}

	void init(const real str);
	void doIt(physElement *p, const real t, const real dt) ;
};


//Provides a constant force to a Element along the y axis.
//One per Element.
class physHelium : public physForce
{
private:
	int		m_iP1;
	real	m_rRate;
public:
	physHelium() {}
	~physHelium() {}

	void init(const int p1, const real rate);
	void doIt(physElement *p, const real t, const real dt);
};


// a poser copies position and valocity from two mVec3's kept elsewhere
class physPoser : public physForce {
private:
	int		p1, ready; 
	mVec3	*poser, *volare; // who ho ho
public:
	physPoser() {}
	~physPoser() {}

	void init(const int p1, mVec3 *p, mVec3 *v);
	void doIt(physElement *p, const real t, const real dt) ;
};


// a rivet staples a Element (p1) in place. 
// a riveted Element ignores all other forces. 
class physRivet : public physForce {
private:
	int		p1, ready; 
	mVec3	pos, vel; 
public:
	physRivet() {}
	~physRivet() {}

	void init(const int p1);
	void doIt(physElement *p, const real t, const real dt) ;
};


// a spring is a piston of constant length
class physSpring : public physForce {
private:
	int		p1, p2; 
	real	rlen, k; 
public:
	physSpring() {}
	~physSpring() {}

	void init(const int pa, const int pb, const real ak, const real restLen);
	void doIt(physElement *p, const real t, const real dt);
};

// a wally prevents its Element (p1) from going through wall collision 
// geometry. You need one wally per Element. 

class physWally : public physForce {
private:
	colManager *cm; 
	int	p1; 
	real radius; 
public:
	physWally() {}
	~physWally() {}

	void init(colManager *m, const int p1, const real r);
	void doIt(physElement *p, const real t, const real dt);
};



class physTubeConstraint : public physForce
{
private:
	colManager *cm; 
	int	m_iElementIdx; 
	real m_rWidthRadius, m_rHeightRadius; 
public:
	physTubeConstraint() {}
	~physTubeConstraint() {}

	void init(colManager *m, const int elementIdx, const real widthRadius, const real heightRadius);
	void doIt(physElement *elements, const real t, const real dt);
};