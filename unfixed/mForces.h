

//	mForce 	
// a few forces to exert on particles


#ifndef MFORCE_TYPE
#define MFORCE_TYPE

#include "mPart.h"

typedef enum {
	MFORCE_AIR_FRICTION=0, // v*=.9ish for all
	MFORCE_COORD_WALL, // off which all bounce
	MFORCE_SPRING, // two parts
	MFORCE_RIVET, // one part
	MFORCE_GRAVITY, // all parts
	MFORCE_PISTON,
} mForceType;


typedef struct {
	int p1, p2, p3; // ID's of particles
	int type;
	// OOP fail: just use the arrays to stash data
	// yes, a child class is what Spock would do.
	int is[4];
	real fs[4];
} mForce; 


void mForce_friction(mForce *f, real k);
void mForce_coordWall(mForce *f, int which, real value);
void mForce_spring(mForce *f, int p1, int p2, real k, real restlen);
void mForce_rivet(mForce *f, int p1);
void mForce_gravity(mForce *f, real g);
void mForce_piston(mForce *f, real g);


void mForce_compute(mForce *f, mPart *ps, int numParts, real dt);


#endif

// Neal McDonald 1/1/2005