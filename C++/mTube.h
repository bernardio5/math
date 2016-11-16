/* Neal McDonald 2009
 
	phTubes are variable-width extrusions
	of circles around mLongPaths. 

	We use them to contain cars. 
	They are defined by a center mLongPath that sweeps an oval, 
	and a second mLongPath that provides the oval-- x is the width, 
	y is the height, and z is not used yet. 

	The tube defines a torus. Any 3d coordinate system
	can be transformed to the coordsystem defined by this torus.
	But let's be more specific: 

	Given p=(x,y,z) in world coords, we can map p to params=(t,theta,d), 
	where
		t gives the parameter of the point, centerp, on "center", closest to p,
		theta gives a point on the oval (shellp) at t s.t.  is on 
			the line connecting centerp and shellp,
		d gives the distance from shellp to p. d<0=>p is outside the mTube. 

	This transform is not representable by a mx4. sorry!

	So: p is a point being mapped.
	params is parametric coordinates of p
	centerp is the closest point on "center" to p
	shellp is the point on the tube on the line between p and p3.


	In addition, the tube has a variable radius. 
	This affects normals and shellpoints, but nothing else. 

	In addition, the tube twists in response to turns in the path-- banking!
	This affects the results of "map" calls. 
	These are stored as the third element of the "radiuses". 
	Twist is measured in radians from "right," ccw. 

	Finally, it is possible to flatten the bottom of the tube to make it more 
	like a racetrack. This also affects shellpoints and normals only. 


	/////// internals:
	To speed execution, mTube does not use an mLongPath, except at the beginning
	to denerate data. Instead, it makes many (8192) sample points, and interpolates
	between them. 
	Therefore, an mTube is a collection of segments. Each segment is a cylinder and defines
	a cylindircal coordinate system. Segment axes are connected at their endpoints. 
	Segments are bounded by planes that are at an angle to the axes of their segments. 
	The t parameter is used to select a segment and then gives way to a local t (0<=localt<1),
	that measures where a point is w.r.t. the two endcap planes. 

	Any point in space has a closest segment. There is a line || to the segment axis, 
	through the point, that hits both endcaps. These intersection points let us generate 
	a unique localt for each point for each segment. 

*/

#ifndef M_TUBE_CLASS
#define M_TUBE_CLASS


#include "mVec3.h"
#include "mLongPath.h"

// 8192 4096 2048 1024 512 256 128 64 32
// 8192 = 32 * 2^8
#define MTUBE_CACHE_SIZE 8192
// the number of segments in this mTube. 


class mTube {
public: 
	mVec3		m_cYHat;					// (0,1,0)
	real		m_rDt;						// 1.0f/MTUBE_CACHE_SIZE

	mVec3		centers[MTUBE_CACHE_SIZE];
	mVec3		dirs[MTUBE_CACHE_SIZE];		// dir[i]=dels[i].normalized
	mVec3		radiuses[MTUBE_CACHE_SIZE];	// horiz, vert and twist values
	real		delLens[MTUBE_CACHE_SIZE];  // centers[i].distanceFrom(centers[i+1], &delLens[i]); 
	mVec3		ns[MTUBE_CACHE_SIZE];		// endcap plane normals. 
											// ns[i] = 0.5(dirs[i-1]+dirs[i])
	real		flatAng[MTUBE_CACHE_SIZE];  // left and right edges of the flattened part of the mTube

	// convenience function for init(); sets radiuses[i].z
	void doBanking(mLongPath *center); 

	// find the index of the particle closest to p
	// golly! it's slow. Caaaareful!
	void searcher(mVec3 *p, int *result);

	// use the result of a previous sercher call to quickly
	// find the same thing that searcher finds. 
	// can fail, so only use if p doesn't move too much. 
	void updateSearcher(mVec3 *p, int *result, int span);
	// what's too much, you say? hah!

	void getDiskPoints(mVec3 *p, int section, mVec3 *p1, mVec3 *p2); 

	// given p, find "prev" and "next" s.t. the point on the tube
	// to which p is closest is between centers[prev] and centers[next].
	// Then set localt to specify that point. 
	void getPair(mVec3 *p, int ind, int *prev, int *next, real *localt);

	// given the results from getPair, lerp together a 
	//    center, dir, up, right, rs, theta12
	void getBasis(int prev, int next, real localt, mVec3 *center, 
					mVec3 *dir, mVec3 *up, mVec3 *right, mVec3 *rs, real *flat);

	// given the width og the dent, compute how much the tube is dented.
	void computeDent(real flat, real theta, real *scale);

	// mao and updateMap have a lot in common-- this. 
	void mapCompletion(mVec3 *p, mVec3 *params, int *section); 


	// helper for getPointNormal
	void getLocals(real t, real *localt, int *prev, int *next);

	// helper for getPointNormal. tweaks p's placement along the section's axis
	void sliderAdjuster(real localt, int prev, mVec3 *p); 
public:
	mTube() {	
		m_rDt = 1.0f / MTUBE_CACHE_SIZE; 
		m_cYHat.set(0,1,0); 
	}
	~mTube() {}
	void copy(mTube *it); 

	// provide 2 arrays of verts to init the 2 mLongPaths. 
	void init(const int datact, mVec3 *p, mVec3 *r);
	
	// given p, find params, shellp, and centerp as defined above. 
	// "section" is used by "updateMap" to provide a speedup. 
	void map(mVec3 *p, mVec3 *params, int *section); 

	// same as "map" but faster. use for p's whose values are 
	// changing incrementally. 
	void updateMap(mVec3 *p, mVec3 *params, int *section); 

	// opposite of map. given parameters, generate p. 
	// then get dir from lerp and generate n via cross-prod to compensate
	// for varying radii of the tube. 
	void getPointNormal(mVec3 *params, mVec3 *p, mVec3 *n, mVec3 *dir, mVec3 *shellp);
};


#endif // M_TUBE_CLASS
