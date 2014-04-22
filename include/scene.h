#ifndef _SCENE_H_
#define _SCENE_H_

#include "include/object.h"
#include "include/light.h"

#include "libs/nv_math.h"
#include "libs/nv_mathdecl.h"
#include "libs/nv_algebra.h"

// Camera
#include "include/camera.h"

class Scene {
  	Object *scache;
  	Object *obj_list;
  	Light  *light_list;
	
	public:
		Scene(void);
  		void addObject(Object &obj);
  		void addLight(Light &light);
  		Colour raytrace(Ray &ray, int level);
  		bool shadowtrace(Ray &ray, double tlimit);

      vec3 ReflectionVector(vec3 vector, vec3 normal);
      vec3 SpecularVector(vec3 vector, vec3 normal);

      vec3 RefractVector(vec3 normal, vec3 incident, double refractIndex);

      const void createScene1(Camera *camera);
      const void createScene2(Camera *camera);

      vec3 CosineWeightedRandomHemisphereDirection(vec3 normal);
};

#endif
