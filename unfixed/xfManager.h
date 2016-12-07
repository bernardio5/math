


/* (c) Greenhouse Software 2002
	Excuse: 
		Transforms need to be stackable, need to point to each other a lot, 
		need to be evaluated some but not always, need to be available, 
		need to know when it's safe to be deleted.

		xfMgr is a latecomer and I'm kind of mad at it. 

		first, you add a transform. This copies the xf in and sets its refCtr to 0.
		then, you add a reference to it. 
			pupMgr automatically adds references to all the xf's you give it. 

		Puppets and transforms can be the children of transforms. 
		"ReferenceCounters" tracks how many children each transform has. 
		If your counter drops to 0, delete yourself. 
		If your counter is -1, you're deleted. 


	*/



#ifndef XF_MANAGER_TYPE
#define XF_MANAGER_TYPE

////////////////////////////////

#include "baseManager.h"

#include "mx4.h" 
#include "mXform.h" 
#include "mTube.h"

#include "errManager.h"
#include "fileManager.h"
#include "timeManager.h"

#include "physManager.h"
#include "xfPeriodic.h"
////////////////////////////////

#define XF_MAX_XFS			1000

typedef int xfName; 

class xfManager : public baseManager {
	fileManager			*m_cFileMgr;

	mXform				*m_cAll[XF_MAX_XFS]; 
	short int			m_iReferenceCounters[XF_MAX_XFS]; 
	short int			m_iParents[XF_MAX_XFS]; 
	int					m_iNextIndex; 
	bool				m_bFirstPass;

	void getIndex(int *result); 
public:
	xfManager(errManager *erm, 
				fileManager *fim); 
	void clear(); 
	~xfManager(); 

	////////////// functions for making parrticular types of mXforms
	void makeBillboard(int *result, entManager *em, mVec3 *pos);
	void makeDynamicStrut(int *result, physManager *p, const int p1, const int p2);
	void makeInverseLorentz(int *result, physManager *p, const int p1);
//	void makeRigidBodyScanner(int *result, physManager *p, const int body);
	void makeScaler(int *result, mVec3 *scale);
	void makeScanner1(int *result, physManager *p, const int p1);
	void makeScanner2(int *result, physManager *p, const int p1, const int p2); 
	void makeScanner3(int *result, physManager *p, const int p1, const int p2, const int p3);
	void makeStatic(int *result, mVec3 *p, mVec3 *d, mVec3 *u); 
	void makeStaticStrut(int *result, mVec3 *p, mVec3 *p2);
	void makeTubeMover(int *result, mTube *tube, mVec3 *params, mVec3 *dparams, timeManager *tm);
	void makeTubeMoverTest(int *result, mTube *tube, mVec3 *params, mVec3 *dparams, timeManager *tm);
	void makeSpinX(int *result, timeManager *tm, real s); 
	void makeSpinY(int *result, timeManager *tm, real s); 
	void makeSpinZ(int *result, sndAnalysis *sm, const sndBeatName n, real s); 
	void makeSpriteStatic(int *result, mVec3 *pos);
	void makeSpriteBounceY(int *result, sndAnalysis *sm, const sndBeatName n, int distance);
	void makeSpriteWorldToMap(int *result, physManager *pm, const int particleIdx, int mapWidth, int mapHeight, mBBox2 *lvlExtents);
	void makeSpriteWorldToScreen(int *result, grEnv *gr, physManager *pm, const int particleIdx);
	void makeSpriteSlider(int *result, int *place, int x, int y, int dx, int dy);
	void makePTranslate(int *result, mVec3 *p, mVec3 *v, xfPeriodic *prd);
	void makePRotate(int *result, mVec3 *rot, mVec3 *v, xfPeriodic *prd);
	void makePScale(int *result, mVec3 *p, mVec3 *v, xfPeriodic *prd);
	void makePMoveTo(int *result, mVec3 *p1, mVec3 *p2, mVec3 *v, xfPeriodic *prd);
	
	// set a to be b's parent. Add a reference to a
	void setAAboveB(const xfName a, const xfName b); 

	// returns an integer to be used in all the other function calls. 
	void addXF(mXform *it, xfName *index); 

	// increments the xf's reference counter
	void addReference(const xfName index); 

	// increments the xf's reference counter; when counter <1, deletes the xf.
	void loseReference(const xfName index); 

	// set which material to use as a puppet's default
	void update(const xfName index); 

	void getPosition(const xfName index, mVec3 *p); 

	void getData(const xfName index, mx4 *result); 
};




#endif  //#ifdef XF_MANAGER_TYPE
