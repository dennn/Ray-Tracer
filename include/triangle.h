#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "include/object.h"

class Triangle: public Object {
  	Vertex v0, v1, v2;

 	public:
  		Triangle(Vertex &pv0, Vertex &pv1, Vertex &pv2);
  		bool intersect(Ray &ray, Hit *h);
};

#endif
