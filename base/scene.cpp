#include <math.h>
#include <iostream>

#include "include/scene.h"

// Objects
#include "include/sphere.h"
#include "include/triangle.h"
#include "include/plane.h"

// Lights
#include "include/directional_light.h"

// Transform Stack
#include "include/transformStack.h"

Scene::Scene()
{
	scache = 0;
	obj_list = (Object *)0;
}

void Scene::addObject(Object &obj)
{
	obj.link(obj_list);
	obj_list = &obj;
}

void Scene::addLight(Light &lt)
{
	lt.link(light_list);
	light_list = &lt;
}

Colour Scene::raytrace(Ray &ray, int level)
{
	float ta,t;
	Colour col;
	Object *obj;
	Object *closest;
	Light *lt;
	Hit   hit;
	vec4 position;
	vec4 lightPosition;
	vec3 normal;

	if (level == 0)
	{
		col.clear();
		return col; // stop if recursed deep enough
	}

	// TEST EACH OBJECT AGAINST RAY, FIND THE CLOSEST

	t = 1000000000.0; // a long way aways
	closest = (Object *)0;
	obj = obj_list;

//	ray = ray.worldToObjectSpace(obj);

	while (obj != (Object *)0)
	{
		if(obj->intersect(ray, &hit) == true)
		{
			if (hit.t < t)
			{
//				hit.objectToWorldSpace();
				closest = hit.obj;
				t = hit.t;
				normal = hit.n;
				position = hit.p;
			}
		}

		obj = obj->next();
	}

	col.clear();

	if (closest != (Object *)0)
	{
		lt = light_list;

		Colour ka = closest->obj_mat->ka;
		Colour kd = closest->obj_mat->kd;
		Colour ks = closest->obj_mat->ks;
		Colour kr = closest->obj_mat->kr;
		Colour kt = closest->obj_mat->kt;
		double shininess = closest->obj_mat->n;
		double refractiveIndex = closest->obj_mat->refractiveIndex;

		bool shiny = closest->obj_mat->shiny;
		bool reflective = closest->obj_mat->reflective;
		bool refractive = closest->obj_mat->refractive;

		Ray sray, reflectedRay, refractedRay;

		// Fix speckly
		vec3 dSmall;
		mult(dSmall, ray.D, 0.00001);
		position -= dSmall;

		vec3 viewerDirection = (ray.P - position);
		viewerDirection.normalize();

		while (lt != (Light *)0)
		{
			vec3 xldir;
			Colour lcol;

			lt->getLightProperties(lightPosition, &xldir, &lcol);
			xldir.normalize();

			// add shadow test here

			Ray shadowRay;

			shadowRay.P = position;
			shadowRay.D = xldir;

			if (shadowtrace(shadowRay, t) == false) {
				// calculate diffuse component

				float dlc = dot(xldir, normal);

				if (dlc >= 0.0) {
					// calculate specular component here

					float slc = 0.0;

					if (shiny == true) {
						vec3 lightOffset = vec3(lightPosition - position);
						lightOffset.normalize();

						vec3 reflectionVector = ReflectionVector(lightOffset, normal);

						float slcDot = dot(viewerDirection, reflectionVector);

						if (slcDot > 0.0) {
							slc = pow(slcDot, shininess);
						}
					}

					// combine components

					col.red += ka.red + lcol.red*(dlc * kd.red + slc * ks.red);
					col.green += ka.green + lcol.green*(dlc * kd.green + slc * ks.green);
					col.blue += ka.blue + lcol.blue*(dlc * kd.blue + slc * ks.blue);
				}
			}

			lt = lt->next(); // next light
		}

		// add reflected rays here
		if (reflective == true) {
			reflectedRay.D = ReflectionVector(viewerDirection, normal);
			reflectedRay.D.normalize();
			reflectedRay.P = position;

			Colour reflectedColour = raytrace(reflectedRay, level-1);
			reflectedColour *= kr;

			col.red += reflectedColour.red;
			col.blue += reflectedColour.blue;
			col.green += reflectedColour.green;
		}		

		vec4 newPosition = position;
		newPosition += dSmall;
		newPosition += dSmall;

		vec3 newviewerDirection = (ray.P - newPosition);
		newviewerDirection.normalize();
	

		bool internalReflection = false;
		// add refracted rays here
		if (refractive == true) {
			refractedRay.D = RefractVector(normal, newviewerDirection, viewerDirection, refractiveIndex, internalReflection);
			if (internalReflection == true) {
				refractedRay.P = position;
			} else {
				refractedRay.P = newPosition;
			}

			Colour refractedColour = raytrace(refractedRay, level -1);
			refractedColour *= kt;

			col.red += refractedColour.red;
			col.blue += refractedColour.blue;
			col.green += refractedColour.green;
		}
	}

	return col;
}


