#include "include/colour.h"

#include <math.h>

#define NaNToZero(X) (isnan((X)) ? 0.0 : (X))

Colour::Colour()
{
	red   = 0.0;
	green = 0.0;
	blue  = 0.0;
	alpha = 0.0;
}

void Colour::clear(void)
{
	red   = 0.0;
	green = 0.0;
	blue  = 0.0;
	alpha = 0.0;
}

void Colour::set(float pred, float pgreen, float pblue, float palpha)
{
	red   = pred;
	green = pgreen;
	blue  = pblue;
	alpha = palpha;
}

float Colour::getRed(void)
{
	return red;
}

float Colour::getGreen(void)
{
	return green;
}

float Colour::getBlue(void)
{
	return blue;
}

float Colour::getAlpha(void)
{
	return alpha;
}

Colour Colour::operator * (float s) {
	Colour newColour;

	newColour.red = red * s;
	newColour.green = green * s;
	newColour.blue = blue * s;
	newColour.alpha = alpha * s;

	return newColour;
}


Colour Colour::operator + (Colour const &c) {
	
	Colour newColour;

	newColour.red = NaNToZero(c.red) + NaNToZero(red);
	newColour.green = NaNToZero(c.green) + NaNToZero(green);
	newColour.blue = NaNToZero(c.blue) + NaNToZero(blue);
	newColour.alpha = NaNToZero(c.alpha) + NaNToZero(alpha);

	return newColour;
}

void Colour::operator *= (const Colour c) {
   	red *= c.red;
   	green *= c.green;
   	blue *= c.blue;
   	alpha *= c.alpha;
}
