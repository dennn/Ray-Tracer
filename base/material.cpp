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

const void Material::generateWhiteColour()
{
	ka.red = 0.1;
	ka.green = 0.1;
	ka.blue = 0.1;
	kd.red = 1.0;
	kd.green = 1.0;
	kd.blue = 1.0;
	kr.red =  0.0;
	kr.green = 0.0;
	kr.blue = 0.0;
	ks.red = 0.5;
	ks.green = 0.5;
	ks.blue = 0.5;
	kt.red = 0.0;
	kt.green = 0.0;
	kt.blue = 0.0;
	n = 400.0;
}

void Material::generateRandomColour()
{
	double cr, cg, cb, ca;

	cr = _frand(); cg = _frand(); cb = _frand(); ca = _frand();

	ka.red = cr * 0.1;
	ka.green = cg * 0.1;
	ka.blue = cb * 0.1;
	kd.red = cr * 0.5;
	kd.green = cg * 0.5;
	kd.blue = cb * 0.5;
	kr.red =  0.0;
	kr.green = 0.0;
	kr.blue = 0.0;
	ks.red = 0.5;
	ks.green = 0.5;
	ks.blue = 0.5;
	kt.red = 0.0;
	kt.green = 0.0;
	kt.blue = 0.0;
	n = 400.0;
}

