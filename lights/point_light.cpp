// a simple point light source

#include <math.h>

#include "include/point_light.h"

// specified by a direction and a colour

PointLight::PointLight(vec4 &p, Colour &i)
{
	position = p;

	// Set attentuation values
	// http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation
	// 1.0f, 0.007f, 0.0002f should give a range of 600m
	constantAttenuation = 1.0f;
	linearAttenuation = 0.007f;
	exponentialAttenuation = 0.0002f;

	spotLight = false;

	intensity.set(i.getRed(),i.getGreen(),i.getBlue(),i.getAlpha());
}

PointLight::PointLight(vec4 &p, vec3 &d, Colour &i)
{
	position = p;
	direction = d;
	direction.normalize();

	// Set attentuation values using this as guidance
	//https://developer.valvesoftware.com/wiki/Constant-Linear-Quadratic_Falloff
	constantAttenuation = 1.0f;
	linearAttenuation = 0.002f;
	exponentialAttenuation = 0.008f;

	spotLight = true;

	intensity.set(i.getRed(),i.getGreen(),i.getBlue(),i.getAlpha());
}

// provide the intensity and direction from which light arrives at given vertex

void PointLight::getLightProperties(vec4 &pos, vec3 *ldir, Colour *i)
{
	vec3 rayDirection = position - pos;

	float spotFactor;
	if (spotLight == true) {
		spotFactor = dot(rayDirection, direction);
		spotFactor = (1.0 - spotFactor);
	} else {
		spotFactor = 1;
	}

	// Calculates the length using sqrt(x^2 + y^2 + z^2)
	float distance = nv_norm(rayDirection);

	ldir->x = rayDirection.x;
	ldir->y = rayDirection.y;
	ldir->z = rayDirection.z;

	// Calculate the attenuation
	float attenuation = 1 / (constantAttenuation + linearAttenuation * distance + exponentialAttenuation * distance * distance);

	i->set(intensity.getRed()*attenuation * spotFactor,
		   intensity.getGreen()*attenuation * spotFactor,
		   intensity.getBlue()*attenuation * spotFactor,
		   intensity.getAlpha()*attenuation * spotFactor);

}
