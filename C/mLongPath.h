
/* Neal McDonald 2009
 Excuse: 
		math for splines from more than 4 points. 
	*/
#ifndef M_LONGPATH_CLASS
#define M_LONGPATH_CLASS


/////////////////////////////////

#include "mPath.h"

/////////////////////////////////


// a set of (concatenated) jnPaths.
// t is scaled and translated so that moving t 
// from 0-1 returns points all paths in turn.
typedef struct {
	int		m_iNumPoints;
	int		m_iNumPaths;
	mPath	*paths;
	real	deltaT;	//distance between the subpath points (1 / (numPoints - 3) )	
} mLongPath, *mLongPathPtr; 

void mlpInit(mLongPathPtr p);

void mlpCopy(mLongPathPtr p, mLongPath *path);

//void setup(const int nPoints, real *data);
void mlpSetup(mLongPathPtr p, int nPoints, mVec3 *points, const bool closed);


//////// call setup before you call these functions.
void mlpOneSample(mLongPathPtr p, real t, mVec3 *res);

// evenly-spaced samples including ones at t=0.0 and 1.0
void mlpRegularSamples(mLongPathPtr p, real dt, mVec3 *res); 

// one sample for each t in ts
void mlpManySamples(mLongPathPtr p, int sampleCount, real *ts, mVec3 *res); 


// set p2 to be the closest point on the mPath to p. 
// use "accuracy" to tune execution speed and the accuracy of the results
// accuracy==0=>fastest  accuracy==1=>5 times more accurate
// 5 is probably close enough. I guess. 
// t is the parameter of p2.
// "holder" is the index of the mPath the point is closest to. 
// it's a hider violater, but it makes some optimizations possible. 
void mlpClosest(mLongPathPtr p, mVec3 *p, int accuracy, 
				real *distSq, mVec3 *p2, real *t, int *holder); 

// use "holder" to provide a hint as to where to look-- it's a speedup. 
void mlpUpdateClosest(mLongPathPtr pl, mVec3 *p, int accuracy, 
					  real *distSq, mVec3 *p2, real *t, int *holder); 






#endif // M_LONGPATH_CLASS

