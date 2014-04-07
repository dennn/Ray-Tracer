#include <math.h>

#include "include/vector.h"

Vector::Vector()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Vector::Vector(double px, double py, double pz)
{
	Vector *v = new Vector();
	v->set(px, py, pz);
}

void Vector::set(double px, double py, double pz)
{
	x = px;
	y = py;
	z = pz;
}

void Vector::normalise(void)
{
	double l;

	l = x*x+y*y+z*z;
	l = sqrt(l);

	x = x/l;
	y = y/l;
	z = z/l;
}

double Vector::dot(const Vector &b)
{
	return (x*b.x)+(y*b.y)+(z*b.z);
}

/* 
	Calculate the cross product using:
	cx = aybz - azby
	cy = azbx - axbz
	cz = axby - aybx
*/

Vector Vector::cross(const Vector &b)
{
	Vector vectorC = Vector();
	vectorC.x = (y*b.z) - (z*b.y);
	vectorC.y = (z*b.x) - (x*b.z);
	vectorC.z = (x*b.y) - (y*b.x);

	return vectorC;
}

/*  Allow for vectors to be multiplied by a double or float value */
Vector Vector::operator * (double const d) 
{
	return Vector(x*d, y*d, z*d);
}

Vector& Vector::operator *= (double const s) {
	x *= s;
   	y *= s;
   	z *= s;

   return * this;
}

/*  Allow for vectors to be subtracted from each other */
Vector Vector::operator - (Vector const &v) 
{
	return Vector(x-v.x, y-v.y, z-v.z);
}

Vector& Vector::operator -= (Vector const &v) {
   	x -= v.x;
   	y -= v.y;
   	z -= v.z;

   	return * this;
}

/* Allow for vectors to be added to each other */
Vector Vector::operator + (Vector const &v) 
{
	return Vector(x+v.x, y+v.y, z+v.z);
}

Vector& Vector::operator += (Vector const &v) {
  	x += v.x;
   	y += v.y;
   	z += v.z;

   	return * this;
}