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

#define XSIZE 512
#define YSIZE 512
#define ANTIALIASING_SAMPLES 5

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
	Vector v;
	int x,y;
	int n;
	DirectionalLight *dl;
	Colour cl;

	srand(30115);

	clear_framebuffer();

	// Create a new scene to render
	scene = new Scene();

	// Create and add a directional light to the scene
	v.set(-1.0,-1.0,3.0);
	cl.set(1.0,1.0,1.0,1.0);
	
	dl = new DirectionalLight(v, cl);
	scene->addLight(*dl);

	// Add 10 random spheres to the scene
	for (n = 0; n < 10; n += 1)
	{
		Sphere *s;
		Material *m;
		Vertex p;

		// position
		p.set(frand()-0.5,frand()-0.5,frand()+5.0,1.0);

		// create with random radius
		s = new Sphere(p, frand()/2.0);

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
	Vertex p0, p1, p2;

	p0.set(-1.0, 0.0, 1.0, 1.0);
	p1.set(0.0, 1.0, 1.0, 1.0);
	p2.set(1.0, 0.0, 1.0, 1.0);

	t = new Triangle(p0, p1, p2);
	m = new Material();
	m->generateRandomColour();
	t->setMaterial(m);

	scene->addObject(*t);

	// Add a plane
	Plane *plane;
	Vertex p;
	p.set(0.0, 2.0, 0.0, 1.0);

	plane = new Plane(p, 40.0);
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

			/* If Anti-Aliasing is turned on then fire multiple rays per pixel block, using a jitter*/

			if (ANTIALIASING_SAMPLES > 1) {
				for (int i=0; i< ANTIALIASING_SAMPLES;i++) {
					for (int j=0; j < ANTIALIASING_SAMPLES;j++) {
						ray.P = camera->eyePosition;
						double rayX = 2.0 * ((float)(x + (i+RAND)/ANTIALIASING_SAMPLES)/XSIZE - 0.5);
						double rayY = 2.0 * (0.5 - (float)(y + (j+RAND)/ANTIALIASING_SAMPLES)/YSIZE);

						/* Calculate a primary ray. Inspired by:
						http://stackoverflow.com/questions/13078243/ray-tracing-camera */

						Vector rayVector = (camera->cameraRight * rayX + camera->cameraUp * rayY + camera->cameraDirection * focusDistance);
						ray.D = rayVector;
      					ray.D.normalise();

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

				Vector rayVector = (camera->cameraRight * rayX + camera->cameraUp * rayY + camera->cameraDirection * focusDistance);
				ray.D = rayVector;
      			ray.D.normalise();

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
