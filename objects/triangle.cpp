// Triangle object

#include <math.h>

#include "include/triangle.h"

Triangle::Triangle(vec4 &pv0, vec4 &pv1, vec4 &pv2)
{
	v0 = pv0;
	v1 = pv1;
	v2 = pv2;
}

/** 
 * Triangle intersection test
 * Inspired by: https://github.com/AlexanderTolmachev/ray-tracer/blob/master/src/triangle.cpp
 */
bool Triangle::intersect(Ray &ray, Hit *hit)
{
	vec3 e1, e2, normal;

	e1 = vec3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
	e2 = vec3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);

	cross(normal, e1, e2);
	normal.normalize();

	vec3 pVector;
	cross(pVector, ray.D, e2);
	double determinant = dot(e1, pVector);

	if (fabs(determinant) < FLOAT_ZERO) {
		return false;
	}

	double invertedDeterminant = 1.0/determinant;

	vec3 tVector = vec3(ray.P.x - v0.x, ray.P.y - v0.y, ray.P.z - v0.z);

	double lambda = dot(tVector, pVector);
	lambda *= invertedDeterminant;

	if (lambda < 0.0 || lambda > 1.0) {
		return false;
	}

	vec3 qVector;
	cross(qVector, tVector, e1);

	double mue = dot(ray.D, qVector);
	mue *= invertedDeterminant;

	if (mue < 0.0 || mue + lambda > 1.0) {
		return false;
	}

	double distance = invertedDeterminant * dot(e2, qVector);

	if (distance < FLOAT_ZERO) {
		return false;
	}

	// We've got a hit

	hit->obj = this;

	// Distance
	hit->t = distance;
	// Point
	hit->p.x = ray.P.x  + distance * ray.D.x;
	hit->p.y = ray.P.y  + distance * ray.D.y;
	hit->p.z = ray.P.z  + distance * ray.D.z;
	hit->p.w = 1.0;
	// Normal
	hit->n.x = normal.x;
	hit->n.y = normal.y;
	hit->n.z = normal.z;
	hit->n.normalize();

	return true;
}
