
/* 	Copyright (C) 2002 Greenhouse Software.  All Rights Reserved.
	see netManager.h for intro information about this code. 

	To Do:
		1) Forces to keep things from going through patches
		2) forces to implement animation
			a) how to load?
			b) how to store?
			c) how to initialize?
			d) how to access? 
			e) how to enable rendering? 
		3) good framework for particle management
	
*/

#include "windows.h"
#include <assert.h>
#include <math.h>

#include "physManager.h"

// for player input routine
#include "inpManager.h"
#include "physForces.h"
#include "profiler.h"

Define_Zone(physWally_doIt);
Define_Zone(physFloory_doIt);
Define_Zone(physTubeConstraint_doIt);


//////////////////////
// Drive Tire Force //
//////////////////////
void physDriveTire::init(colManager *cm, const int p, const real tireFriction, const real radius, const real s)
{
	static real zero = (real)0.0f;
	m_cColMgr = cm;
	m_iElementIdx = p;
	m_rTirefriction = tireFriction;
	m_rRadius = radius;
	contactThreshhold = 0.5f; //in meters
	speed = zero;
	steering = zero; 
	accelerator = zero;
	str = s;
}


void physDriveTire::doIt(physElement *elements, const real t, const real dt)
{
	static real steerScale = (real)0.1;
	static real zero = (real)0.0;
	real	sidewaysVel;
	real	surfAndUpDot;
	real	surfNormVelComponent; //the amount of the velocity of the particle that is along the normal to the collision surface(s)
    mVec3	pos, vel, dir, tdir, right, up, norm, totNorm, params, center, shellp, df;
	mx4		upAxis;
	int		*tubeIdx;

	pos.copy(&(elements[m_iElementIdx].p));
	vel.copy(&(elements[m_iElementIdx].v));
	tubeIdx = &(elements[m_iElementIdx].iTubeIdx);
	
	if (*tubeIdx == -1)
	{
		m_cColMgr->map(&pos, &params, tubeIdx); 
	}
	else
	{
		m_cColMgr->updateMap(&pos, &params, tubeIdx); 
	}
/*
	centerCarDistance = params.z;

	df.eqSub(&shellp, &pos);
	df.length3(&carWallDistance);

	carWallDistance -= m_rRadius;

//FLOOR CHECK
	if(carWallDistance < contactThreshhold)
	{
*/
		//The tire is in contact with the tube surface.  Do stuff.

		m_cColMgr->getPointNormal(&params, &pos, &norm, &tdir, &shellp);

		elements[m_iElementIdx].u.dotWith(&norm, &surfAndUpDot);

		if(surfAndUpDot > 0.964) //within ~15 degrees of up
		{
			//set the new up vector!
			elements[m_iElementIdx].u.copy(&norm);

			vel.copy(&(elements[m_iElementIdx].v));
			dir.copy(&(elements[m_iElementIdx].d));
			up.copy(&(elements[m_iElementIdx].u));

			//get the right vector for the new up and old dir
			right.eqCross(&dir, &up); // right = dir X up
			//set the new direction
			dir.eqCross(&up, &right);
			dir.normalize();

			//Steering, since we're in contact with the road now.
			upAxis.setToRotation(steering * steerScale * dt, &up);
			upAxis.preMultSingle(&dir, &(elements[m_iElementIdx].d));

			// reflect velocity about totNorm
			norm.dotWith(&vel, &surfNormVelComponent); 

			//Handle car thrust
			speed = accelerator * str;

			df.copy(&(elements[m_iElementIdx].d));
			df.scale(speed * elements[m_iElementIdx].mass);
			elements[m_iElementIdx].f.addEq(&df);
			
			//do tire stuff here

			//right.normalize();
			vel.dotWith(&right, &sidewaysVel); //sideways velocity

			//calculate lateral tire friction
			right.scale(-m_rTirefriction*sidewaysVel);
			df.eqScale(&right, elements[m_iElementIdx].mass);
			elements[m_iElementIdx].f.addEq(&df);
		}
//	}
}

void physDriveTire::takeIt(mVec3 *c)
{
	steering = c->x;
	accelerator = c->y; 
}












void physCheater::init(const int pa) {
	p1 = pa;
	f.zero();
	m_bDirty = false;
}

