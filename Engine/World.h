/*******************************************************************************************
*	Code and amendments by s0lly														   *
*	https://www.youtube.com/c/s0lly							                               *
*	https://s0lly.itch.io/																   *
*	https://www.instagram.com/s0lly.gaming/												   *
********************************************************************************************/

#pragma once

#include "Vec2.h"
#include "Colors.h"

struct World
{
	Vec2 loc{ 0.0f, 0.0f };
	float radius = 800.0f;
	Color color;
};