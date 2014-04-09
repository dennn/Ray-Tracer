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
		mat4 currentMatrix;


 	public:
 		TransformStack();
 		void pushMatrix();
 		void popMatrix();

 		mat4 copyCurrentMatrix();

 		void setIdentityMatrix();

 		void applyScaleTransform(vec3 scale);
 		void applyTranslateTransform(vec3 translate);
 		void applyRotateTransform(vec3 rotate, float angles_deg);
};

#endif
