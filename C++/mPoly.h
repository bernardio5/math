// Neal McDonald 2009

#ifndef M_POLY_CLASS
#define M_POLY_CLASS

#include "math/mTri.h"
#include "math/mVert.h"

/* a polygon is a path of vertices.
	This class will take 2d polygons,
	and is able to give triangularizations,
	and that's all.

	Area, inside tests, area signs, and everything else
	are implemented as a sum of operations on triangles.

	This is not a generic polygon class.
	There's no such thing.
	*/



class mPoly {

public:
	mPoly();
	~mPoly();

	// return false for degenerates
	bool init(int ct, real *x, real *y);

	// returns the number of triangles
	bool	Triangulate();

	// star-shaped polygons are already triangulated!
	bool initFromStar(real cx, real cy, real ct, real *x, real *y); 

	real	AreaPoly2();


	//////////// call only after good triangulation
	bool getNthTri(int which, mTri **res);

	// returns: 0->no, 1->on edge, 2->properly
	char inside(real x, real y);

	// "clip" saves segments in "clips", returns #segments
	int clip(mSegment *in);
	bool getNthClipped(int which, mSegment *out);

	// check after init
	bool ready;



private:
	int maxVerts, nvertices;	
	mVert *vertices; 
	int numTris, maxTris, currentTri;
	mTri *tris;
	mSegment *clips; 
	int numClippedSegs;

	bool 	expandTris(int a);
	bool 	addTri(mVert *a, mVert *b, mVert *c);
	bool 	expandVerts(int a);

	real 	Area2(mVert *a, mVert *b, mVert *c);
	bool 	Xor( bool x, bool y );
	bool 	IntersectProp(mVert *a, mVert *b, mVert *c, mVert *d);
	int 	AreaSign(mVert *a, mVert *b, mVert *c);
	bool 	Left(mVert *a, mVert *b, mVert *c);
	bool	LeftOn(mVert *a, mVert *b, mVert *c);
	bool	Collinear(mVert *a, mVert *b, mVert *c);
	bool	Between(mVert *a, mVert *b, mVert *c);
	bool	Intersect(mVert *a, mVert *b, mVert *c, mVert *d);
	bool	Diagonalie(mVert *a, mVert *b);
	bool	InCone(mVert *a, mVert *b);
	bool	Diagonal(mVert *a, mVert *b);


	void	EarInit();

	/* Global variable definitions */
};


#endif


// Neal McDonald 1/1/2005
