#include <math.h>

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
}

Camera::Camera()
{
	eyePosition = Vertex(0.0, 0.0, 1.0, 1.0);
 	lookAt = Vector(0.0, 0.0, 2.0);
 	upVector = Vector(0.0, -1.0, 0.0);

 	FOV = 40.0f;

 	calculateUVW();
}

float Camera::FOVToFocusDistance()
{
  float radiansFOV = FOV * static_cast<float>(M_PI) / 180.0f;

  return 1.0 / tan(radiansFOV / 2.0);

}
