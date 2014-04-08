#include "include/ray.h"

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

