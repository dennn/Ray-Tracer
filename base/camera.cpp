#include "include/camera.h"

/* 
	This is used to calculate the Orthonormal Basis. Taken from
	Ray Tracing From the Ground Up (Listing 9.3)
*/
void Camera::calculateUVW()
{
	w = (lookAt - eyePosition)
	w.normalise();
	u = upVector.cross(w);
	u.normalise();
	v = w.cross(u);
}