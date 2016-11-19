/* // Neal McDonald 2009


	17 Jan 2003:	Current implementation involves concatenating Bézier Splines.
					Therefore, the number of points in the input parameter must be
					4 + (3n)
*/


#include "mLongPath.h" 
#include "mReal.h"
#include <math.h>



mLongPath::mLongPath()
{
	paths = NULL;
	m_iNumPoints = 0;
	m_iNumPaths = 0;
	deltaT = 0.0f;
}

mLongPath::~mLongPath()
{
	delete [] paths;
}

void mLongPath::copy(mLongPath *path)
{
	m_iNumPoints = path->m_iNumPoints;
	m_iNumPaths = path->m_iNumPaths;
	deltaT = path->deltaT;

	if(paths != NULL)
		delete [] paths;

	paths = new mPath[m_iNumPaths];

	for(int i=0;i<(m_iNumPaths);i++)
	{
		paths[i].copy(&(path->paths[i]));
	}
}

void mLongPath::setup(const int nPoints, mVec3 *points, const bool closed)
{
	mVec3	*newPoints, d, temp;
	int		j,numNewPoints;
	
	m_iNumPoints = nPoints;

	if(m_iNumPoints < 4)
		return;

	if(closed)
	{
		numNewPoints = 1+3*(nPoints);
	}
	else
	{
		numNewPoints = 1+3*(nPoints-1);
	}

	newPoints = new mVec3[numNewPoints];

	for(int i=1;i<nPoints-1;++i)
	{
		d.eqSub(&(points[i+1]),&(points[i-1]));
		d.scale(0.20f);
		j = 3*i;
		newPoints[j].copy(&(points[i]));
		newPoints[j-1].eqSub(&(points[i]), &d);
		newPoints[j+1].eqAdd(&(points[i]), &d);
	}
	newPoints[0].copy(&(points[0]));
	if(closed)
	{
		//last point = first point
		newPoints[numNewPoints-1].copy(&(newPoints[0]));

		//calc for second point and next to last point, based on point 1 and point nPoints-1
		d.eqSub(&(points[1]),&(points[nPoints-1]));
		d.scale(0.20f);
		//second point
		newPoints[1].eqAdd(&(points[0]), &d);
		//next to last point
		newPoints[numNewPoints-2].eqSub(&(points[0]), &d);

		//three points before the last two points
		d.eqSub(&(points[0]),&(points[nPoints-2]));
		d.scale(0.20f);
		newPoints[numNewPoints-4].copy(&(points[nPoints-1]));
		newPoints[numNewPoints-5].eqSub(&(points[nPoints-1]), &d);
		newPoints[numNewPoints-3].eqAdd(&(points[nPoints-1]), &d);
	}
	else
	{
		newPoints[1].eqAdd(&(newPoints[2]),&(newPoints[0]));
		newPoints[1].scale(0.5f);
		newPoints[numNewPoints-1].copy(&(points[nPoints-1]));
		newPoints[numNewPoints-2].eqAdd(&(newPoints[numNewPoints-3]),&(newPoints[numNewPoints-1]));
		newPoints[numNewPoints-2].scale(0.5f);
	}

	m_iNumPaths = 1 + (numNewPoints-4)/3;

	paths = new mPath[m_iNumPaths];

	for(int i=0;i<(m_iNumPaths);i++)
	{
		paths[i].setupM(&(newPoints[i*3]));
	}
	//Interval between paths
	deltaT = 1.0f / (real)(m_iNumPaths);

	delete [] newPoints;
}

void mLongPath::oneSample(const real t, mVec3 *res)
{
	const real one = (real)1.0;
	const real zero = (real)0.0;
	real tPrime, temp, adjustedT;
	int whichPath;

	//Ensure that:
	//0.0 >= t > 1.0
	//Yay modulo!
	adjustedT = (real)floor((double)(t / one));
	adjustedT = t - adjustedT;

	tPrime = (adjustedT / deltaT);

	temp = (real)floor((double)tPrime);
	tPrime = tPrime - temp;


	//pick the appropriate path
	whichPath = ((int)floor(adjustedT / deltaT))%m_iNumPaths;

	paths[whichPath].oneSample(tPrime, res);
}



// find the point on the path closest to p.
void mLongPath::closest(mVec3 *p, int accuracy, real *distSq, mVec3 *p2, real *t, int *holder) {
	real dsq, mind, localt, foundt; 
	int i, j, which; 
	mVec3 p2loc; 

	// roughly find the closest point-- remember which segment it's on.
	paths[0].closest(p, 0, &mind, &p2loc, &localt); 
	which = 0; 
	for (i=1; i<m_iNumPaths; ++i) { 
		paths[i].closest(p, 0, &dsq, &p2loc, &localt); 
		if (dsq<mind) { 
			mind = dsq; 
			which = i;
		}
	}

	// look really hard at the 3 segments near the prelim closest;
	// find the closest for those 3
	paths[which].closest(p, accuracy, distSq, p2, &foundt); 
	*holder = which;

	// search the one after
	j = (which+1);
	if (j>=m_iNumPaths) { j = 0; }
	paths[j].closest(p, accuracy, &dsq, &p2loc, &localt); 
	if (dsq<(*distSq)) { 
		p2->copy(&p2loc); 
		*distSq = dsq; 
		*holder = j;
		foundt = localt; 
	}
	
	// search the one before
	j = (which-1);
	if (j<0) { j = m_iNumPaths-1; }
	paths[j].closest(p, accuracy, &dsq, &p2loc, &localt); 
	if (dsq<(*distSq)) { 
		p2->copy(&p2loc); 
		*distSq = dsq; 
		*holder = j;
		foundt = localt; 
	}
	*t = ((1.0f/m_iNumPaths)*(*holder)) + (foundt/m_iNumPaths); 
}



// find the point on the path closest to p.
void mLongPath::updateClosest(mVec3 *p, int accuracy, real *distSq, mVec3 *p2, real *t, int *holder) {
}




