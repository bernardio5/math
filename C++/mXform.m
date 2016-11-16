// Neal McDonald 2002

#include "mXform.h"
#include "physManager.h" 
#include "entManager.h" 
#include <math.h>

mXform::mXform()
{
	static real zero = (real)0.0; 
	static real one = (real)1.0; 

	m_cParent = NULL; 
	m_iMVersion = 0; 
	m_cM.setToIdentity(); 
	m_cR.setToIdentity();
	m_iRVersion = -1;
}

mXform::~mXform() {}

mXform *mXform::getParent()
{
	return m_cParent;
}
void mXform::setParent(mXform *parent)
{
	m_cParent = parent; 
	++m_iMVersion; 
} 

void mXform::setMVersion(int v)
{
	m_iMVersion = v;
}
int mXform::getMVersion()
{
	return m_iMVersion;
}
void mXform::setRVersion(int v)
{
	m_iRVersion = v;
}
int mXform::getRVersion()
{
	return m_iRVersion;
}

void mXform::update() {}

mXform *mXform::clone()
{
	return new mXform();
}

void mXform::move(mVec3 *in, mVec3 *out)
{
	update();
	m_cR.preMultSingle(in, out); 
}

void mXform::moveMany(const int ct, mVec3 *in, mVec3 *out)
{
	update();
	m_cR.preMultMany(ct, in, out); 
}


///////////////
// Billboard //
///////////////
mXformBillboard::mXformBillboard(entManager *em, mVec3 *pos)
{
	::mXform();
	m_pEntMgr = em;
	m_cP.copy(pos);
	update(); 
}

mXformBillboard::~mXformBillboard() {}

mXform *mXformBillboard::clone()
{
	mXformBillboard *out = new mXformBillboard(m_pEntMgr, &m_cP);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	return out;
}

void mXformBillboard::update()
{
	m_cM.setToIdentity(); 
	m_cR.setToIdentity();

	if (m_cParent!= NULL)
	{
		m_cParent->m_cR.getPosition(&m_cP);
	}

	m_pEntMgr->getCurrentCameraPosition(&m_cD);
	m_cD.subEq(&m_cP);
	m_cD.normalize();

	m_cU.set((real)0.0, (real)1.0, (real)0.0);
	m_cRight.eqCross(&m_cD, &m_cU);
	m_cU.eqCross(&m_cRight, &m_cD);
	m_cU.normalize();

	m_cR.setToPosition(&m_cP, &m_cD, &m_cU);

	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}



////////////
// Scaler //
////////////
mXformScaler::mXformScaler(mVec3 *scale)
{
	::mXform();
	m_cScale.copy(scale);
	update(); 
}

mXformScaler::~mXformScaler() {}

mXform *mXformScaler::clone()
{
	mXformScaler *out = new mXformScaler(&m_cScale);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	return out;
}

void mXformScaler::update()
{
	m_cM.setToIdentity(); 
	m_cR.setToIdentity();

	//m_cM.setToScale(&m_cScale);
	m_cM.m_dM[0] *= m_cScale.x; //X scale
	m_cM.m_dM[5] *= m_cScale.y;//Y Scale
	m_cM.m_dM[10] *= m_cScale.z; //Z Scale

	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}

///////////////
// Scanner 1 //
///////////////
mXformScanner1::mXformScanner1(physManager *pm, const int p1)
{
	static real zero = (real)0.0; 
	static real one = (real)1.0;
	::mXform();
	m_iP1 = p1; 
	m_cPhysMgr = pm; 
	m_cP.set(zero, zero, zero);
	m_cD.set(zero, zero, one);
	m_cU.set(zero, one, zero); 
	update(); 
}

mXformScanner1::~mXformScanner1() {}

mXform *mXformScanner1::clone()
{
	mXformScanner1 *out = new mXformScanner1(m_cPhysMgr, m_iP1);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	out->m_cP.copy(&m_cP);
	out->m_cD.copy(&m_cD);
	out->m_cU.copy(&m_cU);
	return out;
}

void mXformScanner1::update()
{
	static real zero = (real)0.0;
	static real one = (real)1.0;
	static real epsilon = (real)0.00001;
	static real almostOne = (real)0.99999;

	//mVec3	oldD, oldU;
	mVec3	right;
	//real	len, dot;

	m_cM.setToIdentity(); 
	m_cR.setToIdentity();

//	oldD.copy(&m_cD); 
//	oldU.copy(&m_cU); 

	m_cPhysMgr->getElementPos(m_iP1, &m_cP);
	m_cPhysMgr->getElementDir(m_iP1, &m_cD);
	m_cPhysMgr->getElementUp(m_iP1, &m_cU);
/*
	right.eqCross(&m_cD, &m_cU);	//right vector
	m_cU.eqCross(&right, &m_cD);	//real up vector
*/
	m_cM.setToPosition(&m_cP, &m_cD, &m_cU);
	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}


