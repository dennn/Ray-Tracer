// Cylinder object

#include <math.h>
#include "include/cylinder.h"

Cylinder::Cylinder(vec4 &topCenter, vec4 &bottomCenter, float radius);
{
	this.topCenter = topCenter;
	this.bottomCenter = bottomCenter;
	this.radius = radius;
}

/* Cylinder intersection test
http://cs.gettysburg.edu/~skim/cs373/lectures/16_Rectangle_Cylinder.pdf */

bool Sphere::intersect(Ray &ray, Hit *hit)
{
	vec3 cylinderAxis = topCenter - bottomCenter;
	cylinderAxis.normalize();

	vec3 bottomCenterToRayOrigin = ray.P - bottomCenter;

	vec3 u = ray.D - cylinderAxis * dot(ray.D);
	vec3 v = bottomCenterToRayOrigin - cylinderAxis * dot(bottomCenterToRayOrigin, cylinderAxis);

	// Solve the quartic equation

	float a = dot(u, u);

	if (fabs(a) > 0.0) {
		float b = dot(u, v);
		float c = dot(v, v) - radius * radius;

		float discriminant = b*b - 4*a*c;

		if (discriminant < 0.0) {
			return false;
		}

		discriminant = sqrtf(discriminant);

		float denominator = 1.0/ (2.0 * a);

		root = (-b - discriminant) * denominator;

		if (root >= 0.0) {

		}

	}

	// offset ray by sphere position
	// equivalent to transforming ray into local sphere space

	ro = vec3(ray.P.x-sp.x,ray.P.y-sp.y,ray.P.z-sp.z);

	float a = dot(ray.D, ray.D);
	float b = 2.0 * dot(ray.D, ro);
	float c = dot(ro, ro) - r*r;

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
		hit->n.x = hit->p.x - sp.x;
		hit->n.y = hit->p.y - sp.y;
		hit->n.z = hit->p.z - sp.z;
		hit->n.normalize();

		return true;
	}

	hit->t = t0;

	hit->p.x = ray.P.x  + t0 * ray.D.x;
	hit->p.y = ray.P.y  + t0 * ray.D.y;
	hit->p.z = ray.P.z  + t0 * ray.D.z;
	hit->p.w = 1.0;
	hit->n.x = hit->p.x - sp.x;
	hit->n.y = hit->p.y - sp.y;
	hit->n.z = hit->p.z - sp.z;
	hit->n.normalize();

	return true;
}
