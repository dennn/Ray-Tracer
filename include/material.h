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
      const void generateGreenColour();
      const void generateGreyColour();
      const void generateBlueColour();

      const void generateGlassMaterial();
      const void generateMirrorColour();

      const void generateShinyRedColour();
      const void generateShinyWhiteColour();
      const void generateShinyYellowColour();
      const void generateShinyBlackColour();

      const void generateSandColour();

  		bool shiny;
  		bool reflective;
  		bool refractive;

};

#endif
