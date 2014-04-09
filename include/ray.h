#ifndef _RAY_H_
#define _RAY_H_

#include "libs/nv_math.h"
#include "libs/nv_mathdecl.h"
#include "libs/nv_algebra.h"

class Object;

class Ray {
 	public:
  		vec4 P;
  		vec3 D;
  		vec4 position(double t);

  		vec3 getDirection();
  		vec4 getPosition();

  		Ray inverseTransformOfRay(Object *obj);
};

#endif
