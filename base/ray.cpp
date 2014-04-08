#include "include/ray.h"


Vertex Ray::position(double t)
{
  Vertex v;

  v.x = P.x + t * D.x;
  v.y = P.y + t * D.y;
  v.z = P.z + t * D.z;
  v.w = P.w;

  return v;
}

Vector Ray::getDirection()
{
	return D;
}

Vertex Ray::getPosition()
{
	return P;
}

