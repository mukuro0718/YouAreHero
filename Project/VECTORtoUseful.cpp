#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"

VECTOR operator +(const VECTOR _a, const VECTOR _b)
{
	VECTOR out = _a;
	out.x += _b.x;
	out.y += _b.y;
	out.z += _b.z;
	return out;
}
VECTOR operator -(const VECTOR _a, const VECTOR _b)
{
	VECTOR out = _a;
	out.x -= _b.x;
	out.y -= _b.y;
	out.z -= _b.z;
	return out;
}
VECTOR operator *(const VECTOR _a, const float _b)
{
	VECTOR out = _a;
	out.x *= _b;
	out.y *= _b;
	out.z *= _b;
	return out;
}