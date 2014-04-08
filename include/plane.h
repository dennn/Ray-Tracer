#ifndef _PLANE_H_
#define _PLANE_H_

#include "include/object.h"

class Plane: public Object {
  	Vertex normal;
  	float distance;

 	public:
  		Plane(Vertex &pNormal, float pDistance);
  		bool intersect(Ray &ray, Hit *h);
};

#endif
