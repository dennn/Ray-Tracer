#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "include/vector.h"
#include "include/vertex.h"

class Camera {
 	public:
 		Vertex eyePosition;
 		Vector lookAt;
 		Vector upVector;

 		Vector cameraDirection, cameraUp, cameraRight;

 		// Constructor
 		Camera();

		void calculateUVW(); 		
};

#endif
