#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "include/colour.h"

class Material {
 	public:
  		Colour ka; // Ambient
  		Colour kd; // Diffuse
  		Colour ks; // Specular
  		Colour kr; // Reflective
  		Colour kt; // Transparent
  		double n; // Shininess
      double refractiveIndex;

  		const void generateWhiteColour();
      const void generateGlassMaterial();
      const void generateShinyRedMaterial();
      const void generateMirrorMaterial();


  		bool shiny;
  		bool reflective;
  		bool refractive;

};

#endif