void physCheater::doIt(physElement *elements, const real t, const real dt)
{
	if(m_bDirty)
	{
		f.scale(dt/(elements[p1].mass));
		(elements[p1].v).addEq(&f); 
		f.zero();
		m_bDirty = false;
	}
}
void physCheater::takeIt(mVec3 *af)
{
	f.copy(af); 
	m_bDirty = true;
}








///CRRRRRRAAP!
void physCruftAttractor::init(const int p, mVec3 *tar, real strength)
{
	m_rStrength = strength;
	p1 = p;
	target.copy(tar);
}

void physCruftAttractor::doIt(physElement *elements, const real t, const real dt)
{
	mVec3	delta;
	real	distSq;
	real	speedLimit = 10.0f;

	delta.copy(&target);
	delta.distanceFromsq(&(elements[p1].p), &distSq);
	delta.subEq(&(elements[p1].p));


	delta.normalize();
	if(distSq > 100.0f)
		distSq = 100.0f;

	delta.scale((1.0f/distSq) * m_rStrength * dt);
	elements[p1].v.addEq(&delta);

//Temporary speed limit hack
	if(elements[p1].v.x > speedLimit)
		elements[p1].v.x = speedLimit;
	if(elements[p1].v.x < -speedLimit)
		elements[p1].v.x = -speedLimit;
	if(elements[p1].v.y > speedLimit)
		elements[p1].v.y = speedLimit;
	if(elements[p1].v.y < -speedLimit)
		elements[p1].v.y = -speedLimit;
	if(elements[p1].v.z > speedLimit)
		elements[p1].v.z = speedLimit;
	if(elements[p1].v.z < -speedLimit)
		elements[p1].v.z = -speedLimit;


	//(p->v).y += m_iStrength * dt;
}

void physCruftAttractor::takeIt(mVec3 *newTar)
{
	target.copy(newTar);
}









// shadower maker
void physFloater::init(const int pa, const int pb, mVec3 *dir) {
	p1 = pa;
	p2 = pb;
	d.copy(dir); 
}
void physFloater::doIt(physElement *elements, const real t, const real dt) {
	(elements[p1].v).copy(&(elements[p2].v));
	(elements[p1].p).eqAdd(&(elements[p2].p), &d);
}




//////////////////
// Floory Force //
//////////////////
void physFloory::init(colManager *colM, const int pindx, const real r)
{
	p1 = pindx;
	radius = r;
	cm = colM;
}



void physFloory::doIt(physElement *e, const real t, const real dt)
{
	static real zero = (real)0.0; 
	real	u, d;
	mVec3	*pos, *vel, tdir, norm, params, center, shellp, vcor;
	int		*tubeIdx;

	//Profiler information
	Profile_Scope(physFloory_doIt);

	pos = &(e[p1].p); 
	vel = &(e[p1].v);
	tubeIdx = &(e[p1].iTubeIdx);
	
	if (*tubeIdx == -1)
	{
		cm->map(pos, &params, tubeIdx); 
	}
	else
	{
		cm->updateMap(pos, &params, tubeIdx); 
	}
	d = params.z;
	// d is distance from the floor. We want d>=0.
	// but wait, there's a "radius" involved. 
	d -= radius; 

	if (d<0) { // point's sphere is going out of mTube
		cm->getPointNormal(&params, pos, &norm, &tdir, &shellp); 
	
		// zero out velocity taking you from surface
		norm.dotWith(vel, &u); 
		if (u<zero) {
			u *= -1.0f;
			vcor.copy(&norm); 
			vcor.scale(u);
			vel->addEq(&vcor);
		}
	
		// move p to ensure it is on the surface
		norm.scale(-d); 
		pos->addEq(&norm); 
	}
}






// velocity-dependent air-drag-like force.
void physFriction::init()
{
	m_rDragCoefficient = (real)0.01;
	m_rRho = (real)1.29; //kg/m^3
}

void physFriction::doIt(physElement *e, const real t, const real dt)
{
	mVec3	dragVector;
	real	speed;

	dragVector.copy(&(e->v));
	dragVector.length3sq(&speed);
	dragVector.scale(-(real)0.5f * m_rRho * m_rDragCoefficient * speed * e->r * e->r);
	e->f.addEq(&dragVector);
}





// "down"-ward (-y) force. 
void physGravity::init(real s) {
	str = s;
}

void physGravity::doIt(physElement *e, const real t, const real dt)
{
	e->f.y += (str * e->mass);
}



