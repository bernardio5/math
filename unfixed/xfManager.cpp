


/* (c) Greenhouse Software 2002
	All rights reserved. 
	See xfManager.h for more info. 
	*/




#include "xfManager.h"

#include "evnManager.h"

#include "profiler.h"

Define_Zone(xfMgr_update);

xfManager::xfManager(errManager *erm, 
					   fileManager *fim) {
	int i; 
	m_iMyType = MGR_GR;
	m_cErrMgr = erm;
	m_cFileMgr = fim;

	m_iNextIndex = 0; 
	m_bFirstPass = true; 

	for (i=0; i<XF_MAX_XFS; ++i) {
		m_cAll[i] = NULL;
		m_iReferenceCounters[i] = -1; 
		m_iParents[i] = -1; 
	}

	chill(); 
}



void xfManager::clear() { 
	int i; 
	for (i=0; i<XF_MAX_XFS; ++i) {
		if (m_iReferenceCounters[i]>-1) {
			delete m_cAll[i];
			m_cAll[i] = NULL;
			m_iReferenceCounters[i] = -1; 
			m_iParents[i] = -1; 
		}
	}
	m_iNextIndex = 0; 
	m_bFirstPass = true; 

}


xfManager::~xfManager() { 
	clear(); 
}





void xfManager::setAAboveB(const xfName a, const xfName b) {
	if ((a<0) || (a>=XF_MAX_XFS)) {
		//modalPop("xfMgr: index out of range");
		return;
	}
	if (m_iReferenceCounters[a]==-1) {
		//modalPop("xfMgr: index of unused mxform");
		return;
	}

	if ((b<0) || (b>=XF_MAX_XFS)) {
		//modalPop("xfMgr: index out of range");
		return;
	}
	if (m_iReferenceCounters[b]==-1) {
		//modalPop("xfMgr: index of unused mxform");
		return;
	}

	++m_iReferenceCounters[a]; 
	m_iParents[b] = a;
	m_cAll[b]->setParent(m_cAll[a]);

}


void xfManager::getIndex(int *r) {
	int i;
	*r = -1; i = m_iNextIndex;
	while ((i<XF_MAX_XFS) && (*r==-1)) {
		if (m_iReferenceCounters[i]==-1) {
			*r = i;
		}	
		++i;
	}
	if (*r==-1) { 
		i = 0;
		while ((i<XF_MAX_XFS) && (*r==-1)) {
			if (m_iReferenceCounters[i]==-1) {
				*r = i;
			}	
			++i;
		}
		if (*r==-1) {
			modalPop("xfMgr::no available xforms"); 
		}
	}
}

void xfManager::addXF(mXform *it, int *index) {
	getIndex(index); 
	if (*index!=-1) {
		m_cAll[*index] = it->clone();
		m_iReferenceCounters[*index] = 0; 
	}
}


void xfManager::makeBillboard(int *result, entManager *em, mVec3 *pos)
{
	getIndex(result);
	if (*result!=-1)
	{
		m_cAll[*result] = new mXformBillboard(em,pos);
		m_iReferenceCounters[*result] = 0; 
	}
}

void xfManager::makeDynamicStrut(int *result, physManager *p, const int p1, const int p2)
{
	getIndex(result);
	if (*result!=-1)
	{
		m_cAll[*result] = new mXformDynamicStrut(p, p1, p2);
		m_iReferenceCounters[*result] = 0; 
	}
}

void xfManager::makeInverseLorentz(int *result, physManager *p, const int p1)
{
	getIndex(result);
	if (*result!=-1)
	{
		m_cAll[*result] = new mXformInverseLorentz(p, p1);
		m_iReferenceCounters[*result] = 0; 
	}
}
/*
void xfManager::makeRigidBodyScanner(int *result, physManager *p, const int body)
{
	getIndex(result); 
	if (*result!=-1)
	{
		m_cAll[*result] = new mXformRigidBodyScanner(p, body);
		m_iReferenceCounters[*result] = 0; 
	}
}
*/
void xfManager::makeScaler(int *result, mVec3 *scale)
{
	getIndex(result); 
	if (*result!=-1)
	{
		m_cAll[*result] = new mXformScaler(scale);
		m_iReferenceCounters[*result] = 0; 
	}
}

