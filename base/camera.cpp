#include "include/camera.h"

/* 
	This is used to calculate the Orthonormal Basis. Taken from
	Ray Tracing From the Ground Up (Listing 9.3)
*/
void Camera::calculateUVW()
{
	cameraDirection = lookAt - eyePosition;
	cameraDirection.normalise();
	cameraRight = cameraDirection.cross(upVector);
	cameraRight.normalise();
	cameraUp = cameraRight.cross(cameraDirection);
	cameraUp.normalise();
}

Camera::Camera()
{
	eyePosition = Vertex(0.0, 10.0, -5.0, 1.0);
 	lookAt = Vector(0.0, 0.0, 1.0);
 	upVector = Vector(0.0, -1.0, 0.0);

 	calculateUVW();
}