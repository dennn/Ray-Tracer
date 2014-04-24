#include <math.h>

#include "include/hit.h"
#include "include/object.h"

Hit::Hit(void)
{
}

// http://www.unknownroad.com/rtfm/graphics/rt_normals.html 

void Hit::objectToWorldSpace()
{
	vec4 newPosition, oldPosition;
	oldPosition = vec4(p.x, p.y, p.z, 1);
	mult(newPosition, obj->transformation , oldPosition);

	p.x = newPosition.x;
	p.y = newPosition.y;
	p.z = newPosition.z;
	p.w = newPosition.w;

	vec4 newNormal, vec4OldNormal;
	mat4 transformTranspose;
	vec4OldNormal = vec4(n.x, n.y, n.z, 0);
	invert(transformTranspose, obj->transformation);
	transpose(transformTranspose);
	mult(newNormal, transformTranspose, vec4OldNormal);
	normalize(newNormal);

	n.x = newNormal.x;
	n.y = newNormal.y;
	n.z = newNormal.z;
}