///////////////
// Scanner 2 //
///////////////
mXformScanner2::mXformScanner2(physManager *pm, const int p1, const int p2)
{
	static real zero = (real)0.0; 
	static real one = (real)1.0;
	::mXform();
	m_iP1 = p1; 
	m_iP2 = p2; 
	m_cPhysMgr = pm; 
	m_cP.set(zero, zero, zero);
	m_cD.set(zero, zero, one); 
	m_cU.set(zero, one, zero); 
	update(); 
}

mXformScanner2::~mXformScanner2() {}

mXform *mXformScanner2::clone()
{
	mXformScanner2 *out = new mXformScanner2(m_cPhysMgr, m_iP1, m_iP2);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	out->m_cP.copy(&m_cP);
	out->m_cD.copy(&m_cD);
	out->m_cU.copy(&m_cU);
	return out;
}

void mXformScanner2::update()
{
	static real zero = (real)0.0;
	static real one = (real)1.0;
	static real epsilon = (real)0.00001;
	static real almostOne = (real)0.99999;

	mVec3	oldD, oldU;
	real	len, dot;

	m_cM.setToIdentity(); 
	m_cR.setToIdentity();

	m_cPhysMgr->getElementPos(m_iP1, &m_cP);
	m_cPhysMgr->getElementPos(m_iP2, &m_cD);
	m_cD.subEq(&m_cP); 
	m_cU.set(zero, one, zero); 

	// same as prev case
	m_cD.length3(&len); 
	if (len<epsilon)
	{
		m_cD.copy(&oldD); 
	}
	else
	{
		m_cD.scale(one/len); 
	}

	m_cD.dotWith(&m_cU, &dot); 
	if (dot>almostOne)
	{
		m_cU.eqCross(&m_cD, &m_cP); 
		m_cU.normalize(); 
	}

	m_cM.setToPosition(&m_cP, &m_cD, &m_cU);
	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}



///////////////
// Scanner 3 //
///////////////
mXformScanner3::mXformScanner3(physManager *pm, const int p1, const int p2, const int p3)
{
	static real zero = (real)0.0; 
	static real one = (real)1.0;
	::mXform();
	m_iP1 = p1; 
	m_iP2 = p2; 
	m_iP3 = p3; 
	m_cPhysMgr = pm; 
	m_cP.set(zero, zero, zero);
	m_cD.set(zero, zero, one); 
	m_cU.set(zero, one, zero); 
	update(); 
}

mXformScanner3::~mXformScanner3() {}

mXform *mXformScanner3::clone()
{
	mXformScanner3 *out = new mXformScanner3(m_cPhysMgr, m_iP1, m_iP2, m_iP3);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	out->m_cP.copy(&m_cP);
	out->m_cD.copy(&m_cD);
	out->m_cU.copy(&m_cU);
	return out;
}

void mXformScanner3::update()
{
	static real zero = (real)0.0;
	static real one = (real)1.0;
	static real epsilon = (real)0.00001;
	static real almostOne = (real)0.99999;

	mVec3	oldD, oldU;
	real	len, dot;

	m_cM.setToIdentity(); 
	m_cR.setToIdentity();

	m_cPhysMgr->getElementPos(m_iP1, &m_cP);
	m_cPhysMgr->getElementPos(m_iP2, &m_cD);
	m_cD.subEq(&m_cP); 
	m_cPhysMgr->getElementPos(m_iP3, &m_cU);
	m_cU.subEq(&m_cP); 

	// same as prev case
	m_cD.length3(&len); 
	if (len<epsilon) { 
		m_cD.copy(&oldD); 
	} else {
		m_cD.scale(one/len); 
	}

	m_cU.length3(&len); // but also length check u.
	if (len<epsilon) { 
		m_cU.copy(&oldU); 
	} else {
		m_cU.scale(one/len); 
	}

	m_cD.dotWith(&m_cU, &dot); 
	if (dot>almostOne) {
		// d parallel to u-- bad!
		m_cU.eqCross(&m_cD, &m_cP); 
		m_cU.normalize(); 
	}

	m_cM.setToPosition(&m_cP, &m_cD, &m_cU);
	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}


//////////////////
// Spline Mover //
//////////////////
mXformSplineMover::mXformSplineMover(mLongPath *path, const real speed, const real start, timeManager *tm)
{
	static real zero = (real)0.0; 
	static real one = (real)1.0;
	::mXform();

	m_pPath.copy(path);
	m_cP.set(zero, zero, zero);
	m_cD.set(zero, zero, one); 
	m_cU.set(zero, one, zero); 

	m_rSpeed = speed;
	m_rStart = start;
	m_cTimeMgr = tm;
	update(); 
}

mXformSplineMover::~mXformSplineMover() {}

