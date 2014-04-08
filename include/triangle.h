#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "include/object.h"

class Triangle: public Object {
  	vec4 v0, v1, v2;

 	public:
  		Triangle(vec4 &pv0, vec4 &pv1, vec4 &pv2);
  		bool intersect(Ray &ray, Hit *h);
};

#endif
