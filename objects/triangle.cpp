// Triangle object

#include <math.h>

#include "include/triangle.h"

Triangle::Triangle(Vertex &pv0, Vertex &pv1, Vertex &pv2)
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
	Vector e1, e2, normal;
	Vector rayDirection = ray.getDirection();
	Vertex rayPosition = ray.getPosition();

	e1 = Vector(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
	e2 = Vector(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);

	normal = e1.cross(e2);
	normal.normalise();

	Vector pVector = rayDirection.cross(e2);
	double determinant = e1.dot(pVector);

	if (fabs(determinant) < FLOAT_ZERO) {
		return false;
	}

	double invertedDeterminant = 1.0/determinant;

	Vector tVector = Vector(rayPosition.x - v0.x, rayPosition.y - v0.y, rayPosition.z - v0.z);

	double lambda = tVector.dot(pVector);
	lambda *= invertedDeterminant;

	if (lambda < 0.0 || lambda > 1.0) {
		return false;
	}

	Vector qVector = tVector.cross(e1);

	double mue = rayDirection.dot(qVector);
	mue *= invertedDeterminant;

	if (mue < 0.0 || mue + lambda > 1.0) {
		return false;
	}

	double distance = invertedDeterminant * e2.dot(qVector);

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
	hit->n.normalise();

	return true;
}