mXform *mXformSplineMover::clone()
{
	mXformSplineMover *out = new mXformSplineMover(&m_pPath, m_rSpeed, m_rStart, m_cTimeMgr);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	out->m_cP.copy(&m_cP);
	out->m_cD.copy(&m_cD);
	out->m_cU.copy(&m_cU);

	out->m_cTimeMgr = m_cTimeMgr;
	return out;
}

void mXformSplineMover::update()
{
	mVec3 right, up;
	evnTime t, patht;

	m_cM.setToIdentity(); 
	m_cR.setToIdentity();

	//get the position at time m_rTime
	m_cTimeMgr->getGameTime(&t); 
	patht = t * m_rSpeed;
	if (patht > 1.0f) { patht -= (evnTime)floor(patht); }
	m_pPath.oneSample(patht, &m_cP);

	//get the orientation (X(m_rTime+speed) - X(m_rTime))
	patht += 0.01f;
	if (patht > 1.0f) { patht -= (evnTime)floor(patht); }

	m_pPath.oneSample(patht, &m_cD);
	m_cD.subEq(&m_cP);
	m_cD.normalize();

	up.set((real)0.0, (real)1.0, (real)0.0);

	right.eqCross(&m_cD, &up);

	//set the up vector
	m_cU.eqCross(&right, &m_cD);

	m_cM.setToPosition(&m_cP, &m_cD, &up);
	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}




//////////////////
// Tube Mover //
//////////////////
mXformTubeMover::mXformTubeMover(mTube *tube, mVec3 *params, mVec3 *dparams, timeManager *tm)
{
	static real zero = (real)0.0; 
	static real one = (real)1.0;
	::mXform();

	m_pTube = tube; 
	m_cStart.copy(params); 
	m_cSpeed.copy(dparams); 
	m_cTimeMgr = tm;

	update(); 
}

mXformTubeMover::~mXformTubeMover() {}

mXform *mXformTubeMover::clone()
{
	mXformTubeMover *out = new mXformTubeMover(m_pTube, &m_cStart, &m_cSpeed, m_cTimeMgr);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);
	return out;
}

void mXformTubeMover::update()
{
	mVec3 distance, p, n, dir, shellp;
	evnTime t;

	//get the position at time m_rTime
	m_cTimeMgr->getGameTime(&t); 
	distance.copy(&m_cSpeed);
	distance.scale(t); 
	distance.addEq(&m_cStart); 

	m_pTube->getPointNormal(&distance, &p, &n, &dir, &shellp); 
	m_cM.setToPosition(&p, &dir, &n);
	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL) {
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	} else { // or, if no parent..
		m_cR.setToIdentity();
	}

	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}



////////////////////
// Tube Mover Test//
////////////////////
mXformTubeMoverTest::mXformTubeMoverTest(mTube *tube, mVec3 *params, mVec3 *dparams, timeManager *tm)
{
	static real zero = (real)0.0; 
	static real one = (real)1.0;
	::mXform();

	m_pTube = tube; 
	m_cStart.copy(params); 
	m_cSpeed.copy(dparams); 
	m_cTimeMgr = tm;

	section = -1;

	update(); 
}

mXformTubeMoverTest::~mXformTubeMoverTest() {}

mXform *mXformTubeMoverTest::clone()
{
	mXformTubeMoverTest *out = new mXformTubeMoverTest(m_pTube, &m_cStart, &m_cSpeed, m_cTimeMgr);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);
	return out;
}

void mXformTubeMoverTest::update()
{
	mVec3 params, p, n, dir, shellp, centerp;
	evnTime t;

	//get the position at time m_rTime
	m_cTimeMgr->getGameTime(&t); 
	params.copy(&m_cSpeed);
	params.scale(t); 
	params.addEq(&m_cStart); 

	m_pTube->getPointNormal(&params, &p, &n, &dir, &shellp); 

	if (section==-1) {  
		m_pTube->map(&p, &params, &section); 
	} else {
		m_pTube->updateMap(&p, &params, &section); 
	}
	m_pTube->getPointNormal(&params, &p, &n, &dir, &shellp);

	m_cM.setToPosition(&p, &dir, &n);
	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL) {
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	} else { // or, if no parent..
		m_cR.setToIdentity();
	}

	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}









//////////////////
// Static Strut //
//////////////////
mXformStaticStrut::mXformStaticStrut(mVec3 *pos, mVec3 *pos2)
{
	static real zero = (real)0.0;
	static real one = (real)1.0;
	::mXform();
	m_cP.set(zero, zero, zero);
	m_cD.set(zero, zero, one); 
	m_cU.set(zero, one, zero);
	m_cPos.copy(pos);
	m_cPos2.copy(pos2);
	update(); 
}

mXformStaticStrut::~mXformStaticStrut() {}

mXform *mXformStaticStrut::clone()
{
	mXformStaticStrut *out = new mXformStaticStrut(&m_cPos2, &m_cPos2);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	out->m_cP.copy(&m_cP);
	out->m_cD.copy(&m_cD);
	out->m_cU.copy(&m_cU);

	out->m_cPos.copy(&m_cPos);
	out->m_cPos2.copy(&m_cPos2);
	return out;
}

