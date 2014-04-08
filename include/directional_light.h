#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

#include "include/light.h"

class DirectionalLight: public Light {
  	vec3 direction;
  	Colour intensity;
 	
 	public:
  		DirectionalLight(vec3 &d, Colour &c);
 		virtual void getLightProperties(vec4 &pos, vec3 *ldir, Colour *i);
};

#endif
