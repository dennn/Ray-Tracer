#ifndef _HIT_H_
#define _HIT_H_

#include "libs/nv_math.h"
#include "libs/nv_mathdecl.h"
#include "libs/nv_algebra.h"

class Object;

class Hit {
 	public:
  		vec4 p;
  		vec3 n;
  		double t;
  		Object *obj;
  		Hit(void);
};

#endif
