

//	mPart	
// simple particle object; extend me
// yawn. done this too may times.

#ifndef MPART_TYPE
#define MPART_TYPE

#include "mVec3.h"


typedef struct {
	mVec3 p, v, a; // position, velocity, acceleration
	float m, age, span; // mass, age, lifespan
	int alive, ID;
} mPart; 


void mPart_copy(mPart *to, mPart *from);


#endif

// Neal McDonald 1/1/2005