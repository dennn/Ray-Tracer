#ifndef _TRANSFORMSTACK_H_
#define _TRANSFORMSTACK_H_

#include "libs/nv_math.h"
#include "libs/nv_mathdecl.h"
#include "libs/nv_algebra.h"

#include <vector>

class TransformStack {
	private: 
		std::vector<mat4> stack;
		mat4 getIdentityMatrix();

 	public:
 		TransformStack();

 		mat4 currentMatrix;

 		void pushMatrix();
 		void popMatrix();

 		void setIdentityMatrix();

 		void applyScaleTransform(vec3 scale);
 		void applyTranslateTransform(vec3 translate);
};

#endif
