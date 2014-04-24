#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>     

// Scene
#include "include/scene.h"

// Camera
#include "include/camera.h"

#define XSIZE 512
#define YSIZE 512
#define ANTIALIASING_SAMPLES 4

#define RAND (float(rand())/float(RAND_MAX))

Colour frame_buffer[YSIZE][XSIZE];

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
	int x,y;
	int n;
	
	srand(30015);

	clear_framebuffer();

	// Create a new camera
	camera = new Camera();

	// Create a new scene to render
	scene = new Scene();
	scene->createScene2(camera);
//	scene->createScene2(camera);

	float focusDistance = camera->FOVToFocusDistance();
	float sampleWeight = 1.0f / (ANTIALIASING_SAMPLES * ANTIALIASING_SAMPLES);

	// RAYTRACE SCENE

	#pragma omp parallel for private(x)
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
						ray.P = camera->eyePosition;

						// Trace primary ray
						Colour tempCol = scene->raytrace(ray, 6, camera) * sampleWeight;
						col = col + tempCol;
					}
				}
			} else {
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
      			ray.P = camera->eyePosition;

				// Trace primary ray
				col = scene->raytrace(ray, 6, camera);
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
