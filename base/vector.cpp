#include <math.h>

#include "include/vector.h"
#include "include/vertex.h"


Vector::Vector()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Vector::Vector(double px, double py, double pz)
{
	x = px;
	y = py;
	z = pz;
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
	return Vector(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
}

/*  Allow for vectors to be multiplied by a double or float value */
Vector Vector::operator * (const double &val) 
{
	return Vector(x*val, y*val, z*val);
}

/*  Allow for vectors to be subtracted from each other */
Vector Vector::operator - (const Vector &v) 
{
	return Vector(x-v.x, y-v.y, z-v.z);
}

/* Allow for vectors to be added to each other */
Vector Vector::operator + (const Vector &v) 
{
	return Vector(x+v.x, y+v.y, z+v.z);
}

/* Allow for Vertex to be taken away from vector */
Vector Vector::operator - (const Vertex &v)
{
	return Vector(x-v.x, y-v.y, z-v.z);
}

/* Allow for Vertex to be added to a vector */
Vector Vector::operator + (const Vertex &v)
{
	return Vector(x+v.x, y+v.y, z+v.z);
}