bool Scene::shadowtrace(Ray &ray, double tlimit)
{
	Object *obj;
	Hit   hit;

	// check for shadow intersections
	obj = obj_list;

	while (obj != (Object *)0)
	{
		if(obj->intersect(ray, &hit) == true)
		{
			if (hit.t < tlimit)
			{
				return true;
			}
		}

		obj = obj->next();
	}

	return false;
}

vec3 Scene::ReflectionVector(vec3 vector, vec3 normal)
{
	return -vector + dot(vector, normal) * 2 * normal;
}

vec3 Scene::RefractVector(vec3 normal, vec3 incident, vec3 oldIncident, double refractIndex, bool internalReflection)
{
	double n;
	double cosI = dot(normal, incident);
	vec3 newNormal;
	if (cosI < 0) {
	 	n = 1.0003/refractIndex;
	 	cosI = -cosI;
	} else {
		n = refractIndex/1.0003;
	 	newNormal = -normal;
	}

	double snellRoot = 1.0 - (n * n * (1.0 - cosI * cosI));
	if (snellRoot <= 1.0) {
		// Total internal reflection
		internalReflection = true;
		return ReflectionVector(normal, oldIncident);
	}
	return (n * incident) + (n * cosI - sqrtf(snellRoot)) * newNormal;
}

/* Predefined scenes */

float frand()
{
	int x;
	float f;

	x = rand();
	f = (float)(x & 0xffff);
	f = f/65536.0;

	return f;
}

const void Scene::createScene1(Camera *camera)
{
	TransformStack *stack;
	vec3 v, v2;
	DirectionalLight *dl, *dl2;
	Colour cl;
	Material *m;

	camera->eyePosition = vec3(0.0, -2.0, 10.0);
	camera->lookAt = vec3(0.0, 0.0, -8.0);

	// Transformations
	stack = new TransformStack();
	stack->pushMatrix();
	stack->setIdentityMatrix();

	// Create and add a directional light to the scene
	v = vec3(-1.0, -1.0, -2.0);
	cl.set(2.0, 2.0, 2.0, 2.0);
	dl = new DirectionalLight(v, cl);
	this->addLight(*dl);

	// Create and add the glass sphere
	Sphere *glassSphere;
	vec4 p;
	p = vec4(0.0, -3.0, -4.0, 0.0);
	glassSphere = new Sphere(p, 2.0);
	invert(glassSphere->inverseTransformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateGlassMaterial();
	glassSphere->setMaterial(m);
	this->addObject(*glassSphere);

	// Create and add the shiny red sphere behind 
	Sphere *redSphere;
	p = vec4(4.0, -3.0, -14.0, 0.0);
	redSphere = new Sphere(p, 2.0);
	invert(redSphere->inverseTransformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyRedMaterial();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

	// Add plane bottom
	Plane *bottomPlane;
	p = vec4(0.0, 1.0, 0.0, 1.0);
	bottomPlane = new Plane(p, 5.0);
	invert(bottomPlane->inverseTransformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateWhiteColour();
	bottomPlane->setMaterial(m);
	this->addObject(*bottomPlane);


	/* Add a triangle
	Triangle *t;
	vec4 p0, p1, p2;

	p0 = vec4(-1.0, 0.0, -2.0, 1.0);
	p1 = vec4(0.0, 1.0, -2.0, 1.0);
	p2 = vec4(1.0, 0.0, -2.0, 1.0);

	t = new Triangle(p0, p1, p2);
	invert(t->inverseTransformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateRandomColour();
	t->setMaterial(m);

//	this->addObject(*t);*/

	/* Add plane left
	Plane *leftPlane;
	p = vec4(1.0, 0.0, 0.0, 1.0);

	leftPlane = new Plane(p, 5.0);

	invert(leftPlane->inverseTransformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateRandomColour();
	leftPlane->setMaterial(m);

	scene->addObject(*leftPlane);

	// Add plane right
	Plane *rightPlane;
	p = vec4(-1.0, 0.0, 0.0, 1.0);

	rightPlane = new Plane(p, 12.0);

	invert(rightPlane->inverseTransformation, stack->copyCurrentMatrix());
	m->generateRandomColour();
	rightPlane->setMaterial(m);

	scene->addObject(*rightPlane);

	// Add plane back
	Plane *backPlane;
	p = vec4(0.0, 0.0, -1.0, 1.0);

	backPlane = new Plane(p, 30.0);

	invert(backPlane->inverseTransformation, stack->copyCurrentMatrix());
	m->generateRandomColour();
	backPlane->setMaterial(m);

	scene->addObject(*backPlane);*/
}