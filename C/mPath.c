

/* Neal McDonald 2009
 Excuse: 2d spline implementation
 */

#include "mPath.h"

#include <math.h>





/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////



/*	Sets "M" to be the mx used to generate points
 that interpolate the "dataPts" 
 */
void mpSetupM(pathPtr t, mVec3 dataPts[4]) {
	
	mvCopy(&(t->m_cCorners[0]), &(dataPts[0])); 
	mvCopy(&(t->m_cCorners[1]), &(dataPts[1])); 
	mvCopy(&(t->m_cCorners[2]), &(dataPts[2])); 
	mvCopy(&(t->m_cCorners[3]), &(dataPts[3])); 
	
	
	//	for (i=0; i<3; i++) {
	//		M[12+i] = dataPts[i];
	//		M[ 8+i]  = 3 * (dataPts[3+i] - dataPts[i]);  
	//		M[ 4+i]  = 3 * (dataPts[6+i] - dataPts[3+i]) - M[8+i];
	//		M[   i]  = dataPts[9+i] - dataPts[i] - M[8+i] - M[4+i];
	//	}
	
	t->M[12] = dataPts[0].x;
	t->M[ 8] = M_THREE * (dataPts[1].x - dataPts[0].x);  
	t->M[ 4] = M_THREE * (dataPts[2].x - dataPts[1].x) - t->M[8];
	t->M[ 0] = dataPts[3].x - dataPts[0].x - t->M[8] - t->M[4];
	
	t->M[13] = dataPts[0].y;
	t->M[ 9] = M_THREE * (dataPts[1].y - dataPts[0].y);  
	t->M[ 5] = M_THREE * (dataPts[2].y - dataPts[1].y) - t->M[9];
	t->M[ 1] = dataPts[3].y - dataPts[0].y - t->M[9] - t->M[5];
	
	t->M[14] = dataPts[0].z;
	t->M[10] = M_THREE * (dataPts[1].z - dataPts[0].z);  
	t->M[ 6] = M_THREE * (dataPts[2].z - dataPts[1].z) - t->M[10];
	t->M[ 2] = dataPts[3].z - dataPts[0].z - t->M[10] - t->M[6];
	
	t->M[15] = M_ZERO; 
	t->M[11] = M_ZERO;
	t->M[ 7] = M_ZERO;
	t->M[ 3] = M_ZERO;
	
}

void mpCopy(pathPtr t, mPath *p) {
	int i; 
	for (i=0; i<4; ++i) {
		mvCopy(&(t->m_cCorners[i]), &(p->m_cCorners[i])); 
	}
	
	for(i=0;i<16;i++) {
		t->M[i] = p->M[i];
	}
}









void mpOneSample(pathPtr p, real t, mVec3 *result) {
	real tt = t*t;
	real ttt = tt*t; 
	
	result->x = (p->M[0] * ttt) + (p->M[4] * tt) + (p->M[8]  * t) + p->M[12]; 
	result->y = (p->M[1] * ttt) + (p->M[5] * tt) + (p->M[9]  * t) + p->M[13]; 
	result->z = (p->M[2] * ttt) + (p->M[6] * tt) + (p->M[10] * t) + p->M[14]; 
}










void mpRegularSamples(pathPtr t, real dt, mVec3 *result) {
	int i; 
	real r0, dr0, ddr0, dddr0, t; 
	real r1, dr1, ddr1, dddr1; 
	real r2, dr2, ddr2, dddr2; 
	
	r0 = t->M[12]; 
	r1 = t->M[13]; 
	r2 = t->M[14]; 
	dr0 = t->M[8]; 
	dr1 = t->M[9]; 
	dr2 = t->M[10]; 
	ddr0 = M_TWO * t->M[4]; 
	ddr1 = M_TWO * t->M[5]; 
	ddr2 = M_TWO * t->M[6]; 
	dddr0 = M_SIX * t->M[0]; 
	dddr1 = M_SIX * t->M[1]; 
	dddr2 = M_SIX * t->M[2]; 
	
	i = 0; 
	for (t=M_ZERO; t<(M_ONE+dt); t+=dt) {
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











void mpManySamples(pathPtr t, int sampleCount, 
				   real *ts,
				   mVec3 *result) {
	int i; 
	real r0, dr0, ddr0, dddr0, pdt; 
	real r1, dr1, ddr1, dddr1; 
	real r2, dr2, ddr2, dddr2; 
	
	r0 = t->M[12]; 
	r1 = t->M[13]; 
	r2 = t->M[14]; 
	dr0 = t->M[8]; 
	dr1 = t->M[9]; 
	dr2 = t->M[10]; 
	ddr0 = M_TWO * t->M[4]; 
	ddr1 = M_TWO * t->M[5]; 
	ddr2 = M_TWO * t->M[6]; 
	dddr0 = M_SIX * t->M[0]; 
	dddr1 = M_SIX * t->M[1]; 
	dddr2 = M_SIX * t->M[2]; 
	
	i = 0; 
	pdt = M_ZERO;
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







void mpClosest(pathPtr t, mVec3 *p, int accuracy, real *distSq, mVec3 *p2, real *time) {
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
			if (ts[j]<M_ZERO) { ts[j] = M_ZERO; }
			if (ts[j]>M_ZERO) { ts[j] = M_ONE; }
			mpOneSample(t, ts[j], &(row[j])); 
		}
		
		// find the closest one. 
		mvDistanceFromsq(p, &(row[0]), &d); 
		mind = d; 
		which = 0; 
		for (j=1; j<12; ++j) { 
			mvDistanceFromsq(p, &(row[j]), &d); 
			if (d<mind) { 
				mind = d; 
				which = j; 
			}
		}
		
		// copy out the results
		*distSq = mind; 
		mvCopy(p2, &(row[which])); 
		*time = ts[which]; 
		
		// ensure there are points before and after
		//		if (which==0) { which=1; }
		//		if (which==11) { which=10; }
		// get ready for the next loop
		closestt = ts[which]; 
		gapt *= M_FIFTH;
	}
	
	
}









