#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Scene
#include "include/scene.h"

// Objects
#include "include/sphere.h"
#include "include/triangle.h"
#include "include/plane.h"

// Lights
#include "include/directional_light.h"

// Camera
#include "include/camera.h"

// Transform Stack
#include "include/transformStack.h"

#define XSIZE 512
#define YSIZE 512
#define ANTIALIASING_SAMPLES 4

#define RAND (float(rand())/float(RAND_MAX))

Colour frame_buffer[YSIZE][XSIZE];

float frand()
{
	int x;
	float f;

	x = rand();
	f = (float)(x & 0xffff);
	f = f/65536.0;

	return f;
}

void write_framebuffer()
{
	int x, y;
	float r, g, b;

	printf("P3\n%d %d\n255\n", XSIZE, YSIZE);

	for(y=YSIZE-1;y>=0;y-=1)
	{
		for(x=0;x<XSIZE;x+=1)
		{
			r = 255.0 * frame_buffer[y][x].getRed();
			g = 255.0 * frame_buffer[y][x].getGreen();
			b = 255.0 * frame_buffer[y][x].getBlue();
			if (r > 255.0) r = 255.0;
			if (g > 255.0) g = 255.0;
			if (b > 255.0) b = 255.0;
			printf("%d %d %d\n",(int)r, (int)g, (int)b);
		}
	}
}

void clear_framebuffer()
{
	int x,y;

	for(y=0;y<YSIZE;y+=1)
	{
		for(x=0;x<XSIZE;x+=1)
		{
			frame_buffer[y][x].clear();
		}
	}
}

// The main raytacing entry point.

int main(int argc, const char *argv[])
{
	Scene *scene;
	Camera *camera;
	vec3 v;
	int x,y;
	int n;
	DirectionalLight *dl;
	Colour cl;
	TransformStack *stack;

	// Transformations
	stack = new TransformStack();
	stack->pushMatrix();
	//stack->applyScaleTransform(vec3(2.0, 2.0, 2.0));
	//stack->applyTranslateTransform(vec3(0.0, 5.0, 0.0));
	//stack->applyRotateTransform(vec3(0.0, 1.0, 0.0), 90.0f);
	srand(30115);

	clear_framebuffer();

	// Create a new scene to render
	scene = new Scene();

	// Create and add a directional light to the scene
	v = vec3(-1.0,-1.0,3.0);
	cl.set(3.0,3.0,3.0,3.0);
	
	dl = new DirectionalLight(v, cl);
	scene->addLight(*dl);

	// Add 10 random spheres to the scene
	for (n = 0; n < 100; n += 1)
	{
		Sphere *s;
		Material *m;
		vec4 p;

		// position
		p = vec4(frand()-0.5,frand()-0.5,frand()+5.0,1.0);

		// create with random radius
		s = new Sphere(p, frand()/2.0);
		invert(s->inverseTransformation, stack->copyCurrentMatrix());

		// create new material with red random Ka and Kd
		m = new Material();
		m->generateRandomColour();

		// set spheres material
		s->setMaterial(m);

		// as sphere to scene
		scene->addObject(*s);
	}

	// Add a triangle
	Triangle *t;
	Material *m;
	vec4 p0, p1, p2;

	p0 = vec4(-1.0, 0.0, 1.0, 1.0);
	p1 = vec4(0.0, 1.0, 1.0, 1.0);
	p2 = vec4(1.0, 0.0, 1.0, 1.0);

	t = new Triangle(p0, p1, p2);
	invert(t->inverseTransformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateRandomColour();
	t->setMaterial(m);

	scene->addObject(*t);

	// Add a plane
	Plane *plane;
	vec4 p;
	p = vec4(0.0, 1.0, 0.0, 1.0);

	plane = new Plane(p, 5.0);
	invert(t->inverseTransformation, stack->copyCurrentMatrix());
	m = new Material();
	m->generateWhiteColour();
	plane->setMaterial(m);

	scene->addObject(*plane);

	// Create a new camera

	camera = new Camera();
	camera->FOV = 30.0f;

	float focusDistance = camera->FOVToFocusDistance();
	float sampleWeight = 1.0f / (ANTIALIASING_SAMPLES * ANTIALIASING_SAMPLES);

	// RAYTRACE SCENE

	for(y=0;y<YSIZE;y+=1)
	{
		for(x=0;x<XSIZE;x+=1)
		{
			Ray ray;
			Colour col;
			vec3 focusDistanceVector;
			mult(focusDistanceVector, camera->cameraDirection, focusDistance);

			/* If Anti-Aliasing is turned on then fire multiple rays per pixel block, using a jitter*/

			if (ANTIALIASING_SAMPLES > 1) {
				for (int i=0; i< ANTIALIASING_SAMPLES;i++) {
					for (int j=0; j < ANTIALIASING_SAMPLES;j++) {
						ray.P = camera->eyePosition;
						double rayX = 2.0 * ((float)(x + (i+RAND)/ANTIALIASING_SAMPLES)/XSIZE - 0.5);
						double rayY = 2.0 * (0.5 - (float)(y + (j+RAND)/ANTIALIASING_SAMPLES)/YSIZE);

						/* Calculate a primary ray. Inspired by:
						http://stackoverflow.com/questions/13078243/ray-tracing-camera */

						vec3 rayXVector, rayYVector;  
						mult(rayXVector, camera->cameraRight, rayX);
						mult(rayYVector, camera->cameraUp, rayY);

						vec3 rayVector = (rayXVector + rayYVector + focusDistanceVector);
						ray.D = rayVector;
      					ray.D.normalize();

						// Trace primary ray
						Colour tempCol = scene->raytrace(ray,6) * sampleWeight;
						col = col + tempCol;
					}
				}
			} else {
				ray.P = camera->eyePosition;
				double rayX = 2.0 * ((float)x/XSIZE - 0.5);
				double rayY = 2.0 * (0.5 - (float)y/YSIZE);

				/* Calculate a primary ray. Inspired by:
				http://stackoverflow.com/questions/13078243/ray-tracing-camera */

				vec3 rayXVector, rayYVector;  
				mult(rayXVector, camera->cameraRight, rayX);
				mult(rayYVector, camera->cameraUp, rayY);

				vec3 rayVector = (rayXVector + rayYVector + focusDistanceVector);
				ray.D = rayVector;
      			ray.D.normalize();

				// Trace primary ray
				col = scene->raytrace(ray,6);
			}

			// Save result in frame buffer
			frame_buffer[y][x].red = col.red;
			frame_buffer[y][x].green = col.green;
			frame_buffer[y][x].blue = col.blue;
		}
	}

	// OUTPUT IMAGE
 
	write_framebuffer();
}
