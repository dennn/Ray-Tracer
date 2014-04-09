#include "include/ray.h"
#include "include/object.h"

vec4 Ray::position(double t)
{
  vec4 v;

  v.x = P.x + t * D.x;
  v.y = P.y + t * D.y;
  v.z = P.z + t * D.z;
  v.w = P.w;

  return v;
}

vec3 Ray::getDirection()
{
	return D;
}

vec4 Ray::getPosition()
{
	return P;
}

Ray Ray::inverseTransformOfRay(Object *obj)
{
	vec4 newPosition;
	mult(newPosition, obj->inverseTransformation, this->P);
	vec4 newDirection;
	mult(newDirection,obj->inverseTransformation, this->D);

	Ray newRay;
	newRay.P.x = newPosition.x;
	newRay.P.y = newPosition.y;
	newRay.P.z = newPosition.z;
	newRay.P.w = newPosition.w;
	newRay.D.x = newDirection.x;
	newRay.D.y = newDirection.y;
	newRay.D.z = newDirection.z;
	return newRay;
}


