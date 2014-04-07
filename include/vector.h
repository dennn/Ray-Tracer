#ifndef _VECTOR_H_
#define _VECTOR_H_

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
  		Vector operator * (double const d);
  		Vector& operator *= (double const d);

		Vector operator - (Vector const &v);
		Vector& operator -= (Vector const &v);
		Vector operator + (Vector const &v);
		Vector& operator += (Vector const &v);
};


#endif
