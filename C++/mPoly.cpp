
#include <memory.h>
#include "mPoly.h"

mPoly::mPoly() {
	maxTris=0;
	tris = new mTri[10];
	if (tris!=NULL) maxTris=10;
	numTris = 0;

	clips = new mSegment[10];
	if (clips!=NULL) maxTris=10;
	numClippedSegs = 0; 

	maxVerts = 10;
	vertices = new mVert[10];
	if (vertices!=NULL) maxVerts = 10;
	nvertices = 0;
	

	ready = false;
	if ((maxTris!=0) && (maxVerts!=0)) { 
		ready=true;
	}
}



mPoly::~mPoly() {
	if (maxVerts>0) {
//		delete [] vertices;
	}
	if (maxTris>0) {
//		delete [] tris;
//		delete [] clips;
	}
}



bool mPoly::init(int ct, real *xs, real *ys) {
	int i;
	if (ready) {

		ready = expandVerts(ct);
		if (ready) {
			for (i=0; i<ct; ++i) {
				vertices[i].v.set(xs[i], ys[i], 0.0); 
				vertices[i].ear = false; 
				vertices[i].vnum = i; 
				if (i>0) { 
					vertices[i].prev = &(vertices[i-1]);
				}			 
				if (i<ct-1) {
					vertices[i].next = &(vertices[i+1]); 
				}
			}
			vertices[0].prev = &(vertices[ct-1]);
			vertices[ct-1].next = &(vertices[0]);
		}
	}
	nvertices=ct;
	return ready;
}




bool mPoly::initFromStar(real cx, real cy, real ct, real *x, real *y) {
	return true; 
}



bool mPoly::expandTris(int a) {
	mTri *nts;
	mSegment *nss;

	if (maxTris>=a) return true;
	nts = new mTri[a];
	nss = new mSegment[a];
	if ((!nts)||(!nss)) {
		ready = false;
		return false;
	}
	memcpy(nts, tris, sizeof(mTri)*numTris);
//	memcpy(nss, clips, sizeof(mSegment)*numTris);
	delete [] tris;
	delete [] clips;
	tris = nts;
	clips = nss;
	maxTris = a;

	return true;
}




bool mPoly::addTri(mVert *a, mVert *b, mVert *c) {
	if (numTris>=maxTris) {
		if (!expandTris(maxTris*2)) {
			return false;
		}
	}
	tris[numTris].init(&(a->v), &(b->v), &(c->v));
	++numTris;
	return true;
}




bool mPoly::expandVerts(int a) {
	mVert *nvs;

	if (maxVerts>=a) return true;
	nvs = new mVert[a];
	if (!nvs) {
		ready = false;
		return false;
	}
	memcpy(nvs, vertices, sizeof(mVert)*maxVerts);
	delete [] vertices;
	vertices = nvs;
	maxVerts = a;
	return true;
}





bool mPoly::getNthTri(int which, mTri **res) {
	if (ready) {
		if (which<numTris) {
			*res = &(tris[which]);
			return true;
		}
	}
	return false;
}



// call only after triangulation
// 0->outside 1->on corner 2->on line 3->inside
char mPoly::inside(real x, real y) {
	int i, onCount;
	char tres, result, touch;

	result = 0;
	onCount = 0;
	touch = 0;
	i=0;
	if (ready) {
		while ((i<numTris)&&(result!=2)) {
			tres = tris[i].inside(x,y); 
			switch (tres) {
			case 0: break;		// outside
			case 1: touch=1; break;	// touching point
			case 2: ++onCount; break; // on line
			case 3: result=2; break; // inside
			}
			++i;
		}
		if (result==0) {
			switch (onCount) {
			case 0: break;
			case 1: result=1; break;
			default: result=2; break;
			}
		}
		if ((result==0) && (touch!=0)) {
			result = 1;
		}
	} else {
		result = 0;
	}
	return result;
}



// call only after triangulation
int mPoly::clip(mSegment *in) {
	int i;
	mSegment thisOut;

	numClippedSegs = 0; 
	for (i=0; i<numTris; ++i) {
		if (tris[i].clip(in, &(clips[numClippedSegs]))) {
			++numClippedSegs;
		}
	}
	return numClippedSegs;
}

bool mPoly::getNthClipped(int which, mSegment *out) { 
	bool result = false; 
	if (which<numClippedSegs) { 
		out->copy(&(clips[which])); 
		result = true; 
	}
	return result; 
}