void mXformStaticStrut::update()
{
/*
	static real zero = (real)0.0;
	static real one = (real)1.0;
	static real epsilon = (real)0.00001;
	static real almostOne = (real)0.99999;

	mVec3	oldD, oldU;
	real	len, dot;

	m_cM.setToIdentity(); 
	m_cR.setToIdentity();

	//m_cPhysMgr->getElementPos(m_iP1, &m_cP);
	//m_cPhysMgr->getElementPos(m_iP2, &m_cD);
	m_cP.copy(&m_cPos);
	m_cD.copy(&m_cPos2);

	m_cD.subEq(&m_cP); 
	m_cU.set(zero, one, zero); 

	// same as prev case
	m_cD.length3(&len); 
	if (len<epsilon)
	{
		m_cD.copy(&oldD); 
	}
	else
	{
		m_cD.scale(one/len); 
	}

	m_cD.dotWith(&m_cU, &dot); 
	if (dot>almostOne)
	{
		m_cU.eqCross(&m_cD, &m_cP); 
		m_cU.normalize(); 
	}

	m_cM.setToPosition(&m_cP, &m_cD, &m_cU);
	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
*/


	static real zero = (real)0.0;
	static real one = (real)1.0;
	static real epsilon = (real)0.00001;
	static real almostOne = (real)0.99999;

	mVec3	oldD, oldU, tmpRight, tmpScale;
	mx4		tmpScaleMatrix;
	real	len, dot;

	m_cM.setToIdentity(); 
	m_cR.setToIdentity();

	m_cP.copy(&m_cPos);
	m_cD.copy(&m_cPos2);

	m_cD.subEq(&m_cP); 
	m_cU.set(zero, one, zero); 

	//temp right vector
	tmpRight.eqCross(&m_cD, &m_cU);
	//cross the right vector with the dir vector to get a real up vector
	m_cU.eqCross(&tmpRight, &m_cD);

	// same as prev case
	m_cD.length3(&len); 
	if (len<epsilon)
	{
		m_cD.copy(&oldD); 
	}
	else
	{
		m_cD.scale(one/len); 
	}

	m_cD.dotWith(&m_cU, &dot); 
	if (dot>almostOne)
	{
		m_cU.eqCross(&m_cD, &m_cP); 
		m_cU.normalize(); 
	}
	tmpScale.set(0.0f, len, 0.0f);
	tmpScaleMatrix.setToIdentity();
	tmpScaleMatrix.m_dM[0] *= 0.25f; //X scale
	tmpScaleMatrix.m_dM[5] *= 0.25f; //Y Scale
	tmpScaleMatrix.m_dM[10] *= len; //Z Scale
	

	m_cM.setToPosition(&m_cP, &m_cD, &m_cU);
	m_cM.preMult(&tmpScaleMatrix);

	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 

}



///////////////////
// Dynamic Strut //
///////////////////
mXformDynamicStrut::mXformDynamicStrut(physManager *pm, const int p1, const int p2)
{
	static real zero = (real)0.0;
	static real one = (real)1.0;
	::mXform();
	m_cP.set(zero, zero, zero);
	m_cD.set(zero, zero, one); 
	m_cU.set(zero, one, zero);
	m_cPhysMgr = pm;
	m_iP1 = p1;
	m_iP2 = p2;
	update(); 
}

mXformDynamicStrut::~mXformDynamicStrut() {}

mXform *mXformDynamicStrut::clone()
{
	mXformDynamicStrut *out = new mXformDynamicStrut(m_cPhysMgr, m_iP1, m_iP2);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	out->m_cP.copy(&m_cP);
	out->m_cD.copy(&m_cD);
	out->m_cU.copy(&m_cU);

	return out;
}

void mXformDynamicStrut::update()
{
	static real zero = (real)0.0;
	static real one = (real)1.0;
	static real epsilon = (real)0.00001;
	static real almostOne = (real)0.99999;

	mVec3	oldD, oldU, tmpRight, tmpScale;
	mx4		tmpScaleMatrix;
	real	len, dot;

	m_cM.setToIdentity(); 
	m_cR.setToIdentity();

	m_cPhysMgr->getElementPos(m_iP1, &m_cP);
	m_cPhysMgr->getElementPos(m_iP2, &m_cD);
	m_cD.subEq(&m_cP); 
	m_cU.set(zero, one, zero); 

	//temp right vector
	tmpRight.eqCross(&m_cD, &m_cU);
	//cross the right vector with the dir vector to get a real up vector
	m_cU.eqCross(&tmpRight, &m_cD);

	// same as prev case
	m_cD.length3(&len); 
	if (len<epsilon)
	{
		m_cD.copy(&oldD); 
	}
	else
	{
		m_cD.scale(one/len); 
	}

	m_cD.dotWith(&m_cU, &dot); 
	if (dot>almostOne)
	{
		m_cU.eqCross(&m_cD, &m_cP); 
		m_cU.normalize(); 
	}
	tmpScale.set(0.0f, len, 0.0f);
	tmpScaleMatrix.setToIdentity();
//	tmpScaleMatrix.m_dM[0] *= 0.25f; //X scale
//	tmpScaleMatrix.m_dM[5] *= 0.25f; //Y Scale
	tmpScaleMatrix.m_dM[10] *= len; //Z Scale
	
	m_cM.setToPosition(&m_cP, &m_cD, &m_cU);
	m_cM.preMult(&tmpScaleMatrix);

	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 

}




