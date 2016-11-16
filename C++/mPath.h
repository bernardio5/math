// Neal McDonald 2002





#ifndef M_PATH_TYPE
#define M_PATH_TYPE

/* 	Encapsulate formulae for different spline types. 
	Provide a 2d curved line type. 

	Many formulae for interpolating points. 
	Pick one, stick with it; shouldn't matter. 
	*/

#include "mVec3.h" 
typedef enum {
	MSP_SPLINE_ERROR = 0,  
	MSP_SPLINE_CARDINAL, 
	MSP_SPLINE_BEZIER, 
	MSP_SPLINE_QUAD, 
	MSP_SPLINE_BES4,
	MSP_SPLINE_LAST
} mSplineType; 



class mPath {
public:
	// no, no arguments. first make, then fill w/data, then sample
	mPath() {}
	~mPath() {}

	void copy(mPath *p);

	// call this before asking for samples. 
	void setupM(mVec3 dataPts[4]);

	// get data out of the spline this way. 
	void oneSample(const real t, mVec3 *res); 

	// evenly-spaced samples including ones at t=0.0 and 1.0
	void regularSamples(real dt, mVec3 *res); 

	// one sample for each t in ts
	void manySamples(const int sampleCount, real *ts, mVec3 *res); 


	////// these functions are used by mLongPath to find the closest point
	
	// find the element of m_cCorners closest to p
	void minCornerDistSquared(mVec3 *p, real *result);

	// set p2 to be the closest point on the mPath to p. 
	// use "accuracy" to tune execution speed and the accuracy of the results
	// accuracy==0=>fastest  accuracy==1=>5 times more accurate
	// 5 is probably close enough. I guess. 
	void closest(mVec3 *p, int accuracy, real *distSq, mVec3 *p2, real *t); 


private:
	mVec3	m_cCorners[4]; 
	// a cache to speed up calls for constant data, varying t
	real	M[16];
};





#endif // M_PATH_TYPE