////////////
// Helium //
////////////
void physHelium::init(const int p1, const real rate)
{
	m_iP1 = p1;
	m_rRate = rate;
}

void physHelium::doIt(physElement *elements, const real t, const real dt)
{
	elements[m_iP1].f.y += (m_rRate * elements[m_iP1].mass);
}







//////////////
// poser
////////////
void physPoser::init(const int pindx, mVec3 *p, mVec3 *v) {
	p1 = pindx; 
	poser = p; 
	volare = v; 
}



void physPoser::doIt(physElement *e, const real t, const real dt)
{
	e[p1].p.copy(poser);
	e[p1].v.copy(volare);
	e[p1].f.zero();
}












void physRivet::init(const int pindx)
{
	p1 = pindx;
	pos.zero();
	vel.zero();
	ready = 0;
}

void physRivet::doIt(physElement *e, const real t, const real dt)
{
	if (ready==0) { // first execution, copy out position
		pos.copy(&(e[p1].p));
		ready = 1; 
	}
	e[p1].p.copy(&pos);
	e[p1].f.zero();
	e[p1].v.zero();
}





void physSpring::init(const int pa, const int pb, const real ak, const real restLen)
{
	p1 = pa;
	p2 = pb;
	k = ak;
	rlen = restLen;
}


void physSpring::doIt(physElement *elements, const real t, const real dt) {
	static real max = (real)1.0; 
	static real neglim = (real)-99.0; 
	real len;
	mVec3 df;
	
	if (rlen<neglim) {
		df.eqSub(&(elements[p1].op), &(elements[p2].op));
		df.length3(&len); 
		// set rest length = starting length
		rlen = len;
	}

	df.eqSub(&(elements[p1].p), &(elements[p2].p));
	df.length3(&len); 

	len = rlen  - len;
	if (len*len > max) { len = max; } 

	df.scale(k * len);

	elements[p1].f.addEq(&df); 
	elements[p2].f.subEq(&df); 
}

#include "phPlane.h"

/////////////////
// Wally Force //
/////////////////
void physWally::init(colManager *colM, const int pindx, const real r)
{
	p1 = pindx;
	radius = r;
	cm = colM;
}


void physWally::doIt(physElement *e, const real t, const real dt)
{
	static real two = (real)2.0; 
	static real zero = (real)0.0;
	real	v, rightwardvel, theta, leftwall, rightwall;
	mVec3	*pos, *vel, dir, norm, center, params, right, shellp;
	int		*tubeIdx;

	//Profiler information
	Profile_Scope(physWally_doIt);

	pos = &(e[p1].p);
	vel = &(e[p1].v);
	tubeIdx = &(e[p1].iTubeIdx);
	
	if(*tubeIdx == -1)
	{
		cm->map(pos, &params, tubeIdx); 
	}
	else
	{
		cm->updateMap(pos, &params, tubeIdx); 
	}
	// honestly, d is floorie's problem. 

	// theta is the angle from right, in radians, 
	// going cclockwise. 
	theta = params.y; 

	leftwall = 4.2f;
	rightwall = 5.2f; 


	if ((theta>rightwall)||(theta<leftwall)) { 
		cm->getPointNormal(&params, pos, &norm, &dir, &shellp); 
		// need a "sideways" vector-- pointing right.
		// need dir for that. 

		right.eqCross(&dir, &norm); 

		// determine whether you're going left or right
		right.dotWith(vel, &rightwardvel); 


		if ( ( (rightwardvel<zero) && (theta<leftwall) ) || // right wall, going right
			( (rightwardvel>zero) && (theta>rightwall) ) // left wall, going left
			)
		{
			//take some energy away from the system -- inelastic collisions
			right.scale(rightwardvel * -2.0f);

			// also, deduct some velocity for being a putz
			dir.dotWith(vel, &v);
			dir.scale(-0.1f); 
			right.addEq(&dir); 

			//totNorm.y = 0.0f;
			vel->addEq(&right);
		}
	}
}




///////////////////////////
// Tube Constraint Force //
///////////////////////////
void physTubeConstraint::init(colManager *m, const int elementIdx, const real widthRadius, const real heightRadius)
{
	m_iElementIdx = elementIdx;
	m_rWidthRadius = widthRadius;
	m_rHeightRadius = heightRadius;
	cm = m;
}

