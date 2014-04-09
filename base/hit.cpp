#include <math.h>

#include "include/hit.h"
#include "include/object.h"

Hit::Hit(void)
{
}

void Hit::objectToWorldSpace()
{
	vec4 newPosition;
	mat4 transformation;
	invert(transformation, obj->inverseTransformation);
	mult(newPosition, transformation , p);

	p.x = newPosition.x;
	p.y = newPosition.y;
	p.z = newPosition.z;
	p.w = newPosition.w;

	vec4 newNormal, vec4OldNormal;
	mat4 transformTranspose;
	vec4OldNormal = vec4(n.x, n.y, n.z, 0);
	transpose(transformTranspose, obj->inverseTransformation);
	mult(newNormal, transformTranspose, vec4OldNormal);
	normalize(newNormal);

	n.x = newNormal.x;
	n.y = newNormal.y;
	n.z = newNormal.z;
}


