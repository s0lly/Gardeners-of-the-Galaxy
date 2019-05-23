/*******************************************************************************************
*	Code and amendments by s0lly														   *
*	https://www.youtube.com/c/s0lly							                               *
*	https://s0lly.itch.io/																   *
*	https://www.instagram.com/s0lly.gaming/												   *
********************************************************************************************/

#pragma once

#include "Colors.h"

struct Atmosphere
{
	float oxygenLevel = 95.0f;
	float carbonDioxideLevel = 5.0f;
	float nitrogenLevel = 0.0f; //78.0f

	Color oxygenColor = Color(20, 50, 240);
	Color carbonDioxideColor = Color(255, 127, 0);
	Color nitrogenColor = Colors::White;

	float airPressure = 100.0f;
};