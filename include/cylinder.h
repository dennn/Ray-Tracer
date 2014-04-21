#ifndef _CYLINDER_H_
#define _CYLINDER_H_

#include "include/object.h"

class Cylinder: public Object {
  	vec4 topCenter, vec4 bottomCenter;
  	float radius

 	public:
  		Cylinder(vec4 &topCenter, vec4 &bottomCenter, float radius);
  		bool intersect(Ray &ray, Hit *h);
};

#endif