/////////////////////
// Inverse Lorentz //
/////////////////////
mXformInverseLorentz::mXformInverseLorentz(physManager *pm, const int p1)
{
	static real zero = (real)0.0;
	static real one = (real)1.0;
	::mXform();
	m_cP.set(zero, zero, zero);
	m_cD.set(zero, zero, one); 
	m_cU.set(zero, one, zero);
	m_cPhysMgr = pm;
	m_iP1 = p1;
	update(); 
}

mXformInverseLorentz::~mXformInverseLorentz() {}

mXform *mXformInverseLorentz::clone()
{
	mXformInverseLorentz *out = new mXformInverseLorentz(m_cPhysMgr, m_iP1);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	out->m_cP.copy(&m_cP);
	out->m_cD.copy(&m_cD);
	out->m_cU.copy(&m_cU);

	return out;
}

void mXformInverseLorentz::update()
{
	static real zero = (real)0.0;
	static real one = (real)1.0;
	static real epsilon = (real)0.00001;
	static real almostOne = (real)0.99999;

	mVec3	oldD, oldU, tmpRight, tmpScale;
	mx4		tmpScaleMatrix;
	real	len;

	m_cM.setToIdentity(); 
	m_cR.setToIdentity();

	m_cPhysMgr->getElementPos(m_iP1, &m_cP);
	m_cPhysMgr->getElementDir(m_iP1, &m_cD);
	m_cD.subEq(&m_cP);

	//scale by speed
	m_cPhysMgr->getElementVel(m_iP1, &m_cP);
	m_cP.length3(&len);

	m_cU.set(zero, one, zero); 

	//temp right vector
	tmpRight.eqCross(&m_cD, &m_cU);
	//cross the right vector with the dir vector to get a real up vector
	m_cU.eqCross(&tmpRight, &m_cD);

	if (len<1.0f)
	{
		len = 1.0f;
	}
/*
	m_cD.dotWith(&m_cU, &dot); 
	if (dot>almostOne)
	{
		m_cU.eqCross(&m_cD, &m_cP); 
		m_cU.normalize(); 
	}
	tmpScale.set(0.0f, len, 0.0f);
	tmpScaleMatrix.setToIdentity();
//	tmpScaleMatrix.m_dM[0] *= 0.25f; //X scale
//	tmpScaleMatrix.m_dM[5] *= 0.25f; //Y Scale
	tmpScaleMatrix.m_dM[10] *= len; //Z Scale
*/
	m_cM.setToPosition(&m_cP, &m_cD, &m_cU);

	m_cD.normalize();
	m_cD.scale(len);
	tmpScaleMatrix.setToScale(&m_cD);

	m_cM.preMult(&tmpScaleMatrix);

	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 

}








////////////
// Static //
////////////
mXformStatic::mXformStatic(mVec3 *p, mVec3 *d, mVec3 *u)
{
	::mXform();
	m_cP.copy(p);
	m_cD.copy(d); 
	m_cU.copy(u); 
	update(); 
}

mXformStatic::~mXformStatic() {}

mXform *mXformStatic::clone()
{
	mXformStatic *out = new mXformStatic(&m_cP, &m_cD, &m_cU);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	return out;
}

void mXformStatic::update()
{
	m_cM.setToIdentity(); 
	m_cR.setToIdentity();

	m_cM.setToPosition(&m_cP, &m_cD, &m_cU);
	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}

////////////
// Spin X //
////////////
mXformSpinX::mXformSpinX(timeManager *tm, real s)
{
	::mXform();
	m_cTimeMgr = tm;
	speed = s;
}

mXformSpinX::~mXformSpinX() {}

mXform *mXformSpinX::clone()
{
	mXformSpinX *out = new mXformSpinX(m_cTimeMgr,speed);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	return out;
}

void mXformSpinX::update()
{
	evnTime time;

	m_cM.setToIdentity(); 
	m_cR.setToIdentity();


	m_cTimeMgr->getGameTime(&time);

	time *= speed;

	m_cM.setToRotation(time,'x');

	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}

////////////
// Spin Y //
////////////
mXformSpinY::mXformSpinY(timeManager *tm, real s)
{
	::mXform();
	m_cTimeMgr = tm;
	speed = s;
}

