/* Neal McDonald 2002
  */

#ifndef M_REAL
#define M_REAL 1

typedef double real;


void real_rand(real *x);
void in_rand(const int range, int *x);

real fract(real x); 



// #define M_PI		(real)3.141592653589793238462643383  
//  ?? already defined? 
#define M_TWOPI		(real)6.283185307179586476925286767
#define M_HALFPI	(real)1.570796326794896619231321691

#define M_ZERO		(real)0.0
#define M_ONE		(real)1.0
#define M_TWO		(real)2.0
#define M_THREE		(real)3.0
#define M_FOUR		(real)4.0
#define M_SIX		(real)6.0
#define M_TWENTY	(real)20.0

#define M_HALF		(real)0.5
#define M_QUARTER	(real)0.25
#define M_FIFTH		(real)0.2

#define M_EPSILON	(real)0.000001


#endif // M_REAL