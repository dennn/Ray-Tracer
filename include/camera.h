#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "libs/nv_math.h"
#include "libs/nv_mathdecl.h"
#include "libs/nv_algebra.h"

#include "include/colour.h"

class Camera {
	private: 
	 vec3 upVector;

 	public:
 		vec4 eyePosition;
 		vec3 lookAt;
 		float FOV;

 		vec3 cameraDirection, cameraUp, cameraRight;

 		// Constructor
 		Camera();

		void calculateUVW(); 	
		float FOVToFocusDistance();	

		Colour backgroundColour;
};

#endif
