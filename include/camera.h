#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "include/vector.h"
#include "include/vertex.h"

class Camera {
 	public:
 		Vector eyePosition;
 		Vector lookAt;
 		Vector upVector;

 		Vector u,v,w;

 		// Constructor
 		Camera() {
 			eyePosition = Vector();
 			eyePosition.set(0.0, 0.0, 100.0);
 			lookAt = Vector();
 			lookAt.set(0.0, 0.0, 0.0);
 			upVector = Vector();
 			upVector.set(0.0, 1.0, 0.0);

 			calculateUVW();
 		}

		void calculateUVW(); 		
};

#endif
