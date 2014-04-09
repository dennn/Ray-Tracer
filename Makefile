OBJS = raytrace.o base/scene.o base/colour.o base/material.o base/ray.o base/hit.o base/camera.o base/transformStack.o objects/object.o objects/sphere.o objects/triangle.o objects/plane.o lights/light.o lights/directional_light.o libs/nv_algebra.o

raytrace: $(OBJS) 
	g++ -o raytrace $(OBJS) -lm

.cpp.o:
	g++ -c -O -I. $< -o $@

raytrace.o: include/scene.h include/sphere.h include/plane.h include/directional_light.h include/camera.h

# Base

base/scene.o: include/scene.h

base/colour.o: include/colour.h

base/material.o: include/material.h

base/ray.o: include/ray.h

base/hit.o: include/hit.h

base/camera.o: include/camera.h

base/transformStack.o: include/transformStack.h

# Objects

objects/object.o: include/object.h

objects/sphere.o: include/sphere.h

objects/triangle.o: include/triangle.h

objects/plane.o: include/plane.h

# Lighting

lights/light.o: include/light.h

lights/directional_light.o: include/directional_light.h

# Base

include/scene.h: include/object.h include/light.h
	touch include/scene.h

include/material.h: include/colour.h
	touch include/material.h

include/ray.h: 
	touch include/ray.h

include/hit.h: 
	touch include/hit.h

include/camera.h: 
	touch include/camera.h

include/transformStack.h:
	touch include/transformStack.h

# Lighting

include/light.h:  include/ray.h include/colour.h
	touch include/light.h

include/directional_light.h: include/light.h
	touch include/directional_light.h

# Objects

include/object.h: include/ray.h include/colour.h include/material.h include/hit.h
	touch include/object.h

include/sphere.h: include/object.h
	touch include/sphere.h

include/triangle.h: include/object.h
	touch include/triangle.h

include/plane.h: include/object.h
	touch include/plane.h

clean: 
	rm raytrace raytrace.o base/camera.o base/colour.o base/hit.o base/material.o base/ray.o base/scene.o base/transformStack.o \
		libs/nv_algebra.o \
		lights/directional_light.o lights/light.o \
		objects/object.o objects/plane.o objects/sphere.o objects/triangle.o