mXformSpinY::~mXformSpinY() {}

mXform *mXformSpinY::clone()
{
	mXformSpinY *out = new mXformSpinY(m_cTimeMgr,speed);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	return out;
}

void mXformSpinY::update()
{
	evnTime time;

	m_cM.setToIdentity(); 
	m_cR.setToIdentity();


	m_cTimeMgr->getGameTime(&time);

	time *= speed;

	m_cM.setToRotation(time,'y');

	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}

////////////
// Spin Z //
////////////
mXformSpinZ::mXformSpinZ(sndAnalysis *sm, const sndBeatName n,  real s)
{
	::mXform();
	m_cAnalysis = sm;
	m_iBeatName = n;
	speed = s;
}

mXformSpinZ::~mXformSpinZ() {}

mXform *mXformSpinZ::clone()
{
	mXformSpinZ *out = new mXformSpinZ(m_cAnalysis,m_iBeatName,speed);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	return out;
}

void mXformSpinZ::update()
{
	real	rot;

	m_cM.setToIdentity(); 
	m_cR.setToIdentity();

	m_cAnalysis->sine(m_iBeatName,&rot);

	rot *= speed;

	m_cM.setToRotation((rot*rot*rot*rot),'x');

	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}






///////////////////
// Sprite Static //
///////////////////


mXformSpriteStatic::mXformSpriteStatic(mVec3 *pos)
{
	::mXform();
	loc.copy(pos);
	update();
}

mXformSpriteStatic::~mXformSpriteStatic() {}

mXform *mXformSpriteStatic::clone()
{
	mXformSpriteStatic *out = new mXformSpriteStatic(&loc);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	return out;
}
void mXformSpriteStatic::update()
{
	m_cM.setToIdentity(); 
	m_cR.setToIdentity();

	m_cM.setToTranslation(&loc);

	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}

/////////////////////
// Sprite Bounce Y //
/////////////////////
mXformSpriteBounceY::mXformSpriteBounceY(sndAnalysis *sa, const sndBeatName n, int distance)
{
	::mXform();
	m_cAnalysis = sa;
	m_iBeatName = n;
	m_iDistance = distance;
}
	
mXformSpriteBounceY::~mXformSpriteBounceY() {}

mXform *mXformSpriteBounceY::clone()
{
	mXformSpriteBounceY *out = new mXformSpriteBounceY(m_cAnalysis, m_iBeatName, m_iDistance);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	return out;
}

void mXformSpriteBounceY::update()
{
	real	deltaY;		//time factor along distance
	mVec3 placement;	//where the sprite will end up

	m_cM.setToIdentity();
	m_cR.setToIdentity();
/*
	m_cAnalysis->sine(m_iBeatName,&deltaY);

	deltaY *= deltaY; //sin^2 (time)

	placement.x = 0.0f;
	placement.y = (real)m_iDistance * deltaY;
	placement.z = 0.0f;
*/
	m_cAnalysis->ramp(m_iBeatName,&deltaY);

	placement.x = 0.0f;
	placement.y = (real)m_iDistance * deltaY;
	placement.z = 0.0f;


	m_cM.setToTranslation(&placement);

	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}








/////////////////
// Sprite LERP //
/////////////////
mXformSpriteLERP::mXformSpriteLERP(const int xAnchor, const int yAnchor, mVec3 *line, real *value)
{
	::mXform();
	m_iXCenter = xAnchor;
	m_iYCenter = yAnchor;
	lerpLine.copy(line);
	m_pReal = value;
}
	
mXformSpriteLERP::~mXformSpriteLERP() {}

mXform *mXformSpriteLERP::clone()
{
	mXformSpriteLERP *out = new mXformSpriteLERP(m_iXCenter, m_iYCenter, &lerpLine, m_pReal);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	return out;
}

void mXformSpriteLERP::update()
{
	mVec3 placement;	//where the sprite will end up

	m_cM.setToIdentity(); 
	m_cR.setToIdentity();

	placement.zero();

	placement.x = (real)m_iXCenter + lerpLine.x * (*m_pReal);
	placement.y = (real)m_iYCenter + lerpLine.y * (*m_pReal);

	m_cM.setToTranslation(&placement);

	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 

}








/////////////////////////
// Sprite World-To-Map //
/////////////////////////
mXformSpriteWorldToMap::mXformSpriteWorldToMap(physManager *pm, const int particleIdx, int mapWidth, int mapHeight, mBBox2 *lvlExtents)
{
	::mXform();
	m_cPhysMgr = pm;
	m_iPartIdx = particleIdx;

	m_iMapWidth = mapWidth;
	m_iMapHeight = mapHeight;

	m_cLvlExtents.copy(lvlExtents);

	update();

}

mXformSpriteWorldToMap::~mXformSpriteWorldToMap() {}

