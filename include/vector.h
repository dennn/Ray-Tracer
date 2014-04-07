#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "Vertex.h"

class Vector {
	public:
		double x;
		double y;
		double z;
		Vector(void);
		Vector(double x, double y, double z);
		void set(double x, double y, double z);
		void normalise(void);
		double dot(const Vector &b);
		Vector cross(const Vector &b);

		// Operators
		Vector operator * (const double &val);

		Vector operator - (const Vector &v);
		Vector operator + (const Vector &v);

		Vector operator - (const Vertex &v);
		Vector operator + (const Vertex &v);
};


#endif