void xfManager::makeScanner1(int *result, physManager *p, const int p1)
{
	getIndex(result); 
	if (*result!=-1)
	{
		m_cAll[*result] = new mXformScanner1(p, p1);
		m_iReferenceCounters[*result] = 0; 
	}
}
void xfManager::makeScanner2(int *result, physManager *p, const int p1, const int p2) {
	getIndex(result); 
	if (*result!=-1) {
		m_cAll[*result] = new mXformScanner2(p, p1, p2);
		m_iReferenceCounters[*result] = 0; 
	}
}
void xfManager::makeScanner3(int *result, physManager *p, const int p1, const int p2, const int p3) {
	getIndex(result); 
	if (*result!=-1) {
		m_cAll[*result] = new mXformScanner3(p, p1, p2, p3);
		m_iReferenceCounters[*result] = 0; 
	}
}

void xfManager::makeStatic(int *result, mVec3 *p, mVec3 *d, mVec3 *u) {
	getIndex(result); 
	if (*result!=-1) {
		m_cAll[*result] = new mXformStatic(p, d, u);
		m_iReferenceCounters[*result] = 0; 
	}
}
void xfManager::makeStaticStrut(int *result, mVec3 *p, mVec3 *p2)
{
	getIndex(result); 
	if (*result!=-1) {
		m_cAll[*result] = new mXformStaticStrut(p, p2);
		m_iReferenceCounters[*result] = 0; 
	}
}


void xfManager::makeTubeMover(int *result, mTube *tube, mVec3 *params, 
							  mVec3 *speed, timeManager *tm)
{
	getIndex(result); 
	if (*result!=-1) {
		m_cAll[*result] = new mXformTubeMover(tube, params, speed, tm);
		m_iReferenceCounters[*result] = 0; 
	}
}

void xfManager::makeTubeMoverTest(int *result, mTube *tube, mVec3 *params, 
							  mVec3 *speed, timeManager *tm)
{
	getIndex(result); 
	if (*result!=-1) {
		m_cAll[*result] = new mXformTubeMoverTest(tube, params, speed, tm);
		m_iReferenceCounters[*result] = 0; 
	}
}

void xfManager::makeSpinX(int *result, timeManager *tm, real s) {
	getIndex(result); 
	if (*result!=-1) {
		m_cAll[*result] = new mXformSpinX(tm, s);
		m_iReferenceCounters[*result] = 0; 
	}
}
void xfManager::makeSpinY(int *result, timeManager *tm, real s) {
	getIndex(result); 
	if (*result!=-1) {
		m_cAll[*result] = new mXformSpinY(tm, s);
		m_iReferenceCounters[*result] = 0; 
	}
}
void xfManager::makeSpinZ(int *result, sndAnalysis *sm, const sndBeatName n,  real s) {
	getIndex(result); 
	if (*result!=-1) {
		m_cAll[*result] = new mXformSpinZ(sm, n, s);
		m_iReferenceCounters[*result] = 0; 
	}
}

void xfManager::makeSpriteStatic(int *result, mVec3 *pos)
{
	getIndex(result);
	if(*result!=-1)
	{
		m_cAll[*result] = new mXformSpriteStatic(pos);
		m_iReferenceCounters[*result]=0;
	}
}

void xfManager::makeSpriteBounceY(int *result, sndAnalysis *sm, const sndBeatName n,  int distance)
{
	getIndex(result);
	if(*result!=-1)
	{
		m_cAll[*result] = new mXformSpriteBounceY(sm, n, distance);
		m_iReferenceCounters[*result]=0;
	}
}

void xfManager::makeSpriteWorldToMap(int *result, physManager *pm, const int particleIdx, int mapWidth, int mapHeight, mBBox2 *lvlExtents)
{
	getIndex(result);
	if(*result!=-1)
	{
		m_cAll[*result] = new mXformSpriteWorldToMap(pm, particleIdx, mapWidth, mapHeight, lvlExtents);
		m_iReferenceCounters[*result]=0;
	}
}

void xfManager::makeSpriteWorldToScreen(int *result, grEnv *gr, physManager *pm, const int particleIdx)
{
	getIndex(result);
	if(*result!=-1)
	{
		m_cAll[*result] = new mXformSpriteWorldToScreen(gr, pm, particleIdx);
		m_iReferenceCounters[*result]=0;
	}
}


