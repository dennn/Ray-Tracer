#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "include/object.h"

class Sphere: public Object {
  	vec4 sp;
  	float  r;

 	public:
  		Sphere(vec4 &psp, float pr);
  		bool intersect(Ray &ray, Hit *h);
};

#endif
