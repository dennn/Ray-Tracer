// Plane object
#include <math.h>

#include "include/plane.h"

Plane::Plane(vec3 &pNormal, float pDistance)
{
	normal = pNormal;
	distance = pDistance;
}

/** 
 * Plane intersection test
 * Inspired by: 	
 */
bool Plane::intersect(Ray &ray, Hit *hit)
{	
	double cosineRay = dot(normal, ray.D);

	if (fabs(cosineRay) < FLOAT_ZERO) {
		return false;
	}

	double _distance = -(dot(ray.P, normal) + distance) / cosineRay;

	if (_distance < 0.0) {
		return false;
	}

	// We've got a hit
	hit->obj = this;

	// Distance
	hit->t = _distance;
	
	// Point
	hit->p.x = ray.P.x  + _distance * ray.D.x;
	hit->p.y = ray.P.y  + _distance * ray.D.y;
	hit->p.z = ray.P.z  + _distance * ray.D.z;
	hit->p.w = 1.0;
	// Normal
	hit->n.x = normal.x;
	hit->n.y = normal.y;
	hit->n.z = normal.z;
	hit->n.normalize();

	return true;
}
