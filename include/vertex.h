#ifndef _VERTEX_H_
#define _VERTEX_H_

class Vector;

class Vertex {
 	public:
  		double x;
  		double y;
  		double z;
  		double w;
  		Vertex(void);
 		Vertex(double x, double y, double z, double w);
  		void set(double x, double y, double z, double w);

};

#endif
