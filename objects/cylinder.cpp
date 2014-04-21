// Cylinder object

#include <math.h>
#include "include/cylinder.h"

Cylinder::Cylinder(vec4 &topCenter, vec4 &bottomCenter, float radius)
{
	this->topCenter = topCenter;
	this->bottomCenter = bottomCenter;
	this->radius = radius;
}

/* Cylinder intersection test
http://cs.gettysburg.edu/~skim/cs373/lectures/16_Rectangle_Cylinder.pdf */

bool Cylinder::intersect(Ray &ray, Hit *hit)
{
	vec3 cylinderAxis = topCenter - bottomCenter;
	cylinderAxis.normalize();

	vec3 bottomCenterToRayOrigin = ray.P - bottomCenter;

	vec3 u = ray.D - dot(ray.D, cylinderAxis) * cylinderAxis;
	vec3 v = bottomCenterToRayOrigin - dot(bottomCenterToRayOrigin, cylinderAxis) * cylinderAxis;

	// Solve the quadra	tic equation
	float a = dot(u, u);
	float b = dot(u, v);
	float c = dot(v, v) - radius * radius;

	float disc = b*b - 4*a*c;

	if (disc < 0.0)
	{
		return false; // a negative value indicates no intersection.
	}

	float ds = sqrtf(disc);
	float q;

	if (b < 0.0)
	{
		q = (-b - ds)/2.0;
	} else
	{
		q = (-b + ds)/2.0;
	}

	float t0 = q/a;
	float t1 = c/q;

	if (t0>t1)
	{
		float temp = t0;
		t0 = t1;
		t1 = temp;
	}

	if (t1 < 0.0)
	{
		return false;
	}

	// if an intersection has been found, record details in hit object

	hit->obj = this;

	if (t0 < 0.0)
	{
		hit->t = t1;

		hit->p.x = ray.P.x  + t1 * ray.D.x;
		hit->p.y = ray.P.y  + t1 * ray.D.y;
		hit->p.z = ray.P.z  + t1 * ray.D.z;
		hit->p.w = 1.0;
		hit->n.x = hit->p.x - topCenter.x;
		hit->n.y = hit->p.y - topCenter.y;
		hit->n.z = hit->p.z - topCenter.z;
		hit->n.normalize();

		return true;
	}

	hit->t = t0;

	hit->p.x = ray.P.x  + t0 * ray.D.x;
	hit->p.y = ray.P.y  + t0 * ray.D.y;
	hit->p.z = ray.P.z  + t0 * ray.D.z;
	hit->p.w = 1.0;
	hit->n.x = hit->p.x - topCenter.x;
	hit->n.y = hit->p.y - topCenter.y;
	hit->n.z = hit->p.z - topCenter.z;
	hit->n.normalize();

	return true;
}
