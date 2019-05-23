/*******************************************************************************************
*	Code and amendments by s0lly														   *
*	https://www.youtube.com/c/s0lly							                               *
*	https://s0lly.itch.io/																   *
*	https://www.instagram.com/s0lly.gaming/												   *
********************************************************************************************/

#include "Vec2.h"

Vec2 Vec2::operator*(Mat2 & rhs)
{
	return Vec2(x * rhs.a + y * rhs.c, x * rhs.b + y * rhs.d);
}
