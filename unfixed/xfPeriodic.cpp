/* (c) Greenhouse Software 2003

	Periodic Transform Stuff
*/

#include "xfPeriodic.h"

// Input Types
enum {
	PERDINPUT_CONST = 0,
	PERDINPUT_BEAT,
	PERDINPUT_TIME,

	PERDINPUT_LAST
};

// Constructor
xfPeriodic::xfPeriodic() {
	m_FnType		= PERDFUNC_SAWTOOTH;
	m_sInputType	= PERDINPUT_CONST;
	m_rInput		= 0.0;
	m_cAnalysis		= NULL;
	m_cTimeMgr		= NULL;
	
}

void xfPeriodic::copy(xfPeriodic *it) {
	m_FnType		= it->m_FnType;
	m_sInputType	= it->m_sInputType;
	m_rInput		= it->m_rInput;
	m_cAnalysis		= it->m_cAnalysis;
	m_cTimeMgr		= it->m_cTimeMgr;
}

// Constant init
void xfPeriodic::init(perdFuncType funcType, const real rInput) {
	m_FnType		= funcType;
	m_rInput		= rInput;
	m_sInputType	= PERDINPUT_CONST;
}

// Beat-based init
void xfPeriodic::init(perdFuncType funcType, sndAnalysis *cSA) {
	m_FnType		= funcType;
	m_cAnalysis		= cSA;
	m_sInputType	= PERDINPUT_BEAT;
}

// Time-based init
void xfPeriodic::init(perdFuncType funcType, timeManager *cTM) {
	m_FnType		= funcType;
	m_cTimeMgr		= cTM;
	m_sInputType	= PERDINPUT_TIME;
}

real xfPeriodic::eval(void) {
	real		rOutput = 0.0;

	switch  (m_sInputType) {
		case PERDINPUT_BEAT		:
			m_cAnalysis->sawtooth(SND_BEAT_BEST, &m_rInput);
		break;
		case PERDINPUT_TIME		:
			m_cTimeMgr->getGameTime(&m_rInput);
		break;
	}

	switch (m_FnType) {
		case PERDFUNC_SAWTOOTH	:
			rOutput = m_rInput;
		break;
		case PERDFUNC_SINE		:
			rOutput = (real)sin(6.283185308 * m_rInput);
		break;
		case PERDFUNC_SQUARE	:
			rOutput = (real)sin(6.283185308 * m_rInput);
			rOutput = rOutput * rOutput;
		break;
		case PERDFUNC_BUMP		:
			rOutput = (real)sin(6.283185308 * m_rInput);
			rOutput = rOutput * rOutput * rOutput * rOutput;
		break;
		case PERDFUNC_STEP		:
			if(m_rInput < 0.5)	rOutput = 1.0;
			else				rOutput = 0.0;
		break;
	}

	return (rOutput);
}