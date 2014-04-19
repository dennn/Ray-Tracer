#include <math.h>

#include "include/camera.h"

/* 
	This is used to calculate the Orthonormal Basis. Taken from
	Ray Tracing From the Ground Up (Listing 9.3)
*/
void Camera::calculateUVW()
{
	sub(cameraDirection, lookAt, eyePosition);
	cameraDirection.normalize();
	cross(cameraRight, cameraDirection, upVector);
	cameraRight.normalize();
	cross(cameraUp, cameraRight, cameraDirection);
}

Camera::Camera()
{
	eyePosition = vec4(0.0, 0.0, 0.0, 1.0);
 	lookAt = vec3(0.0, 0.0, -1.0);
 	upVector = vec3(0.0, -1.0, 0.0);

 	FOV = 40.0f;

 	calculateUVW();
}

float Camera::FOVToFocusDistance()
{
  float radiansFOV = FOV * static_cast<float>(M_PI) / 180.0f;

  return 1.0 / tan(radiansFOV / 2.0);

}
