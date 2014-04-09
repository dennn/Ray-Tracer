#include "include/transformStack.h"

TransformStack::TransformStack()
{
	setIdentityMatrix();
}

void TransformStack::setIdentityMatrix()
{
	currentMatrix = getIdentityMatrix();
}

mat4 TransformStack::getIdentityMatrix()
{
	mat4 identity = mat4(1, 0, 0, 0, 
						 0, 1, 0, 0, 
						 0, 0, 1, 0, 
						 0, 0, 0, 1);
	return identity;
}

mat4 TransformStack::copyCurrentMatrix()
{
	mat4 matrixCopy = currentMatrix;

	return matrixCopy;
}

// Stack functions
void TransformStack::pushMatrix()
{
	stack.push_back(currentMatrix);
}

void TransformStack::popMatrix()
{
	stack.pop_back();
	currentMatrix = stack.back();
}

// Transformations - Taken from Ray Tracing From the Ground Up


void TransformStack::applyScaleTransform(vec3 scale) 
{
	mat4 scaleMatrix = mat4(scale.x, 0, 		0,  	 0,
							0, 		 scale.y,   0, 		 0,
							0, 		 0, 		scale.z, 0,
							0, 		 0, 		0, 		 1);
	mult(currentMatrix, currentMatrix,scaleMatrix);
}

 void TransformStack::applyTranslateTransform(vec3 translate)
 {
	mat4 translation = getIdentityMatrix();

	// Nvidia Library function
	translation.set_translation(translate);

	mult(currentMatrix, currentMatrix, translation);
 }

 void TransformStack::applyRotateTransform(vec3 rotate, float angles_deg) {
	float rad = angles_deg * (nv_pi / 180); 
	rotate.normalize();
	mat4 rotation = getIdentityMatrix();

	// Nvidid library function
	rotation.set_rot(rad,rotate);
	mult(currentMatrix, currentMatrix, rotation);
}