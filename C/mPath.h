
/* Neal McDonald 2009
 Excuse: 
 
 Encapsulate formulae for different spline types. 
 Provide a 2d curved line type. 
 
 */



#ifndef M_PATH_TYPE
#define M_PATH_TYPE

#include "mVec3.h" 

/* many formulae for interpolating points. 
 Pick one, stick with it; shouldn't matter. 
 
 typedef enum {
 MSP_SPLINE_ERROR = 0,  
 MSP_SPLINE_CARDINAL, 
 MSP_SPLINE_BEZIER, 
 MSP_SPLINE_QUAD, 
 MSP_SPLINE_BES4,
 MSP_SPLINE_LAST
 } mSplineType; 
 */


typedef struct {
	mVec3	m_cCorners[4]; 
	real	M[16];
} path, *pathPtr; 


// no, no arguments. first make, then fill w/data, then sample
void mpInit(pathPtr *t);

void mpCopy(mPath *p);

// call this before asking for samples. 
//void setupM(const mSplineType t, mVec3 dataPts[4]);
void mpSetupM(mVec3 dataPts[4]);

// get data out of the spline this way. 
void mpOneSample(const real t, mVec3 *res); 

// evenly-spaced samples including ones at t=0.0 and 1.0
void mpRegularSamples(real dt, mVec3 *res); 

// one sample for each t in ts
void mpManySamples(const int sampleCount, real *ts, mVec3 *res); 


////// these functions are used by mLongPath to find the closest point

// find the element of m_cCorners closest to p
void mpMinCornerDistSquared(mVec3 *p, real *result);

// set p2 to be the closest point on the mPath to p. 
// use "accuracy" to tune execution speed and the accuracy of the results
// accuracy==0=>fastest  accuracy==1=>5 times more accurate
// 5 is probably close enough. I guess. 
void mpClosest(mVec3 *p, int accuracy, real *distSq, mVec3 *p2, real *t); 



#endif // M_PATH_TYPE