void physTubeConstraint::doIt(physElement *elements, const real t, const real dt)
{
	static real zero = (real)0.0; 
	real	u, f, centerCarDistance, carWallDistance, centerWallDistance, theta, leftwall, rightwall, rightwardvel;
	mVec3	pos, newPos, vel, dir, tdir, norm, params, center, shellp, vcor, right, test;
	mx4		upAxis;
	int		*tubeIdx;

	//Profiler information
	Profile_Scope(physTubeConstraint_doIt);

	pos.copy(&(elements[m_iElementIdx].p));
	vel.copy(&(elements[m_iElementIdx].v));
	dir.copy(&(elements[m_iElementIdx].d));
	tubeIdx = &(elements[m_iElementIdx].iTubeIdx);
	
	if (*tubeIdx == -1)
	{
		cm->map(&pos, &params, tubeIdx); 
	}
	else
	{
		cm->updateMap(&pos, &params, tubeIdx); 
	}
	cm->getPointNormal(&params, &newPos, &norm, &tdir, &shellp);

    theta = params.y;
	//centerCarDistance = params.z;

	//MAGIC NUMBERS!  LA LA LA!!!
	leftwall = 4.2f;
	rightwall = 5.2f; 

	vcor.eqSub(&center, &pos);
	vcor.length3(&centerCarDistance);

	vcor.eqSub(&shellp, &pos);
	vcor.length3(&carWallDistance);

	vcor.eqSub(&shellp, &center);
	vcor.length3(&centerWallDistance);

	centerCarDistance += m_rHeightRadius;

//FLOOR CHECK
	if(carWallDistance < m_rHeightRadius)
	{
		elements[m_iElementIdx].p.copy(&shellp);
//		vcor.eqScale(&norm, m_rHeightRadius);
//		elements[m_iElementIdx].p.addEq(&vcor);

		elements[m_iElementIdx].u.copy(&norm);

		//Zero out the velocity taking you away from the tube
		vel.dotWith(&norm, &u);
		vcor.eqScale(&norm, -u);
		elements[m_iElementIdx].v.addEq(&vcor);
	}

//WALL CHECK
	if ((theta>rightwall)||(theta<leftwall))
	{ 
//		cm->getPointNormal(&params, &wall, &norm, &tdir);
		
		//Find the tangent to the tube's cross-section surface
		right.eqCross(&tdir, &norm); 

		// determine whether you're going left or right
		right.dotWith(&vel, &rightwardvel); 

		if(	((rightwardvel<zero) && (theta<leftwall)) || // right wall, going right
			((rightwardvel>zero) && (theta>rightwall)) )// left wall, going left
		{

			//change the direction a bit - fake torque
			tdir.dotWith(&dir, &f);
			if(f > zero)
			{
				//we're going forward!
				right.negate();
			}

			//get a vector between the wall normal and the direction you're headed
			vcor.eqAdd(&right, &dir);
			//add another dir component so that no zero vectors are possible
			//(i.e. in the case of a head-on with the wall)
			vcor.addEq(&dir);
			vcor.normalize();

			test.eqCross(&dir, &vcor);
			test.dotWith(&norm, &u);

			//Which direction do we turn?
			if(f > zero)
			{
				//moving forward
				if((u > zero) && (rightwardvel < zero))
				{
					//turn right a tad
					upAxis.setToRotation((real)-0.4*dt, &norm);
				}
				else
				{
					//turn left a tad
					upAxis.setToRotation((real)0.4*dt, &norm);
				}
			}
			else
			{
				//moving backward
				if((u > zero) && (rightwardvel > zero))
				{
					//turn right a tad
					upAxis.setToRotation((real)-0.4*dt, &norm);
				}
				else
				{
					//turn left a tad
					upAxis.setToRotation((real)0.4*dt, &norm);
				}
			}

			//apply "torque"
			upAxis.preMultSingle(&dir, &(elements[m_iElementIdx].d));
/*
			elements[m_iElementIdx].d.scale((real)9.0);
			elements[m_iElementIdx].d.addEq(&right);
			elements[m_iElementIdx].d.normalize();
*/

			//take some energy away from the system -- inelastic collisions
			elements[m_iElementIdx].v.scale((real)0.995);
//			right.scale(rightwardvel * 1.2f);
//			elements[m_iElementIdx].v.addEq(&right);
		}
	}

}


