#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "include/light.h"

class PointLight: public Light {
  	vec4 position;
  	vec3 direction;
  	Colour intensity;

	// http://ogldev.atspace.co.uk/www/tutorial20/tutorial20.html
  	float constantAttenuation;
  	float linearAttenuation;
  	float exponentialAttenuation;

  	bool spotLight;
 	
 	public:
 		PointLight(vec4 &p, Colour &c);
  		PointLight(vec4 &p, vec3 &d, Colour &c);
  		
 		virtual void getLightProperties(vec4 &pos, vec3 *ldir, Colour *i);
};

#endif
