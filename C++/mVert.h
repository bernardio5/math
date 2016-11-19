// Neal McDonald 2009



#ifndef M_VERT_TYPE
#define M_VERT_TYPE

#include "mVec3.h"

// used by mPoly for circular vertex lists
// anyone else? no?

class mVert {
public:
	mVec3 	v;
	int		vnum;		/* Index */
	bool 	ear;		/* TRUE iff an ear */
	mVert 	*next,*prev;

	mVert() {}
	~mVert() {}
};

#endif

// Neal McDonald 1/1/2005