mXform *mXformSpriteWorldToMap::clone()
{
	mXformSpriteWorldToMap *out = new mXformSpriteWorldToMap(m_cPhysMgr, m_iPartIdx, m_iMapWidth, m_iMapHeight, &m_cLvlExtents);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	return out;
}

void mXformSpriteWorldToMap::update()
{
	mVec3 pos, newPos;
	real xPer, zPer;

	// input-- the position of a particle in world space. 
	m_cPhysMgr->getElementPos(m_iPartIdx, &pos);

	xPer = (real)(pos.x - m_cLvlExtents.xlo) / (real)(m_cLvlExtents.xhi - m_cLvlExtents.xlo) ;
	zPer = (real)(pos.z - m_cLvlExtents.zlo) / (real)(m_cLvlExtents.zhi - m_cLvlExtents.zlo);

	newPos.x = (real)m_iMapWidth * xPer;
	newPos.y = (real)m_iMapHeight * zPer;
	newPos.z = 0.0f;

	m_cM.setToTranslation(&newPos);

	++m_iMVersion;
	m_cR.setToIdentity();

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}








////////////////////////////
// Sprite World-To-Screen //
////////////////////////////
mXformSpriteWorldToScreen::mXformSpriteWorldToScreen(grEnv *gr, physManager *pm, const int particleIdx)
{
	::mXform();
	m_cGrEnv = gr;
	m_cPhysMgr = pm;
	m_iPartIdx = particleIdx;
	update();

}

mXformSpriteWorldToScreen::~mXformSpriteWorldToScreen() {}

mXform *mXformSpriteWorldToScreen::clone()
{
	mXformSpriteWorldToScreen *out = new mXformSpriteWorldToScreen(m_cGrEnv, m_cPhysMgr, m_iPartIdx);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	return out;
}
void mXformSpriteWorldToScreen::update()
{
	const real zero = (real)0.0;
	const real half = (real)0.5; 
	const real big = (real)1000.0; 
	mVec3 pos, posEye, posPersp;
	mx4 view, persp;
	real scx, scy;

	// view.copy(&(m_cGrEnv->m_cW2E));
	// persp.copy(&(m_cGrEnv->m_cE2P));
	m_cGrEnv->getXF(GR_W2E, &view); 
	m_cGrEnv->getXF(GR_E2P, &persp); 
	
	scx = half * (m_cGrEnv->m_rScreenX); // the screen resolution 
	scy = half * (m_cGrEnv->m_rScreenY); 
	
	// input-- the position of a particle in world space. 
	m_cPhysMgr->getElementPos(m_iPartIdx, &pos);
	
	view.preMultSingle(&pos, &posEye); 
	persp.preMultSingle(&posEye, &posPersp); 

	m_cM.setToIdentity(); 
	if (posPersp.z<0.0f) { 
		m_cM.m_dM[3] = scx * big; // never, never see it. 
		m_cM.m_dM[7] = scy * big; // never, never see it.
	} else {
		// divide by z and add
		m_cM.m_dM[3] = ((posPersp.x/posPersp.z)*scx)+scx;
		m_cM.m_dM[7] = ((posPersp.y/posPersp.z)*-scy)+scy-12.0f;
	}

	++m_iMVersion;
	m_cR.setToIdentity();

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}





/////////////////
// Sprite slider //
/////////////////
mXformSpriteSlider::mXformSpriteSlider(int *idx, int x, int y, int dx, int dy) {
	::mXform();
	m_pIndex = idx; 
	m_ix = x; 
	m_iy = y;
	m_idx = dx;
	m_idy = dy; 
}
	
mXform *mXformSpriteSlider::clone() {
	mXformSpriteSlider *out = new mXformSpriteSlider(m_pIndex,m_ix,m_iy,m_idx,m_idy);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);
	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);
	return out;
}

void mXformSpriteSlider::update()
{
	int x, y;
	mVec3 placement;	//where the sprite will end up

	y = *m_pIndex;
	if (y==-1) { 
		x = -1000; y = -1000; 
	} else {
        x = m_ix + (y * m_idx); 
		y = m_iy + (y * m_idy); 
	}
	
	placement.set(x, y, 0);
	
	m_cM.setToTranslation(&placement);
	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL) {
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	} else {
		m_cR.setToIdentity(); 
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}









//////////////////////////////
// sprites periodic
//////////////////////////////




// Periodic Translate
mXformPTranslate::mXformPTranslate(mVec3 *p, mVec3 *v, xfPeriodic *prd) {
	m_cP.copy(p); 
	m_cV.copy(v); 
	m_cPrd.copy(prd);
	
}

mXform *mXformPTranslate::clone() {
	mXformPTranslate *out = new mXformPTranslate(&m_cP, &m_cV, &m_cPrd);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);
	
	return out;
}

