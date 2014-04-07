#include "include/camera.h"

/* 
	This is used to calculate the Orthonormal Basis. Taken from
	Ray Tracing From the Ground Up (Listing 9.3)
*/
void Camera::calculateUVW()
{
	cameraDirection = (lookAt - eyePosition);
	cameraDirection.normalise();
	upVector.normalise();
	cameraRight = upVector.cross(cameraDirection);
	cameraUp = cameraRight.cross(cameraDirection);
}

Camera::Camera()
{
	eyePosition = Vertex(0.0, 0.0, -1.0, 0.0);
 	lookAt = Vector(0.0, 0.0, 1.0);
 	upVector = Vector(0.0, 1.0, 0.0);

 	calculateUVW();
}