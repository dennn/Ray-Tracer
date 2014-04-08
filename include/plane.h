#ifndef _PLANE_H_
#define _PLANE_H_

#include "include/object.h"

class Plane: public Object {
  	vec4 normal;
  	float distance;

 	public:
  		Plane(vec4 &pNormal, float pDistance);
  		bool intersect(Ray &ray, Hit *h);
};

#endif
