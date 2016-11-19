/* Neal McDonald 2009
	The number of points in the input parameter must be
					4 + (3n)
*/


#include "mLongPath.h" 
#include <math.h>



void mlpInit(mLongPathPtr p) {
	p->paths = 0;
	p->m_iNumPoints = 0;
	p->m_iNumPaths = 0;
	p->deltaT = 0.0f;
}








void mlpCopy(mLongPathPtr p, mLongPath *path) {
	int i;
	
	p->m_iNumPoints = path->m_iNumPoints;
	p->m_iNumPaths = path->m_iNumPaths;
	p->deltaT = path->deltaT;

	if(p->paths != 0) {
		delete [] paths;
	}
	
	p->paths = new mPath[m_iNumPaths];

	for(i=0;i<(p->m_iNumPaths);i++) {
		mpCopy(&(p->paths[i]), &(path->paths[i]));
	}
}






void mlpSetup(mLongPathPtr p, int nPoints, mVec3 *points, bool closed) {
	mVec3	*newPoints, d, temp;
	int		i, j,numNewPoints;
	
	p->m_iNumPoints = nPoints;

	if(p->m_iNumPoints < 4) {
		return;
	}
	
	if (closed) {
		numNewPoints = 1+3*(nPoints);
	} else {
		numNewPoints = 1+3*(nPoints-1);
	}

	newPoints = new mVec3[numNewPoints];

	for (i=1;i<nPoints-1;++i) {
		mvEqSub(&d, &(points[i+1]), &(points[i-1]));
		mvScale(&d, 0.20f);
		j = 3*i;
		mvCopy(&(newPoints[j]), &(points[i]));
		mvEqSub(&(newPoints[j-1]), &(points[i]), &d);
		mvEqAdd(&(newPoints[j+1]), &(points[i]), &d);
	}
	
	mvCopy(&(newPoints[0]), &(points[0]));
	if (closed) {
		//last point = first point
		mvCopy(&(newPoints[numNewPoints-1]), &(newPoints[0]));

		//calc for second point and next to last point, based on point 1 and point nPoints-1
		mvEqSub(&d, &(points[1]),&(points[nPoints-1]));
		mvScale(&d, 0.20f);
		//second point
		mvEqAdd(&(newPoints[1]), &(points[0]), &d);
		//next to last point
		mvEqSub(&(newPoints[numNewPoints-2]), &(points[0]), &d);

		//three points before the last two points
		mvEqSub(&d, &(points[0]),&(points[nPoints-2]));
		mvScale(&d, 0.20f);
		mvCopy(&(newPoints[numNewPoints-4]), &(points[nPoints-1]));
		mvEqSub(&(newPoints[numNewPoints-5]), &(points[nPoints-1]), &d);
		mvEqAdd(&(newPoints[numNewPoints-3]), &(points[nPoints-1]), &d);
	} else {
		mvEqAdd(&(newPoints[1]), &(newPoints[2]), &(newPoints[0]));
		mvScale(&(newPoints[1]), 0.5f);
		mvCopy(&(newPoints[numNewPoints-1]), &(points[nPoints-1]));
		mvEqAdd(&(newPoints[numNewPoints-2), &(newPoints[numNewPoints-3]), &(newPoints[numNewPoints-1]));
		mvScale(&(newPoints[numNewPoints-2]), 0.5f);
	}

	p->m_iNumPaths = 1 + (numNewPoints-4)/3;

	p->paths = new mPath[m_iNumPaths];

	for (i=0; i<(m_iNumPaths); i++) {
		mpSetupM(&(paths[i]), &(newPoints[i*3]));
	}
	//Interval between paths
	m->deltaT = 1.0f / (real)(m_iNumPaths);

	delete [] newPoints;
}

				  
				  
				  
				  
				  
				  
void mlpOneSample(mLongPathPtr p, const real t, mVec3 *res) {
	real tPrime, temp, adjustedT;
	int whichPath;

	//Ensure that:
	//0.0 >= t > 1.0
	//Yay modulo!
	adjustedT = (real)floorf((double)(t / M_ONE));
	adjustedT = t - adjustedT;

	tPrime = (adjustedT / deltaT);

	temp = (real)floor((double)tPrime);
	tPrime = tPrime - temp;


	//pick the appropriate path
	whichPath = ((int)floor(adjustedT / deltaT))%(p->m_iNumPaths);

	mpOneSample(&(paths[whichPath]), tPrime, res);
}



				  

				  
				  
				  
				  
				  
// find the point on the path closest to p.
void mlpClosest(mLongPathPtr pl, mVec3 *p, int accuracy, real *distSq, mVec3 *p2, real *t, int *holder) {
	real dsq, mind, localt, foundt; 
	int i, j, which; 
	mVec3 p2loc; 

	// roughly find the closest point-- remember which segment it's on.
	mpClosest(&(pl->paths[0]), p, 0, &mind, &p2loc, &localt); 
	which = 0; 
	for (i=1; i<(pl->m_iNumPaths); ++i) { 
		mpClosest(&(pl->paths[i]), p, 0, &dsq, &p2loc, &localt); 
		if (dsq<mind) { 
			mind = dsq; 
			which = i;
		}
	}

	// look really hard at the 3 segments near the prelim closest;
	// find the closest for those 3
	mpClosest(&(pl->paths[which]), p, accuracy, distSq, p2, &foundt); 
	*holder = which;

	// search the one after
	j = (which+1);
	if (j>=(pl->m_iNumPaths)) { j = 0; }
	mpClosest(&(pl->paths[j]), p, accuracy, &dsq, &p2loc, &localt); 
	if (dsq<(*distSq)) { 
		mvCopy(&p2, &p2loc); 
		*distSq = dsq; 
		*holder = j;
		foundt = localt; 
	}
	
	// search the one before
	j = (which-1);
	if (j<0) { j = (pl->m_iNumPaths)-1; }
	mpClosest(&(pl->paths[j]), p, accuracy, &dsq, &p2loc, &localt); 
	if (dsq<(*distSq)) { 
		mvCopy(&p2, &p2loc); 
		*distSq = dsq; 
		*holder = j;
		foundt = localt; 
	}
	*t = ((1.0f/(pl->m_iNumPaths))*(*holder)) + (foundt/(pl->m_iNumPaths)); 
}

				  







// find the point on the path closest to p.
void mlpUpdateClosest(mLongPathPtr pl, mVec3 *p, int accuracy, real *distSq, mVec3 *p2, real *t, int *holder) {
	// oh, go away. 
	mlpClosest(pl, p, accuracy, distSq, p2, t, holder); 
}




