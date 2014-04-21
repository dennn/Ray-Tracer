#include <math.h>
#include <iostream>

#include "include/scene.h"

#include "include/object.h"

// Objects
#include "include/sphere.h"
#include "include/triangle.h"
#include "include/plane.h"
#include "include/cylinder.h"

// Lights
#include "include/directional_light.h"
#include "include/point_light.h"

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

	ray = ray.worldToObjectSpace(obj);

	while (obj != (Object *)0)
	{
		if(obj->intersect(ray, &hit) == true)
		{
			if (hit.t < t)
			{
				hit.objectToWorldSpace();
				closest = hit.obj;
				t = hit.t;
				normal = hit.n;
				normal.normalize();
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
		mult(dSmall, ray.D, 0.001);
		vec4 newPosition = position;
		newPosition -= dSmall;

		vec3 viewerDirection = (ray.P - position);
		viewerDirection.normalize();

		while (lt != (Light *)0)
		{
			vec3 xldir;
			Colour lcol;

			lt->getLightProperties(position, &xldir, &lcol);
			xldir.normalize();

			// add shadow test here

			Ray shadowRay;

			shadowRay.P = newPosition;
			shadowRay.D = xldir;

			if (shadowtrace(shadowRay, t) == false) {
				// calculate diffuse component

				float dlc = dot(xldir, normal);

				if (dlc >= 0.0) {
					// calculate specular component here

					float slc = 0.0;

					if (shiny == true) {
						vec3 lightOffset = vec3(lightPosition - newPosition);
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
			reflectedRay.D = ReflectionVector(ray.D, normal);
			reflectedRay.D.normalize();

			// Do the offset
			vec3 reflectionDSmall;
			mult(reflectionDSmall, reflectedRay.D, 0.0001);
			vec4 newReflectionPosition = position;
			newReflectionPosition += reflectionDSmall;

			reflectedRay.P = newReflectionPosition;

			Colour reflectedColour = raytrace(reflectedRay, level-1);
			reflectedColour *= kr;

			col.red += reflectedColour.red;
			col.blue += reflectedColour.blue;
			col.green += reflectedColour.green;
		}		

		// add refracted rays here
		if (refractive == true) {
			refractedRay.D = RefractVector(normal, ray.D, refractiveIndex);
			refractedRay.D.normalize();

			// Do the offset
			vec3 refractionDSmall;
			mult(refractionDSmall, refractedRay.D, 0.001);
			vec4 newRefractionPosition = position;
			newRefractionPosition += refractionDSmall;
			refractedRay.P = newRefractionPosition;

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

vec3 Scene::RefractVector(vec3 normal, vec3 incident, double refractIndex)
{
	double n;
	double cosI = dot(normal, incident);
	vec3 newNormal = normal;
	if (cosI < 0) {
		// Entering the object
	 	n = 1.0003/refractIndex;
	} else {
		// Exiting the object
		n = refractIndex/1.0003;
	 	newNormal = -normal;
	}

	cosI = dot(incident, newNormal);

	double snellRoot = 1.0 - (n * n) * (1.0 - cosI * cosI);

	if (snellRoot < 0.0) {
		// Total internal reflection
		return ReflectionVector(newNormal, incident);
	} else {
		return (n * incident) + (n * cosI + sqrtf(snellRoot)) * newNormal;
	}
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
	DirectionalLight *dl;
	PointLight *pl;

	Colour cl;
	Material *m;

	camera->eyePosition = vec3(0.0, -2.0, 10.0);
	camera->lookAt = vec3(0.0, 0.0, -8.0);

	// Transformations
	stack = new TransformStack();
	stack->pushMatrix();
	//stack->applyScaleTransform(vec3(2.0, 1.0, 2.0));

	/* Create and add a directional light to the scene
	v = vec3(-1.0, -1.0, -2.0);
	cl.set(2.0, 2.0, 2.0, 2.0);
	dl = new DirectionalLight(v, cl);
	this->addLight(*dl);*/

	// Create and add point lights to the scene
	vec4 lightPosition = vec4(-3.0, 3.0, -9.0, 1.0);
	cl.set(1.0, 1.0, 0.2, 1.0);
	pl = new PointLight(lightPosition, cl);
	this->addLight(*pl);

	// Create and add the glass sphere
	Sphere *glassSphere;
	vec4 p;
	p = vec4(0.0, -3.0, -4.0, 1.0);
	glassSphere = new Sphere(p, 2.0);
	invert(glassSphere->inverseTransformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateGlassMaterial();
	glassSphere->setMaterial(m);
	this->addObject(*glassSphere);

	// Create and add the shiny red sphere behind 
	Sphere *redSphere;
	p = vec4(4.0, -3.0, -14.0, 1.0);
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

	/* Add a cylinder
	Cylinder *cylinder;
	vec4 bottom = vec4(-4.0, -3.0, -14.0, 1.0);
	vec4 top = vec4(2.0, 2.0, -14.0, 1.0);
	cylinder = new Cylinder(bottom, top, 5.0f);
	invert(cylinder->inverseTransformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyRedMaterial();
	cylinder->setMaterial(m);
	this->addObject(*cylinder);*/
}