/**************************************************************
 **************************************************************/


/*
This code is described in "Computational Geometry in C" (Second Edition),
Chapter 1.  It is not written to be comprehensible without the
explanation in that book.

Input: 2n integer coordinates for vertices of a simple polygon,
       in counterclockwise order.  NB: the code will not work
       for points in clockwise order!
Output: the diagonals of a triangulation, in PostScript.

Compile: gcc -o tri tri.c (or simply: make)

Written by Joseph O'Rourke, with contributions by Min Xu.
Last modified: October 1997
Questions to orourke@cs.smith.edu.
--------------------------------------------------------------------
This code is Copyright 1998 by Joseph O'Rourke.  It may be freely
redistributed in its entirety provided that this copyright notice is
not removed.
--------------------------------------------------------------------
*/
#include <stdio.h>
#include <math.h>


/*---------------------------------------------------------------------
Returns twice the signed area of the triangle determined by a,b,c.
The area is positive if a,b,c are oriented ccw, negative if cw,
and zero if the points are collinear.
---------------------------------------------------------------------*/
real mPoly::Area2(mVert *a, mVert *b, mVert *c) {
   return (((b->v.x)-(a->v.x))*((c->v.y)-(a->v.y)))-
   		  (((c->v.x)-(a->v.x))*((b->v.y)-(a->v.y)));
}



real mPoly::AreaPoly2() {
   real sum = 0;
   mVert *p, *a;

   p = vertices;   /* Fixed. */
   a = p->next;    /* Moving. */
   do {
      sum += Area2( p, a, a->next );
      a = a->next;
   } while ( a->next != vertices );
   return sum;
}

/*---------------------------------------------------------------------
Exclusive or: TRUE iff exactly one argument is true.
---------------------------------------------------------------------*/
bool mPoly::Xor( bool x, bool y ) { return !x ^ !y; }

/*---------------------------------------------------------------------
Returns true iff ab properly intersects cd: they share
a point interior to both segments.  The properness of the
intersection is ensured by using strict leftness.
---------------------------------------------------------------------*/
bool mPoly::IntersectProp(mVert *a, mVert *b, mVert *c, mVert *d) {
	/* Eliminate improper cases. */
	if (Collinear(a,b,c) || Collinear(a,b,d) ||
		Collinear(c,d,a) || Collinear(c,d,b) ) {
		return false;
	}

	return Xor(Left(a,b,c),Left(a,b,d)) &&
			Xor(Left(c,d,a),Left(c,d,b));
}



// uncommented. relies on integer assumptions. problems?
int mPoly::AreaSign(mVert *a, mVert *b, mVert *c) {
    real area2;
    area2 = (((b->v.x)-(a->v.x))*((c->v.y)-(a->v.y))) -
    		(((c->v.x)-(a->v.x))*((b->v.y)-(a->v.y)));

    /* The area should be an integer. */
    if      ( real_gt(area2, 0.0) ) return  1;
    else if ( real_lt(area2, 0.0) ) return -1;
    else                     return  0;
}



/*---------------------------------------------------------------------
Returns true iff c is strictly to the left of the directed
line through a to b.
---------------------------------------------------------------------*/
bool mPoly::Left(mVert *a, mVert *b, mVert *c) {
   return  AreaSign(a,b,c) > 0;
}

bool mPoly::LeftOn(mVert *a, mVert *b, mVert *c) {
   return  AreaSign(a,b,c) >= 0;
}

bool mPoly::Collinear(mVert *a, mVert *b, mVert *c) {
   return  AreaSign(a,b,c) == 0;
}

/*---------------------------------------------------------------------
Returns TRUE iff point c lies on the closed segement ab.
First checks that c is collinear with a and b.
---------------------------------------------------------------------*/
bool mPoly::Between(mVert *a, mVert *b, mVert *c)
{
	// probably should be replaced with a point->segment
	// distance check.
   if ( ! Collinear(a,b,c) )
      return false;

   /* If ab not vertical, check betweenness on x; else on y. */
   if (!real_eq((a->v.x),(b->v.x)))
      return (((a->v.x)<=(c->v.x)) && ((c->v.x)<=(b->v.x))) ||
             (((a->v.x)>=(c->v.x)) && ((c->v.x)>=(b->v.x)));
   else
      return (((a->v.y)<=(c->v.y)) && ((c->v.y)<=(b->v.y))) ||
             (((a->v.y)>=(c->v.y)) && ((c->v.y)>=(b->v.y)));
}

