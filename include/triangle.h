#ifndef _TRIANGLE_H
#define _TRIANGLE_H_

#include "include/object.h"

class Triangle: public Object {
  	Vertex sp;
  	float  r;

 	public:
  		Sphere(Vertex &psp, float pr);
  		bool intersect(Ray &ray, Hit *h);
};

#endif
