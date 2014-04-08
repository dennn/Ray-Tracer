OBJS = raytrace.o base/scene.o base/vector.o base/vertex.o base/colour.o base/material.o base/ray.o base/hit.o base/camera.o objects/object.o objects/sphere.o objects/triangle.o objects/plane.o lights/light.o lights/directional_light.o

raytrace: $(OBJS) 
	g++ -o raytrace $(OBJS) -lm

.cpp.o:
	g++ -c -O -I. $< -o $@

raytrace.o: include/scene.h include/sphere.h include/plane.h include/directional_light.h include/spot_light.h include/camera.h

# Base

base/scene.o: include/scene.h

base/vector.o: include/vector.h include/vertex.h

base/vertex.o: include/vertex.h

base/colour.o: include/colour.h

base/material.o: include/material.h

base/ray.o: include/ray.h

base/hit.o: include/hit.h

base/camera.o: include/camera.h

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

include/ray.h: include/vector.h include/vertex.h
	touch include/ray.h

include/hit.h: include/vector.h include/vertex.h
	touch include/hit.h

include/camera.h: include/vector.h include/vertex.h
	touch include/camera.h

# Lighting

include/light.h: include/vector.h include/vertex.h include/ray.h include/colour.h
	touch include/light.h

include/directional_light.h: include/light.h
	touch include/directional_light.h

# Objects

include/object.h: include/vector.h include/vertex.h include/ray.h include/colour.h include/material.h include/hit.h
	touch include/object.h

include/sphere.h: include/object.h
	touch include/sphere.h

include/triangle.h: include/object.h
	touch include/triangle.h

include/plane.h: include/plane.h
	touch include/plane.h