/*---------------------------------------------------------------------
Returns TRUE iff segments ab and cd intersect, properly or improperly.
---------------------------------------------------------------------*/
bool mPoly::Intersect(mVert *a, mVert *b, mVert *c, mVert *d)
{
   if (IntersectProp(a, b, c, d))
      return true;
   else if (   Between( a, b, c )
            || Between( a, b, d )
            || Between( c, d, a )
            || Between( c, d, b )
           )
      return true;
   else    return  false;
}

/*---------------------------------------------------------------------
Returns TRUE iff (a,b) is a proper internal *or* external
diagonal of P, *ignoring edges incident to a and b*.
---------------------------------------------------------------------*/
bool mPoly::Diagonalie(mVert *a, mVert *b) {
   mVert *c, *c1;

   /* For each edge (c,c1) of P */
   c = vertices;
   do {
      c1 = c->next;
      /* Skip edges incident to a or b */
      if ((c!=a)&&(c1!=a)&&(c!=b)&&(c1!=b)
      	&& Intersect(a,b,c,c1))
         return false;
      c = c->next;
   } while (c!=vertices);
   return true;
}



/*---------------------------------------------------------------------
This function initializes the data structures, and calls
Triangulate2 to clip off the ears one by one.
---------------------------------------------------------------------*/
void mPoly::EarInit( void ) {
   mVert *v0, *v1, *v2;   /* three consecutive vertices */
   /* Initialize v1->ear for all vertices. */
   v1 = vertices;
   do {
      v2 = v1->next;
      v0 = v1->prev;
      v1->ear = Diagonal( v0, v2 );
      v1 = v1->next;
   } while (v1!=vertices);
}


/*---------------------------------------------------------------------
Prints out n-3 diagonals (as pairs of integer indices)
which form a triangulation of P.
---------------------------------------------------------------------*/

bool mPoly::Triangulate() {
   mVert *v0, *v1, *v2, *v3, *v4;	/* five consecutive vertices */
   int   n = nvertices;		/* number of vertices; shrinks to 3. */
   bool earfound;		/* for debugging and error detection only. */

	numTris=0;
   EarInit();
   /* Each step of outer loop removes one ear. */
   while ( n > 3 ) {
      /* Inner loop searches for an ear. */
      v2 = vertices;
      earfound = false;
      do {
         if (v2->ear) {
            earfound = true;
            /* Ear found. Fill variables. */
            v3 = v2->next; v4 = v3->next;
            v1 = v2->prev; v0 = v1->prev;

			// save v1, v2, v3 as a triangle
			addTri(v1, v2, v3);

            /* Update earity of diagonal endpoints */
            v1->ear = Diagonal( v0, v3 );
            v3->ear = Diagonal( v1, v4 );

            /* Cut off the ear v2 */
            v1->next = v3;
            v3->prev = v1;
            vertices = v3;	/* In case the head was v2. */
            n--;
            break;   /* out of inner loop; resume outer loop */
         } /* end if ear found */
         v2 = v2->next;
      } while ( v2 != vertices );

      if ( !earfound ) {
		return false; 
	  }
   } /* end outer while loop */
   // the vert list now contains a triangle
   addTri(vertices, vertices->next, vertices->prev); 
	return true;
}


/*---------------------------------------------------------------------
Returns TRUE iff the diagonal (a,b) is strictly internal to the
polygon in the neighborhood of the a endpoint.
---------------------------------------------------------------------*/
bool mPoly::InCone(mVert *a, mVert *b) {
   mVert *a0,*a1;	/* a0,a,a1 are consecutive vertices. */

   a1 = a->next;
   a0 = a->prev;

   /* If a is a convex vertex ... */
   	if( LeftOn(a, a1, a0 ) ) return Left(a,b,a0) && Left(b,a,a1);

  	/* Else a is reflex: */
	return !(LeftOn(a,b,a1) && LeftOn(b,a,a0));
}

/*---------------------------------------------------------------------
Returns TRUE iff (a,b) is a proper internal diagonal.
---------------------------------------------------------------------*/
bool mPoly::Diagonal(mVert *a, mVert *b) {
	return InCone(a,b) && InCone(b,a) && Diagonalie(a,b);
}


