// Neal McDonald 2002



#include "mPath.h"


//#include "stdHeaders.h"
#include <math.h>





/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////



/*	Sets "M" to be the mx used to generate points
	that interpolate the "dataPts" 
	*/
void mPath::setupM(mVec3 dataPts[4]) {
	real three, zero; 

	three = (real)3.0;
	zero = (real)0.0;

	m_cCorners[0].copy(&(dataPts[0])); 
	m_cCorners[1].copy(&(dataPts[1])); 
	m_cCorners[2].copy(&(dataPts[2])); 
	m_cCorners[3].copy(&(dataPts[3])); 

//	for (i=0; i<3; i++) {
//		M[12+i] = dataPts[i];
//		M[ 8+i]  = 3 * (dataPts[3+i] - dataPts[i]);  
//		M[ 4+i]  = 3 * (dataPts[6+i] - dataPts[3+i]) - M[8+i];
//		M[   i]  = dataPts[9+i] - dataPts[i] - M[8+i] - M[4+i];
//	}

	M[12] = dataPts[0].x;
	M[ 8] = three * (dataPts[1].x - dataPts[0].x);  
	M[ 4] = three * (dataPts[2].x - dataPts[1].x) - M[8];
	M[ 0] = dataPts[3].x - dataPts[0].x - M[8] - M[4];

	M[13] = dataPts[0].y;
	M[ 9] = three * (dataPts[1].y - dataPts[0].y);  
	M[ 5] = three * (dataPts[2].y - dataPts[1].y) - M[9];
	M[ 1] = dataPts[3].y - dataPts[0].y - M[9] - M[5];

	M[14] = dataPts[0].z;
	M[10] = three * (dataPts[1].z - dataPts[0].z);  
	M[ 6] = three * (dataPts[2].z - dataPts[1].z) - M[10];
	M[ 2] = dataPts[3].z - dataPts[0].z - M[10] - M[6];

	M[15] = zero; 
	M[11] = zero;
	M[ 7] = zero;
	M[ 3] = zero;

}

void mPath::copy(mPath *p)
{
	int i; 
	for (i=0; i<4; ++i) {
		m_cCorners[i].copy(&(p->m_cCorners[i])); 
	}

	for(i=0;i<16;i++) {
		M[i] = p->M[i];
	}
}









void mPath::oneSample(real t, mVec3 *result) {
	real tt = t*t;
	real ttt = tt*t; 

	result->x = (M[0] * ttt) + (M[4] * tt) + (M[8]  * t) + M[12]; 
	result->y = (M[1] * ttt) + (M[5] * tt) + (M[9]  * t) + M[13]; 
	result->z = (M[2] * ttt) + (M[6] * tt) + (M[10] * t) + M[14]; 
}










void mPath::regularSamples(real dt, 
						mVec3 *result) {
	int i; 
	real r0, dr0, ddr0, dddr0, t; 
	real r1, dr1, ddr1, dddr1; 
	real r2, dr2, ddr2, dddr2; 
	real six, two, one, zero; 

	six = (real)6.0; 
	two = (real)2.0; 
	one = (real)1.0; 
	zero = (real)0.0; 
	
	r0 = M[12]; 
	r1 = M[13]; 
	r2 = M[14]; 
	dr0 = M[8]; 
	dr1 = M[9]; 
	dr2 = M[10]; 
	ddr0 = two * M[4]; 
	ddr1 = two * M[5]; 
	ddr2 = two * M[6]; 
	dddr0 = six * M[0]; 
	dddr1 = six * M[1]; 
	dddr2 = six * M[2]; 

	i = 0; 
	for (t=zero; t<(one+dt); t+=dt) {
		ddr0 += dddr0 * dt;
		ddr1 += dddr1 * dt;
		ddr2 += dddr2 * dt;

		dr0 += ddr0 * dt; 
		dr1 += ddr1 * dt; 
		dr2 += ddr2 * dt; 

		r0 += dr0 * dt;
		r1 += dr1 * dt;
		r2 += dr2 * dt;

		result[i].x = r0; 
		result[i].y = r1; 
		result[i].z = r2; 
		++i; 
	}
}











void mPath::manySamples(int sampleCount, 
						real *ts,
						mVec3 *result) {
	int i; 
	real r0, dr0, ddr0, dddr0, pdt; 
	real r1, dr1, ddr1, dddr1; 
	real r2, dr2, ddr2, dddr2; 
	real six, two, one, zero; 

	six = (real)6.0; 
	two = (real)2.0; 
	one = (real)1.0; 
	zero = (real)0.0; 
	
	r0 = M[12]; 
	r1 = M[13]; 
	r2 = M[14]; 
	dr0 = M[8]; 
	dr1 = M[9]; 
	dr2 = M[10]; 
	ddr0 = two * M[4]; 
	ddr1 = two * M[5]; 
	ddr2 = two * M[6]; 
	dddr0 = six * M[0]; 
	dddr1 = six * M[1]; 
	dddr2 = six * M[2]; 

	i = 0; 
	pdt = zero;
	for (i=0; i<sampleCount; ++i) {
		pdt = ts[i] - pdt; 
		ddr0 += dddr0 * pdt;
		ddr1 += dddr1 * pdt;
		ddr2 += dddr2 * pdt;

		dr0 += ddr0 * pdt; 
		dr1 += ddr1 * pdt; 
		dr2 += ddr2 * pdt; 

		r0 += dr0 * pdt;
		r1 += dr1 * pdt;
		r2 += dr2 * pdt;

		result[i].x = r0; 
		result[i].y = r1; 
		result[i].z = r2; 
		++i; 
	}
}







void mPath::closest(mVec3 *p, int accuracy, real *distSq, mVec3 *p2, real *t) {
	real d, mind, gapt, closestt; 
	mVec3 row[11]; 
	real a, ts[11]; 
	int i, j, which; 

	gapt = 0.1f; 
	closestt = 0.5f; 

	for (i=0; i<=accuracy; ++i) { 
		// get samples for 10 subdivisions of gapt
		for (j=0; j<11; j++) {
			a = (real)(j-5); 
			ts[j] = closestt + (a*gapt); 
			if (ts[j]<0.0f) { ts[j] = 0.0f; }
			if (ts[j]>1.0f) { ts[j] = 1.0f; }
			oneSample(ts[j], &(row[j])); 
		}

		// find the closest one. 
		p->distanceFromsq(&(row[0]), &d); 
		mind = d; 
		which = 0; 
		for (j=1; j<12; ++j) { 
			p->distanceFromsq(&(row[j]), &d); 
			if (d<mind) { 
				mind = d; 
				which = j; 
			}
		}

		// copy out the results
		*distSq = mind; 
		p2->copy(&(row[which])); 
		*t = ts[which]; 

		// ensure there are points before and after
//		if (which==0) { which=1; }
//		if (which==11) { which=10; }
		// get ready for the next loop
		closestt = ts[which]; 
		gapt *= 0.2f;
	}


}









