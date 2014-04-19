#ifndef _SCENE_H_
#define _SCENE_H_

#include "include/object.h"
#include "include/light.h"

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
      vec3 RefractVector(vec3 normal, vec3 incident, vec3 oldIncident, double refractIndex, bool internalReflection);

      const void createScene1(Camera *camera);
};

#endif
