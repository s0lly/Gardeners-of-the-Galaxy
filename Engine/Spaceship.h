/*******************************************************************************************
*	Code and amendments by s0lly														   *
*	https://www.youtube.com/c/s0lly							                               *
*	https://s0lly.itch.io/																   *
*	https://www.instagram.com/s0lly.gaming/												   *
********************************************************************************************/

#pragma once

#include "Vec2.h"
#include "Colors.h"

struct Spaceship
{
	Vec2 loc{ 300.0f, 300.0f };
	float radius = 1800.0f;
	Color color = Colors::Magenta;
	float zLoc = 1810.0f;
	float zChange = 1.0f;
	bool hasArrived = false;
	float arrivalTime = 30.0f;
};