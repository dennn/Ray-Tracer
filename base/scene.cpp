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

#define AMBIENT_OCCLUSION false
#define AMBIENT_OCCLUSION_SAMPLES 16

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

Colour Scene::raytrace(Ray &ray, int level, Camera *camera)
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


	while (obj != (Object *)0)
	{
		if(obj->intersect(ray, &hit) == true)
		{
			if (hit.t < t)
			{
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
		if (AMBIENT_OCCLUSION == true) {
			int numHits = 0;

			vec3 dSmall;
			mult(dSmall, ray.D, 0.0001);
			vec4 newPosition = position;
			newPosition -= dSmall;

			for (int sample = 0; sample <= AMBIENT_OCCLUSION_SAMPLES; sample++) {
				Ray ambientRay;
				ambientRay.D = CosineWeightedRandomHemisphereDirection(normal);
				ambientRay.P = newPosition;
				if (shadowtrace(ambientRay, 100) == true) {
					numHits++;
				}
			}

			float ratio = 1.0f - (float)((float)numHits/ (float)AMBIENT_OCCLUSION_SAMPLES);

			col.setRGBA(255.0f * ratio, 255.0f * ratio, 255.0f* ratio, 255.0f* ratio);

		} else {
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

							vec3 reflectionVector = ReflectionVector(-xldir, normal);
							reflectionVector.normalize();

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

				Colour reflectedColour = raytrace(reflectedRay, level-1, camera);
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

				Colour refractedColour = raytrace(refractedRay, level -1, camera);
				refractedColour *= kt;

				col.red += refractedColour.red;
				col.blue += refractedColour.blue;
				col.green += refractedColour.green;
			}
		}
	} else {
		// We didn't hit anything return the background colour
		col = camera->backgroundColour;
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
	return vector - dot(vector, normal) * 2 * normal;
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

	cosI = -dot(incident, newNormal);

	double snellRoot = 1.0 - (n * n) * (1.0 - cosI * cosI);

	if (snellRoot < 0.0) {
		// Total internal reflection
		return ReflectionVector(newNormal, incident);
	} else {
		return (n * incident) + (n * cosI - sqrtf(snellRoot)) * newNormal;
	}
}

//http://pathtracing.wordpress.com/2011/03/03/cosine-weighted-hemisphere/
vec3 Scene::CosineWeightedRandomHemisphereDirection(vec3 n)
{
    float Xi1 = (float)rand()/(float)RAND_MAX;
    float Xi2 = (float)rand()/(float)RAND_MAX;

    float theta = acos(sqrt(1.0-Xi1));
    float phi = 2.0 * nv_pi * Xi2;

    float xs = sinf(theta) * cosf(phi);
    float ys = cosf(theta);
    float zs = sinf(theta) * sinf(phi);

    vec3 y = vec3(n.x, n.y, n.z);
    vec3 h = y;
    if (fabs(h.x)<=fabs(h.y) && fabs(h.x)<=fabs(h.z))
        h.x= 1.0;
    else if (fabs(h.y)<=fabs(h.x) && fabs(h.y)<=fabs(h.z))
        h.y= 1.0;
    else
        h.z= 1.0;


    vec3 x, z;
    x = h ^ y ;
    x.normalize();

	z = x ^ y;
	z.normalize();

    vec3 direction = xs * x + ys * y + zs * z;
    direction.normalize();

    return direction;
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

// All primitives
const void Scene::createScene1(Camera *camera)
{
	TransformStack *stack;
	vec3 v, v2;
	DirectionalLight *dl;
	PointLight *pl;

	Colour cl;
	Material *m;

	vec4 p;

	camera->eyePosition = vec3(3.0, 2.0, 8.0);
	camera->lookAt = vec3(0.0, 0.0, -3.0);
	camera->calculateUVW();
	cl.setRGBA(51.0f, 153.0f, 255.0f, 255.0f);
//	camera->backgroundColour = cl;

	// Transformations
	stack = new TransformStack();
	stack->pushMatrix();
	//stack->applyScaleTransform(vec3(2.0, 2.0, 2.0));

	// Create and add a directional light to the scene
/*	v = vec3(-1.0, -1.0, -2.0);
	cl.set(2.0, 2.0, 2.0, 2.0);
	dl = new DirectionalLight(v, cl);
	this->addLight(*dl);*/

	// Create and add point lights to the scene
	vec4 lightPosition = vec4(8.0, 4.0, -8.0, 1.0);
	vec3 lightDirection = vec3(0.0, -1.0, 0.0);
	cl.setRGBA(46.0f, 204.0f, 64.0f, 255.0f);
	cl.setScale(1.0f);
	pl = new PointLight(lightPosition, cl);
	//pl = new PointLight(lightPosition, lightDirection, cl);
	this->addLight(*pl);

	//Create a pyramid
	Triangle *t1, *t2, *t3;

	vec4 pTop, pLeft, pRight, pFront;
	pTop = vec4(0.0, -3.0, -2.0, 1.0);
	pLeft = vec4(2.0, -5.0, -4.0, 1.0);
	pRight = vec4(-2.0, -5.0, -4.0, 1.0);
	pFront = vec4(0.0, -5.0, 0.0, 1.0);

	t1 = new Triangle(pRight, pLeft, pTop);
	t2 = new Triangle(pFront, pLeft, pTop);
	t3 = new Triangle(pRight, pFront, pTop);

	t1->transformation = stack->copyCurrentMatrix();
	t2->transformation = stack->copyCurrentMatrix();
	t3->transformation = stack->copyCurrentMatrix();

	m = new Material();
	m->generateGreenColour();
	t1->setMaterial(m);
	t2->setMaterial(m);
	t3->setMaterial(m);

	this->addObject(*t1);
	this->addObject(*t2);
	this->addObject(*t3);

	// Create and add a sphere
	Sphere *redSphere;
	p = vec4(4.0, -3.0, -14.0, 1.0);
	redSphere = new Sphere(p, 2.0);
	redSphere->transformation = stack->copyCurrentMatrix();
	m = new Material();
	m->generateShinyRedColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

//	stack->applyScaleTransform(vec3(3.0, 3.0, 3.0));
	//stack->applyTranslateTransform(vec3(0.0, 3.0, 0.0));

	// Create and add a sphere
	Sphere *blueSphere;
	p = vec4(-4.0, -3.0, -14.0, 1.0);
	blueSphere = new Sphere(p, 2.0);
	blueSphere->transformation = stack->copyCurrentMatrix();
	m = new Material();
	m->generateBlueColour();
	blueSphere->setMaterial(m);
	this->addObject(*blueSphere);

	// Create and add the glass sphere
/*	Sphere *glassSphere;
	p = vec4(0.0, -3.0, 0.0, 1.0);
	glassSphere = new Sphere(p, 2.0);
	invert(glassSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateGlassMaterial();
	glassSphere->setMaterial(m);
	this->addObject(*glassSphere);*/

	stack->setIdentityMatrix();

	// Add plane bottom
	Plane *bottomPlane;
	vec3 pNormal = vec3(0.0, 1.0, 0.0);
	bottomPlane = new Plane(pNormal, 5.0);
	bottomPlane->transformation = stack->copyCurrentMatrix();
	m = new Material();
	m->generateWhiteColour();
	bottomPlane->setMaterial(m);
	this->addObject(*bottomPlane);
}

// All primitives, reflective floor
const void Scene::createScene2(Camera *camera)
{
	DirectionalLight *dl;
	TransformStack *stack;

	Colour cl;
	Material *m;

	vec4 p;

	camera->eyePosition = vec3(6.0, 2.0, 12.0);
	camera->lookAt = vec3(3.0, 2.0, 0.0);
	camera->calculateUVW();
	cl.setRGBA(173.0f, 223.0f, 229.0f, 255.0f);
	camera->backgroundColour = cl;

	// Transformations
	stack = new TransformStack();
	stack->pushMatrix();
	//stack->applyScaleTransform(vec3(1.0, 1.0, 1.0));

	// Create and add a directional light to the scene
	vec3 v = vec3(-1.0, -1.0, -2.0);
	cl.set(2.0, 2.0, 2.0, 2.0);
	dl = new DirectionalLight(v, cl);
	this->addLight(*dl);


	for (int i =0; i <= 5; i++) {
		for (int j = 0; j <= 7; j++) {
			if (j == 3 && i == 6) {
				continue;
			} else {
				// Create and add a sphere
				Sphere *redSphere;
				p = vec4(i*2.1, 2.0, j *-2.1, 1.0);
				redSphere = new Sphere(p, 1.0);
				invert(redSphere->transformation, stack->copyCurrentMatrix());
				m = new Material();
				m->generateGreenColour();
				redSphere->setMaterial(m);
				this->addObject(*redSphere);
			}
		}
	}

	stack->setIdentityMatrix();

	// Add plane bottom
	Plane *bottomPlane;
	vec3 pNormal = vec3(0.0, 1.0, 0.0);
	bottomPlane = new Plane(pNormal, 5.0);
	invert(bottomPlane->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateWhiteColour();
	bottomPlane->setMaterial(m);
	this->addObject(*bottomPlane);
}

// All primitives, reflective floor
const void Scene::createScene3(Camera *camera)
{
	PointLight *pl, *pl2;
	TransformStack *stack;

	Colour cl;
	Material *m;

	vec4 p;

	camera->eyePosition = vec3(0.0, 0.0, 12.0);
	camera->lookAt = vec3(0.0, 0.0, 0.0);

	camera->calculateUVW();

	// Transformations
	stack = new TransformStack();
	stack->pushMatrix();

	// Create and add point lights to the scene
	vec4 lightPosition = vec4(0.0, 3.0, -9.0, 1.0);
	cl.setRGBA(255.0f, 51.0f, 51.0f, 255.0f);
	pl = new PointLight(lightPosition, cl);
	this->addLight(*pl);


	lightPosition = vec4(0.0, 3.0, -9.0, 1.0);
	cl.setRGBA(255.0f, 184.0f, 51.0f, 255.0f);
	pl2 = new PointLight(lightPosition, cl);
	this->addLight(*pl2);

	// Add plane bottom
	Plane *bottomPlane;
	vec3 pNormal = vec3(0.0, 1.0, 0.0);
	bottomPlane = new Plane(pNormal, 2.0);
	invert(bottomPlane->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateWhiteColour();
	bottomPlane->setMaterial(m);
	this->addObject(*bottomPlane);
}

// Point Lights
const void Scene::createSnookerScene(Camera *camera)
{
	PointLight *pl, *pl2;
	TransformStack *stack;

	Colour cl;
	Material *m;

	vec4 p;

	camera->eyePosition = vec3(2.0, 6.0, 10.0);
	camera->lookAt = vec3(2.0, 0.0, -5.0);
	camera->calculateUVW();
	cl.setRGBA(255.0f, 255.0f, 255.0f, 255.0f);
	camera->backgroundColour = cl;

	// Transformations
	stack = new TransformStack();
	stack->pushMatrix();
	stack->setIdentityMatrix();

	// Create and add point lights to the scene
	vec4 lightPosition = vec4(0.0, 5.0, 0.0, 1.0);
	vec3 lightDirection = vec3(0.0, 1.0, -1.0);
	cl.setRGBA(255.0f, 255.0f, 204.0f, 255.0f);
	pl = new PointLight(lightPosition, cl);
	this->addLight(*pl);

	// Add the balls row 1
	Sphere *redSphere;
	p = vec4(-2.0, 1.0, -5.0, 1.0);
	redSphere = new Sphere(p, 1.0);
	invert(redSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyYellowColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

	p = vec4(0.0, 1.0, -5.0, 1.0);
	redSphere = new Sphere(p, 1.0);
	invert(redSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyYellowColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

	p = vec4(2.0, 1.0, -5.0, 1.0);
	redSphere = new Sphere(p, 1.0);
	invert(redSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyRedColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

	p = vec4(4.0, 1.0, -5.0, 1.0);
	redSphere = new Sphere(p, 1.0);
	invert(redSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyYellowColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

	p = vec4(6.0, 1.0, -5.0, 1.0);
	redSphere = new Sphere(p, 1.0);
	invert(redSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyRedColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

	// Row 2

	p = vec4(-1.0, 1.0, -6.0, 1.0);
	redSphere = new Sphere(p, 1.0);
	invert(redSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyRedColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

	p = vec4(1.0, 1.0, -6.0, 1.0);
	redSphere = new Sphere(p, 1.0);
	invert(redSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyYellowColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

	p = vec4(3.0, 1.0, -6.0, 1.0);
	redSphere = new Sphere(p, 1.0);
	invert(redSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyRedColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

	p = vec4(5.0, 1.0, -6.0, 1.0);
	redSphere = new Sphere(p, 1.0);
	invert(redSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyYellowColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

	// Row 3

	p = vec4(0.0, 1.0, -7.0, 1.0);
	redSphere = new Sphere(p, 1.0);
	invert(redSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyYellowColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

	p = vec4(2.0, 1.0, -7.0, 1.0);
	redSphere = new Sphere(p, 1.0);
	invert(redSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyBlackColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

	p = vec4(4.0, 1.0, -7.0, 1.0);
	redSphere = new Sphere(p, 1.0);
	invert(redSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyRedColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

	// Row 4

	p = vec4(1.0, 1.0, -8.0, 1.0);
	redSphere = new Sphere(p, 1.0);
	invert(redSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyRedColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

	p = vec4(3.0, 1.0, -8.0, 1.0);
	redSphere = new Sphere(p, 1.0);
	invert(redSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyYellowColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

	// Row 5

	p = vec4(2.0, 1.0, -9.0, 1.0);
	redSphere = new Sphere(p, 1.0);
	invert(redSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateShinyRedColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);


	// Add plane bottom
	Plane *bottomPlane;
	vec3 pNormal = vec3(0.0, 1.0, 0.0);
	bottomPlane = new Plane(pNormal, 0.0);
	invert(bottomPlane->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateGreenColour();
	bottomPlane->setMaterial(m);
	this->addObject(*bottomPlane);
}

const void Scene::createTransparentScene(Camera *camera)
{
	TransformStack *stack;
	vec3 v, v2;
	DirectionalLight *dl;
	PointLight *pl;

	Colour cl;
	Material *m;

	vec4 p;

	camera->eyePosition = vec3(0.0, 2.0, 8.0);
	camera->lookAt = vec3(0.0, 0.0, -3.0);
	camera->calculateUVW();
	cl.setRGBA(51.0f, 153.0f, 255.0f, 255.0f);
//	camera->backgroundColour = cl;

	// Transformations
	stack = new TransformStack();
	stack->pushMatrix();

	// Create and add a directional light to the scene
	v = vec3(-1.0, -1.0, -3.0);
	cl.set(2.0, 2.0, 2.0, 2.0);
	dl = new DirectionalLight(v, cl);
	this->addLight(*dl);

	// Create and add a sphere
	Sphere *redSphere;
	p = vec4(4.0, -3.0, -14.0, 1.0);
	redSphere = new Sphere(p, 2.0);
	redSphere->transformation = stack->copyCurrentMatrix();
	m = new Material();
	m->generateShinyRedColour();
	redSphere->setMaterial(m);
	this->addObject(*redSphere);

	// Create and add a sphere
	Sphere *blueSphere;
	p = vec4(0.0, 3.0, -14.0, 1.0);
	blueSphere = new Sphere(p, 2.0);
	blueSphere->transformation = stack->copyCurrentMatrix();
	m = new Material();
	m->generateBlueColour();
	blueSphere->setMaterial(m);
	this->addObject(*blueSphere);

	// Create and add the glass sphere
	Sphere *glassSphere;
	p = vec4(0.0, -3.0, -6.0, 1.0);
	glassSphere = new Sphere(p, 2.0);
	invert(glassSphere->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateGlassMaterial();
	glassSphere->setMaterial(m);
	this->addObject(*glassSphere);

	// Add plane bottom
	Plane *bottomPlane;
	vec3 pNormal = vec3(0.0, 1.0, 0.0);
	bottomPlane = new Plane(pNormal, 6.0);
	invert(bottomPlane->transformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateGreenColour();
	bottomPlane->setMaterial(m);
	this->addObject(*bottomPlane);
}

