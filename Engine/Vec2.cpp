#include "Vec2.h"

Vec2 Vec2::operator*(Mat2 & rhs)
{
	return Vec2(x * rhs.a + y * rhs.c, x * rhs.b + y * rhs.d);
}