void xfManager::makeSpriteSlider(int *result, int *place, int x, int y, int dx, int dy)
{
	getIndex(result);
	if(*result!=-1)
	{
		m_cAll[*result] = new mXformSpriteSlider(place, x, y, dx, dy);
		m_iReferenceCounters[*result]=0;
	}
}

void xfManager::makePTranslate(int *result, mVec3 *p, mVec3 *v, xfPeriodic *prd)
{
	getIndex(result);
	if(*result!=-1)
	{
		m_cAll[*result] = new mXformPTranslate(p, v, prd);
		m_iReferenceCounters[*result]=0;
	}
}

void xfManager::makePRotate(int *result, mVec3 *rot, mVec3 *v, xfPeriodic *prd)
{
	getIndex(result);
	if(*result!=-1)
	{
		m_cAll[*result] = new mXformPRotate(rot, v, prd);
		m_iReferenceCounters[*result]=0;
	}
}

void xfManager::makePScale(int *result, mVec3 *p, mVec3 *v, xfPeriodic *prd)
{
	getIndex(result);
	if(*result!=-1)
	{
		m_cAll[*result] = new mXformPScale(p, v, prd);
		m_iReferenceCounters[*result]=0;
	}
}

void xfManager::makePMoveTo(int *result, mVec3 *p1, mVec3 *p2, mVec3 *v, xfPeriodic *prd)
{
	getIndex(result);
	if(*result!=-1)
	{
		m_cAll[*result] = new mXformPMoveTo(p1, p2, v, prd);
		m_iReferenceCounters[*result]=0;
	}
}




void xfManager::addReference(const int index) {
	if ((index<0) || (index>=XF_MAX_XFS)) {
		modalPop("xfMgr: index out of range");
		return;
	}
	if (m_iReferenceCounters[index]==-1) {
		modalPop("xfMgr: index of unused mxform");
		return;
	}
	++m_iReferenceCounters[index];
} 
	


void xfManager::loseReference(const int index) {
	if ((index<0) || (index>=XF_MAX_XFS)) {
		//modalPop("xfMgr: index out of range");
		return;
	}
	if (m_iReferenceCounters[index]==-1) {
		//modalPop("xfMgr: index of unused mxform");
		return;
	}
	--m_iReferenceCounters[index];

	if (m_iReferenceCounters[index]==0) {
		if (m_iParents[index]!=-1) {
			loseReference(m_iParents[index]); 
		}
		if (m_cAll[index] != NULL) { delete m_cAll[index]; }
		m_cAll[index] = NULL;
		m_iReferenceCounters[index] = -1;
	}
}
	


	
void xfManager::update(const int index) {
	Profile_Scope(xfMgr_update);

	if ((index<0) || (index>=XF_MAX_XFS)) {
		//modalPop("xfMgr: index out of range");
		return; 
	}
	if (m_iReferenceCounters[index]<1) {
		//modalPop("xfMgr: index of unused mxform");
		return; 
	}
	m_cAll[index]->update(); 
} 





void xfManager::getPosition(const int index, mVec3 *p) {
	mx4 *r; 
	if ((index<0) || (index>=XF_MAX_XFS)) {
		//modalPop("xfMgr: index out of range");
		p->zero(); 
		return;
	}
	if (m_iReferenceCounters[index]<1) {
		//modalPop("xfMgr: index of unused mxform");
		p->zero(); 
		return;
	}

	// m_cR contains a obj2world xf. 
	// we want world2obj. 
	r = &(m_cAll[index]->m_cR); 

	// TODO: Neal, quit violating mx4's data hiding. 
	// Neal Says: "data what?" You want to write a mx4 inverter?
	p->x = (r->m_dM[3]); 
	p->y = (r->m_dM[7]); 
	p->z = (r->m_dM[11]); 	

	// this is not better, but correct. 
	// r->preMult(p); p->negate(); 
}




void xfManager::getData(const int index, mx4 *result) {
	if ((index<0) || (index>=XF_MAX_XFS)) {
		//modalPop("xfMgr: index out of range");
		result->setToIdentity();
		return; 
	}
	if (m_iReferenceCounters[index]<1) {
		//modalPop("xfMgr: index of unused mxform");
		result->setToIdentity();
		return; 
	}
	result->copy(&(m_cAll[index]->m_cR)); 
}


