#include <math.h>
#include <stdlib.h>

#include "include/material.h"

float _frand()
{
	int x;
	float f;

	x = rand();
	f = (float)(x & 0xffff);
	f = f/65536.0;

	return f;
}

/*
 * Solid colours
 */

const void Material::generateWhiteColour()
{
	ka.red = 0.5;
	ka.green = 0.5;
	ka.blue = 0.5;
	kd.red = 1.0;
	kd.green = 1.0;
	kd.blue = 1.0;
	kr.red =  0.8;
	kr.green = 0.8;
	kr.blue = 0.8;
	ks.red = 0.5;
	ks.green = 0.5;
	ks.blue = 0.5;
	kt.red = 0.0;
	kt.green = 0.0;
	kt.blue = 0.0;
	n = 50.0;
	refractiveIndex = 0.0;

	shiny = false;
	reflective = false;
	refractive = false;
}

const void Material::generateGreenColour()
{
	ka.red = 0.098;
	ka.green = 0.09;
	ka.blue = 0.09;
	kd.red = 0.0;
	kd.green = 0.8;
	kd.blue = 0.0;
	kr.red =  0.0;
	kr.green = 0.0;
	kr.blue = 0.0;
	ks.red = 0.8;
	ks.green = 0.8;
	ks.blue = 0.8;
	kt.red = 0.0;
	kt.green = 0.0;
	kt.blue = 0.0;
	n = 5.0;
	refractiveIndex = 0.0;

	shiny = true;
	reflective = false;
	refractive = false;
}

const void Material::generateGreyColour()
{
	ka.red = 0.447;
	ka.green = 0.447;
	ka.blue = 0.447;
	kd.red = 0.4;
	kd.green = 0.4;
	kd.blue = 0.4;
	kr.red =  0.5;
	kr.green = 0.5;
	kr.blue = 0.5;
	ks.red = 0.0;
	ks.green = 0.0;
	ks.blue = 0.0;
	kt.red = 0.0;
	kt.green = 0.0;
	kt.blue = 0.0;
	n = 27.0;
	refractiveIndex = 0.0;

	shiny = true;
	reflective = false;
	refractive = false;
}

const void Material::generateBlueColour()
{
	ka.red = 0.098;
	ka.green = 0.09;
	ka.blue = 0.09;
	kd.red = 0.0;
	kd.green = 0.0;
	kd.blue = 0.8;
	kr.red =  0.5;
	kr.green = 0.5;
	kr.blue = 0.5;
	ks.red = 1.0;
	ks.green = 1.0;
	ks.blue = 1.0;
	kt.red = 0.0;
	kt.green = 0.0;
	kt.blue = 0.0;
	n = 50.0;
	refractiveIndex = 0.0;

	shiny = true;
	reflective = true;
	refractive = false;
}


/*
 * Refractive colours
 */

const void Material::generateGlassMaterial()
{
	ka.red = 0.01;
	ka.green = 0.01;
	ka.blue = 0.01;
	kd.red = 0.01;
	kd.green = 0.01;
	kd.blue = 0.01;
	kr.red =  0.3;
	kr.green = 0.3;
	kr.blue = 0.3;
	ks.red = 0.2;
	ks.green = 0.2;
	ks.blue = 0.2;
	kt.red = 1.0;
	kt.green = 1.0;
	kt.blue = 1.0;
	n = 5.0;
	refractiveIndex = 1.05;

	shiny = true;
	reflective = false;
	refractive = true;
}

/*
 * Shiny colours
 */

const void Material::generateShinyWhiteColour()
{
	ka.red = 0.001;
	ka.green = 0.001;
	ka.blue = 0.001;
	kd.red = 0.7;
	kd.green = 0.7;
	kd.blue = 0.7;
	kr.red =  0.8;
	kr.green = 0.8;
	kr.blue = 0.8;
	ks.red = 1.0;
	ks.green = 1.0;
	ks.blue = 1.0;
	kt.red = 0.0;
	kt.green = 0.0;
	kt.blue = 0.0;
	n = 10.0;
	refractiveIndex = 2.5;

	shiny = true;
	reflective = true;
	refractive = true;
}

const void Material::generateShinyRedColour()
{
	ka.red = 0.123;
	ka.green = 0.023;
	ka.blue = 0.123;
	kd.red = 0.812;
	kd.green = 0.012;
	kd.blue = 0.812;
	kr.red =  0.5;
	kr.green = 0.5;
	kr.blue = 0.5;
	ks.red = 1.0;
	ks.green = 1.0;
	ks.blue = 1.0;
	kt.red = 0.0;
	kt.green = 0.0;
	kt.blue = 0.0;
	n = 10.0;
	refractiveIndex = 0.0;

	shiny = true;
	reflective = true;
	refractive = false;
}