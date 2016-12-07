/* (c) Greenhouse Software 2003

	Periodic Transform Stuff
*/

#ifndef XFPERIODIC_CLASS
#define XFPERIODIC_CLASS

#include "timeManager.h"
#include "sndManager.h" 

typedef enum {
	PERDFUNC_ERROR = -1,
	PERDFUNC_SAWTOOTH = 0,
	PERDFUNC_SINE,
	PERDFUNC_SQUARE,
	PERDFUNC_BUMP,
	PERDFUNC_STEP,

	PERD_FUNC_LAST
} perdFuncType;
	

class xfPeriodic {
private :
	perdFuncType	m_FnType;
	short			m_sInputType;
	real			m_rInput;
	sndAnalysis		*m_cAnalysis;
	timeManager		*m_cTimeMgr;
public :
	xfPeriodic();
	~xfPeriodic(){};
	void copy(xfPeriodic *);
	void init(perdFuncType, const real);		// Will set up a constant period
	void init(perdFuncType, sndAnalysis *);		// Will set up a beat-based period
	void init(perdFuncType, timeManager *);		// Will set up a time-based period
	real eval();
};

#endif // XFPERIODIC_CLASS