void mXformPTranslate::update() {
	mVec3	pp, dp; 
	real	t;

	pp.copy(&m_cP); 
	dp.copy(&m_cV); 
	t = m_cPrd.eval(); 

	dp.scale(t); 
	pp.addEq(&dp); 

	m_cM.setToTranslation(&pp); 
	
	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	} else { 
		m_cR.setToIdentity(); //!!
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}

// Periodic Rotate
mXformPRotate::mXformPRotate(mVec3 *rot, mVec3 *v, xfPeriodic *prd) {
	m_cRot.copy(rot); 
	m_cV.copy(v); 
	m_cPrd.copy(prd);
	
}

mXform *mXformPRotate::clone() {
	mXformPRotate *out = new mXformPRotate(&m_cRot, &m_cV, &m_cPrd);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);
	
	return out;
}

void mXformPRotate::update() {
	mx4		X, Y, Z;
	mVec3	pp, dp; 
	real	t;

	pp.copy(&m_cRot); 
	dp.copy(&m_cV); 
	t = m_cPrd.eval(); 

	dp.scale(t); 
	pp.addEq(&dp); 
	
	m_cM.setToIdentity(); 	
	
	X.setToRotation(pp.x, 'X');
	Y.setToRotation(pp.y, 'Y');
	Z.setToRotation(pp.z, 'Z');
	
	m_cM.postMult(&X);
	m_cM.postMult(&Y);
	m_cM.postMult(&Z);
	
	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	} else { 
		m_cR.setToIdentity(); //!!
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}

// Periodic Scale
mXformPScale::mXformPScale(mVec3 *p, mVec3 *v, xfPeriodic *prd) {
	m_cP.copy(p); 
	m_cV.copy(v); 
	m_cPrd.copy(prd);
	
}

mXform *mXformPScale::clone() {
	mXformPScale *out = new mXformPScale(&m_cP, &m_cV, &m_cPrd);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);
	
	return out;
}

void mXformPScale::update() {
	mVec3	pp, dp; 
	real	t;

	pp.copy(&m_cP); 
	dp.copy(&m_cV); 
	t = m_cPrd.eval(); 

	dp.scale(t); 
	pp.addEq(&dp); 
	
	m_cM.setToScale(&pp); 	
	
	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	} else { 
		m_cR.setToIdentity(); //!!
	}

	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}

// Periodic MoveTo (LERP)
mXformPMoveTo::mXformPMoveTo(mVec3 *p1, mVec3 *p2, mVec3 *v, xfPeriodic *prd) {
	m_cP1.copy(p1); 
	m_cP2.copy(p2); 
	m_cV.copy(v); 
	m_cPrd.copy(prd);
	
}

mXform *mXformPMoveTo::clone() {
	mXformPMoveTo *out = new mXformPMoveTo(&m_cP1, &m_cP2, &m_cV, &m_cPrd);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);
	
	return out;
}

void mXformPMoveTo::update() {
	mVec3	pa, dp; 
	real	t;

	pa.copy(&m_cP1); 
	dp.eqSub(&m_cP2, &m_cP1); 
	t = m_cPrd.eval(); 

	dp.scale(t); 
	pa.addEq(&dp); 
	
	m_cM.setToTranslation(&pa); 	
	
	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	} else { 
		m_cR.setToIdentity(); //!!
	}
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}

/*
////////////////////////
// Rigid Body Scanner //
////////////////////////
mXformRigidBodyScanner::mXformRigidBodyScanner(physManager *pm, const int body)
{
	::mXform();
	m_iRigidBody = body; 
	m_cPhysMgr = pm; 
	m_cP.set(0.0f, 0.0f, 0.0f);
	m_cD.set(1.0f, 0.0f, 0.0f);
	m_cU.set(0.0f, 1.0f, 0.0f);
	update(); 
}

mXformRigidBodyScanner::~mXformRigidBodyScanner() {}

mXform *mXformRigidBodyScanner::clone()
{
	mXformRigidBodyScanner *out = new mXformRigidBodyScanner(m_cPhysMgr, m_iRigidBody);
	out->setParent(m_cParent);
	out->setMVersion(m_iMVersion);
	out->setRVersion(m_iRVersion);

	out->m_cM.copy(&m_cM);
	out->m_cR.copy(&m_cR);

	out->m_cP.copy(&m_cP);
	return out;
}

void mXformRigidBodyScanner::update()
{
	m_cM.setToIdentity(); 
	m_cR.setToIdentity();

	m_cPhysMgr->getElementPos(m_iRigidBody, &m_cP);
	translationMx.setToTranslation(&m_cP);
	m_cPhysMgr->getElementOrientationMatrix(m_iRigidBody, &m_cM);
	m_cM.postMult(&translationMx);

	++m_iMVersion;

	// get parent's xform and use it
	if (m_cParent!= NULL)
	{
		m_cParent->update();
		m_cR.copy(&(m_cParent->m_cR));
	} 
	m_cR.postMult(&m_cM);

	m_iRVersion = m_iMVersion; 
}
*/