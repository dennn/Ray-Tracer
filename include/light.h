#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "libs/nv_math.h"
#include "libs/nv_mathdecl.h"
#include "libs/nv_algebra.h"

#include "include/colour.h"

class Light {
  	Light *lt_next;
 	public:
  		Light(void);
  		void link(Light *light);
  		Light *next(void);
  		virtual void getLightProperties(vec4 &pos, vec3 *ldir, Colour *i);
};

#endif
