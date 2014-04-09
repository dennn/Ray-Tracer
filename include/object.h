#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "libs/nv_math.h"
#include "libs/nv_mathdecl.h"
#include "libs/nv_algebra.h"
#include "include/colour.h"
#include "include/ray.h"
#include "include/hit.h"
#include "include/colour.h"
#include "include/material.h"

#define FLOAT_ZERO 0.000001f

class Object {
  	  Object *obj_next;
 	
 	public:
  		Material *obj_mat;
  		Object(void);
  		void link(Object *obj);
  		Object *next(void);
  		void setMaterial(Material *m);
  		Colour getColour();
  		virtual bool intersect(Ray &ray, Hit *h) { return false; }
  		virtual bool sintersect(Ray &ray, Hit *h, double tl);

  		mat4 inverseTransformation;
};

#endif
