#ifndef _COLOUR_H_
#define _COLOUR_H_

class Colour {

	public:
		float red;
		float green;
		float blue;
		float alpha;
		Colour(void);
		void clear(void);
		void set(float pred, float pgreen, float pblue, float palpha);
		float getRed(void);
		float getGreen(void);
		float getBlue(void);
		float getAlpha(void);

		Colour operator * (float s);
		Colour operator + (Colour const &c);
		void operator *= (const Colour c);
};

#endif
